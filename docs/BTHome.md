# BTHome BLE — CO₂ Gadget

Complete reference for the BTHome v2 BLE advertising feature: how it is built,
the on-air payload format, the sensor-selection model, encryption, deep-sleep
behaviour, every configuration surface, and the full commit history that
produced it.

BTHome lets the gadget broadcast its measurements as standard
[BTHome v2](https://bthome.io/) BLE service-data advertisements that
**Home Assistant** auto-discovers — no MQTT, no Wi-Fi, no cloud. It runs
independently of (and alongside) the Sensirion "MyAmbience" Gadget BLE service.

---

## 1. Build configuration

BTHome is gated behind two PlatformIO build flags (`platformio.ini`):

```ini
-DSUPPORT_BLE              ; BLE support (NimBLE + Sensirion Gadget BLE)
-DSUPPORT_BTHOME_BLE       ; BTHome v2 advertisements. Requires SUPPORT_BLE.
-DSUPPORT_LOW_POWER_PRESSURE ; (opt-in) read BME280 pressure on deep-sleep wake. Needs SUPPORT_BTHOME_BLE + SUPPORT_LOW_POWER.
```

`SUPPORT_LOW_POWER_PRESSURE` is commented out by default; enable it to advertise
barometric pressure on low-power deep-sleep wakes (see §5 and §8).

`SUPPORT_BTHOME_BLE` **requires** `SUPPORT_BLE`; this is enforced at compile
time in `CO2_Gadget_BLE.h`:

```c
#if defined(SUPPORT_BTHOME_BLE) && !defined(SUPPORT_BLE)
#error "SUPPORT_BTHOME_BLE requires SUPPORT_BLE"
#endif
```

Extra libraries pulled in only for BTHome: `esp_mac.h`, `esp_random.h`,
`mbedtls/ccm.h` (AES-CCM encryption). NimBLE provides the advertiser.

When `SUPPORT_BTHOME_BLE` is undefined, the runtime symbols collapse to
`constexpr` no-ops (`activeBTHome = false`, `bthomeEncryption = false`,
`bthomeBindKey = ""`) so the rest of the firmware compiles unchanged.

---

## 2. Runtime state model

| Variable | Type | Scope | Meaning |
|---|---|---|---|
| `enableBLE` | `bool` | global | Master BLE radio enable. |
| `activeBLE` | `bool` | global | Sensirion "MyAmbience" GATT/advert output. |
| `activeBTHome` | `bool` | global | BTHome advertising output. |
| `bthomeEncryption` | `bool` | global | Encrypt BTHome adverts (AES-CCM). |
| `bthomeBindKey` | `String` | global | 32-hex (16-byte) AES key shared with HA. |
| `bthomeCounter` | `uint32_t` | global | Monotonic encryption counter (anti-replay). |
| `bthomeSensors` | `uint32_t` | global | Bitmask: **user's desired** measurement set. |
| `bthomeFreshMeasurements` | `uint32_t` | global | Bitmask: measurements **populated this read cycle**. |

`enableBLE`, `activeBLE`, and `activeBTHome` are orthogonal. BTHome can run with
the Sensirion service off (`activeBTHome && !activeBLE`) — it then owns the
primary advertisement; with both on, BTHome rides in the **scan response** while
Sensirion owns the primary advert.

---

## 3. BTHome v2 payload format

Service data is advertised under UUID **`0xFCD2`**. The first byte is the BTHome
**device-info** byte:

| Constant | Value | Meaning |
|---|---|---|
| `BTHOME_DEVICE_INFO` | `0x44` | BTHome v2, trigger-based, **not** encrypted. |
| `BTHOME_DEVICE_INFO_ENCRYPTED` | `0x45` | BTHome v2, trigger-based, encrypted. |

### Plain payload

```
[0x44] [0x00 packetId] [obj-id data]... 
```

`0x00` is the BTHome packet-id object; its value is `bthomePacketId`, incremented
each publish so HA can de-duplicate repeated adverts.

### Encrypted payload (AES-CCM)

```
[0x45] [ciphertext] [counter(4, LE)] [MIC(4)]
```

The ciphertext is the measurement objects **without** the packet-id object
(the 4-byte counter replaces it). See [§7 Encryption](#7-encryption).

### Service-data byte budget

The 31-byte legacy advert leaves a conservative **24 usable bytes**
(`BTHOME_MAX_SERVICE_DATA = 24`) for BTHome service data after the flags and
service-data AD headers. Framing overhead removed from that budget:

| Mode | Overhead | Bytes for measurements |
|---|---|---|
| Plain | `1` (device info) + `2` (packet-id) = **3** | 21 |
| Encrypted | `1` (device info) + `4` (counter) + `4` (MIC) = **9** | 15 |

> When both Sensirion and BTHome are active, BTHome rides in the scan response
> (~27 bytes available), but the budget is always computed against the
> conservative 24-byte primary-advert figure, so a measurement is never dropped
> *only* in scan-response mode.

---

## 4. Measurement catalogue

Defined once in `BTHOME_MEASUREMENTS[]` (`CO2_Gadget_BTHome.h`) and mirrored in the
web UI / emulator. Stored in **ascending object-id order**, which is also the
emit order BTHome receivers expect.

| Key | Label | Obj ID | Bytes | Priority | Group | Encoding |
|---|---|---|---|---|---|---|
| `co2` | CO2 | `0x12` | 3 | 1 | core | `uint16` ppm |
| `temperature` | Temperature | `0x02` | 3 | 2 | core | `int16` ×0.01 °C |
| `humidity` | Humidity | `0x03` | 3 | 3 | core | `uint16` ×0.01 % |
| `battery` | Battery | `0x01` | 2 | 4 | core | `uint8` % |
| `pm25` | PM2.5 | `0x0D` | 3 | 5 | optional | `uint16` µg/m³ |
| `pressure` | Pressure | `0x04` | 4 | 6 | optional | `uint24` ×0.01 hPa |
| `pm10` | PM10 | `0x0E` | 3 | 7 | optional | `uint16` µg/m³ |
| `voltage` | Battery Voltage | `0x0C` | 3 | 8 | optional | `uint16` mV (×0.001 V) |
| `pm1` | PM1.0 | `0xEE` | 3 | 9 | nonnative | `uint16` µg/m³ |
| `pm4` | PM4.0 | `0xEF` | 3 | 10 | nonnative | `uint16` µg/m³ |

**Priority** (1 = highest) decides what survives when the selection does not all
fit the byte budget. **Group** drives the UI grouping:

- **core** — primary native measurements.
- **optional** — additional native BTHome measurements.
- **nonnative** — PM1.0 / PM4.0 have **no** standard BTHome object. They use
  private IDs `0xEE`/`0xEF`, are emitted **last** (so a receiver parses every
  standard object before hitting the unknown tail), and are **not** parsed by
  Home Assistant.

Default selection (`BTHOME_DEFAULT_SENSOR_MASK`):
`battery | temperature | humidity | pressure | co2 | pm25 | pm10`.

---

## 5. Availability, validity, freshness & fit

Three independent gates decide whether a selected measurement is actually
broadcast:

### Availability — *can this hardware ever produce it?*
`bthomeMeasurementAvailable(bit)` (`CO2_Gadget_BTHome.h`):

- **CO2** — `UNIT::CO2` registered, or retained from a CO2 sensor on deep-sleep wake.
- **Temperature** — `UNIT::TEMP` or `UNIT::CO2TEMP` registered (SCD30/SCD4x,
  MH-Z19), or retained from an SCD sensor on wake.
- **Humidity** — `UNIT::HUM` or `UNIT::CO2HUM` (SCD30/SCD4x only), or retained.
- **Pressure** — `UNIT::PRESS` (BME280-class).
- **PM1/PM2.5/PM4/PM10** — `UNIT::PM25` registered.
- **Battery** — always available (battery level is always encodable).
- **Voltage** — only when `hasBattery`.

> CO2-only sensors (CM1106, SenseAir S8) register **only** `UNIT::CO2` and have
> no temperature/humidity reading, so temp/hum are correctly **not** inferred
> from `UNIT::CO2` — otherwise a phantom `0.00 °C` / `0 %RH` would be broadcast.
> (Fixed in `0f556cf`; see [§13](#13-commit-history).)

> Pressure is normalized to hPa in `onSensorDataOk()`: the CanAirIO lib returns
> BME280 pressure in **Pa** (~101325) but BMP280/BME680 in hPa (~1013). Values
> that look like Pa are divided by 100, so BME280 pressure passes the 300–1100 hPa
> validity range instead of being silently dropped.

**Availability is mode-aware.** When low-power mode is enabled
(`deepSleepData.lowPowerMode != HIGH_PERFORMANCE`):

- **PM1/PM2.5/PM4/PM10 are reported unavailable** — particulate sensors need a
  multi-second fan warm-up incompatible with the ~0.3 s deep-sleep wake budget,
  so they are never read or advertised on wake.
- **Pressure** is available in low-power only when built with
  `SUPPORT_LOW_POWER_PRESSURE`; otherwise it is reported unavailable.

The descriptor JSON carries an `unavailableReason` (`"lowpower"` / `"notdetected"`
/ `""`) so the Web UI and serial menu show *"not advertised in low-power mode —
selection retained"* instead of the generic "not detected" for a sensor the device
physically has. Selections are **retained** and resume automatically when the
device returns to high-performance mode. This availability gate is also the
authoritative runtime reason PM is excluded from a wake advert (freshness remains
as defence-in-depth).

### Validity — *is the current reading in a sane range?*
`bthomeMeasurementValid(bit)` requires freshness **and** a range check:
CO2 400–5000 ppm, temp −40…85 °C, hum 0–100 %, pressure 300–1100 hPa,
battery ≤ 100 %, voltage 1–6 V (and `hasBattery`). PM values have no range gate.

### Freshness — *was it populated this read cycle?*
`bthomeFreshMeasurements` is a runtime mask, **recomputed each read cycle**:

- Continuous mode — reset and repopulated in `onSensorDataOk()`
  (`CO2_Gadget_Sensors.h`) from the currently-registered units, so a sensor that
  stops responding drops out instead of advertising a stale value.
- Low-power wake — reset and set in `handleLowPowerSensors()`
  (`CO2_Gadget_DeepSleep.h`) per the woken sensor type.
- Battery and voltage are treated as always-fresh.

### Fit — *what fits the byte budget?*
`bthomeFitMask(encrypted, includePacketId)` greedily admits selected + available
+ valid measurements **in priority order** until the budget is exhausted. The
result is the set actually emitted. `isValidBTHomeMeasurement()` returns true
when at least one **non-battery** measurement is selected/available/valid, so a
battery-only payload never triggers an advert on its own.

---

## 6. Publishing & threshold gating

`publishBLE()` (`CO2_Gadget_BLE.h`) is the single publish path, called from the
main loop and from deep-sleep wake. Per cycle it:

1. Computes `validForSensirion` (CO2+temp+hum valid) and `validForBTHome`
   (any non-battery measurement eligible via `bthomeFitMask`).
2. Derives per-dimension evaluate flags — `evaluateCO2/Temp/Hum` — true only
   when that dimension is actually being published by *some* output.
3. Evaluates the `BLE_SEND` thresholds via
   `evaluateBLEPublishThresholds(co2, temp, hum, evaluateCO2, evaluateTemp, evaluateHum)`.
   Only the evaluated dimensions can trip a threshold or update their baseline,
   so a dimension that isn't being sent never forces (or suppresses) a publish.
4. If thresholds pass (or a keepalive timeout is due, or `bypassThresholds`):
   writes the Sensirion sample (if active) and/or rebuilds the BTHome advert.

Threshold logic lives in `libs/CO2_Gadget_Thresholds`. `checkAndMaybeUpdateThresholds`
and `evaluateThresholdsAt` take the three `evaluate*` flags (default `true` for
backward compatibility) and only consider / update the baseline for enabled
dimensions.

> Battery-level updates no longer force a standalone BTHome advert; battery is
> carried with the next threshold/keepalive-driven payload.

---

## 7. Encryption

AES-CCM (128-bit) via mbedTLS, matching the BTHome v2 encrypted spec.

| Constant | Value |
|---|---|
| `BTHOME_ENCRYPTION_KEY_SIZE` | 16 bytes (32 hex chars) |
| `BTHOME_ENCRYPTION_MIC_SIZE` | 4 bytes |
| `BTHOME_ENCRYPTION_NONCE_SIZE` | 13 bytes |

**Nonce (13 B)** = `MAC[6]` ‖ `0xD2 0xFC` (UUID, LE) ‖ `0x45` (encrypted
device-info) ‖ `counter[4]` (LE). The BT MAC is read via `esp_read_mac(…, ESP_MAC_BT)`.

**Counter (`bthomeCounter`)** is the replay-protection nonce input and must be
strictly monotonic across the key's lifetime:

- Incremented on **every** encryption.
- Persisted to NVS at most every `BTHOME_COUNTER_SAVE_INTERVAL` (64) encryptions,
  or forced.
- The value written to NVS is `bthomeCounter + BTHOME_COUNTER_NVS_RESERVE`
  (4096) — a **reserve** so that an unexpected reset never reuses a counter that
  was used since the last save (`getBTHomeCounterNVSValue()`).
- On cold boot, `seedBTHomeCounter()` jumps the counter by a random
  `(esp_random() & 0x0FFF) + 1` to avoid collisions after NVS loss; on
  deep-sleep wake the in-RAM counter is retained and not re-seeded.

**Bind key** is 32 hex characters. `ensureBTHomeBindKey()` normalizes the stored
value and, if missing/invalid, generates a cryptographically-random key and
persists it. The same key must be entered in Home Assistant.

---

## 8. Deep sleep / low power

BLE output state is preserved across deep sleep in RTC memory
(`bleWakeSettings_t`, `CO2_Gadget_DeepSleep.h`), validated by a magic value and
checksum:

```c
constexpr uint32_t BLE_WAKE_SETTINGS_MAGIC = 0xB1E20207;
```

The struct snapshots `enableBLE`, `activeBLE`, `activeBTHome`,
`bthomeEncryption`, `bthomeBindKey`, **`bthomeSensors`**, and `bthomeCounter` on
the way down, and restores them on wake (`saveBLEWakeSettingsToRTC` /
`restoreBLEWakeSettingsFromRTC`). The magic is bumped whenever the struct layout
changes so a stale RTC blob from an older firmware is rejected.

On a timer/sensor wake, `handleLowPowerSensors()` reads the woken CO2 sensor,
sets `bthomeFreshMeasurements` for what it actually read (SCD → CO2+temp+hum;
CM1106SL-NS → CO2), and `publishBLE(ignoreMeasurementInterval=true)` installs a
single wake advert. `bthomeMeasurementAvailable()` consults `deepSleepData.co2Sensor`
to know whether temp/hum survived the wake without a fresh read.

**PM is never read on a wake** (fan warm-up). **Pressure is read on wake only when
built with `SUPPORT_LOW_POWER_PRESSURE`**: the BME280 is re-`begin()`'d and a single
**forced** measurement is taken (so the sensor returns to sleep between wakes),
then `BTHOME_SEL_PRESS` is marked fresh. A retained `deepSleepData.hasPressureOnWake`
flag (captured during normal operation) gates this read. Mode-aware
`bthomeMeasurementAvailable()` (see §5) is the authoritative gate that keeps PM —
and pressure when the flag is off — out of the wake advert.

---

## 9. Configuration surfaces

### 9.1 Serial / on-device menu (`CO2_Gadget_Menu.h`)

```
BLE Config
├── Global BLE        (enableBLE)
├── MyAmbience        (activeBLE — Sensirion output)
└── BTHome
    ├── BTHome output (activeBTHome)
    ├── Encryption    (bthomeEncryption)
    ├── BTHome key    (enter 32 hex; serial editor)
    └── Publish Sensors
        ├── Core               (CO2, Temperature, Humidity, Battery)
        ├── Optional           (PM2.5, Pressure, PM10, Battery Voltage)
        └── No BTHome object   (PM1.0, PM4.0)
```

`bthomeSensors` is a bitmask, but ArduinoMenu TOGGLEs bind to plain bools, so
the mask is mirrored into ten `bthomeSel*` bools (`syncBTHomeSensorMirrors()`)
and recombined on change (`doSetBTHomeSensors()`). Entering the menu prints a
per-measurement status report and a live payload-fit projection
(`printBTHomePayloadProjection()`). Toggle selectability mirrors the Web UI
(`updateBTHomeSensorMenuAvailability()`, re-evaluated on each menu enter): a
toggle is enabled when the measurement is available, low-power-blocked, or already
selected (so a retained selection can be dropped); a not-detected **and**
unselected sensor's toggle is **disabled** (you can't select absent hardware). The
status report distinguishes "Not advertised in low-power mode" from "Not detected"
to match the Web UI wording.

### 9.2 Web UI (`webserver/preferences.{html,js}`, `style.css`)

- **Sensor selection** — grouped checkbox list (Core / Optional / No BTHome
  object) with a live **payload budget hint** that recomputes on every toggle and
  on encryption change (`renderBTHomeSensors`, `enforceBTHomeBudget`). Unavailable
  states are kept compact (markers + a footnote, not long inline text):
  - **Not detected** (no such hardware) — marked `†`. A new selection can't be
    added (checkbox **disabled** when unselected), but an already-selected sensor
    is **retained** (survives a transient/failed-boot non-detection) and stays
    **enabled so it can be unchecked** to drop a permanently removed sensor.
  - **Not advertised in low-power mode** (hardware present but not sampled on a
    deep-sleep wake) — stays **selectable**, marked `*`; resumes automatically in
    high-performance mode.
  - **No valid reading / omitted by budget** — annotated inline as before.
- **Bind key** — shown only when encryption is active. The stored key is **not**
  embedded in the page; explicit controls:
  - **Reveal** — `POST /getBTHomeBindKey` (confirmation prompt; no-store headers).
  - **Copy** — to clipboard.
  - **Regenerate** — stages a fresh `crypto.getRandomValues` key (saved only on Save).
- The bind key is excluded from the general `getActualSettingsAsJson` dump and
  served only by the dedicated endpoint. (Exposing the key to the device owner —
  via Web UI or serial — is **intentional**: HA needs the same key.)
- **Backup/Restore caveat:** because the bind key is excluded from the settings
  dump, a **Backup** (`preferences_backup.json`) does **not** contain it. After a
  **Restore**, reveal/re-enter or regenerate the key and re-pair Home Assistant.
  The bind-key panel shows this note while encryption is active.

### 9.3 Preferences / NVS keys (`CO2_Gadget_Preferences.h`)

| NVS key | Type | Default |
|---|---|---|
| `activeBTHome` | bool | `false` |
| `bthomeEncrypt` | bool | `false` |
| `bthomeBindKey` | string | generated |
| `bthomeCounter` | uint32 | `0` |
| `bthomeSensors` | uint32 | `BTHOME_DEFAULT_SENSOR_MASK` |

`handleSavePreferencesFromJSON()` accepts a partial `bthomeSensors` object
(`{key: bool}`) via `bthomeApplySelectionJson()` and triggers
`refreshBLEOutputs()` when any BLE/BTHome setting actually changed.

### 9.4 Status icon (`CO2_Gadget_TFT.h`, `icons.h`)

`showBTHomeIcon()` draws the 16×16 `iconBTHome` bitmap when
`enableBLE && activeBTHome` (and, on wake, `deepSleepData.activeBLEOnWake`).

### 9.5 Web emulator (`webEmu/server.py`)

Mirrors the firmware descriptor list, budget math, per-sensor availability
toggles, and the `/getBTHomeBindKey` / partial-selection save semantics so the
web UI can be developed without hardware.

---

## 10. End-to-end data flow

```
sensors.loop() ──▶ onSensorDataOk()
                     • update co2/temp/hum/pressure/pm*
                     • recompute bthomeFreshMeasurements
                          │
main loop ──▶ publishBLE()
                 • validForSensirion / validForBTHome
                 • evaluate BLE_SEND thresholds (per-dimension)
                 • if pass/keepalive:
                     ├─ writeSensirionCurrentSample()  (if activeBLE)
                     └─ updateBTHomeAdvertisementData()
                            • bthomeFitMask → buildBTHomeMeasurements
                            • plain → [0x44][pktid][objs]
                            • encrypted → encryptBTHomePayload → [0x45][ct][ctr][mic]
                            • NimBLE setServiceData → primary advert or scan response
```

Settings changes (menu / web / wake) funnel through `refreshBLEOutputs()` →
`refreshBTHomeBLESettings()`, which (re)initialises or tears down BLE and
republishes **without** a full radio teardown when avoidable.

---

## 11. Home Assistant integration

1. Enable **BTHome** (and optionally **Encryption**) on the gadget.
2. If encrypted, copy the **bind key** (Reveal in Web UI, or the serial menu).
3. HA → *Settings → Devices & Services* auto-discovers the gadget as a BTHome
   device; paste the bind key when prompted.
4. Selected native measurements appear as entities. **PM1.0/PM4.0 do not appear**
   (non-standard object IDs).

---

## 12. Known limitations

- PM1.0 / PM4.0 are broadcast under private IDs and are ignored by Home Assistant.
- **In low-power (deep-sleep) mode, PM is not advertised** — particulate sensors
  cannot warm up within the wake budget. The selection is retained and resumes in
  high-performance mode.
- **Barometric pressure is advertised on a deep-sleep wake only when built with
  `SUPPORT_LOW_POWER_PRESSURE`** (off by default). Currently only the BME280 is
  re-read on wake; BMP280/BME680 pressure-on-wake is a possible follow-up.
- Temperature is encoded in °C; the gadget is assumed to operate in Celsius
  (it never calls the sensor library's `setTemperatureUnit`).
- The byte budget is computed conservatively (24 B) even when scan-response mode
  offers more room, so a low-priority measurement may be dropped slightly earlier
  than strictly necessary.
- A PM-only selection (no CO2/temp/hum) publishes only on the keepalive timeout,
  since PM values are not threshold-tracked.

---

## 13. Commit history

All BTHome work, traced chronologically (non-merge commits,
2026-06-04 → 2026-06-22). Grouped by phase.

### Phase 1 — Foundation
| Date | Commit | Summary |
|---|---|---|
| 06-04 | `ef71b02` | Add independent BTHome BLE support |
| 06-05 | `ae09361` | Add encrypted BTHome advertisements (AES-CCM) |

### Phase 2 — Menu & global BLE control
| Date | Commit | Summary |
|---|---|---|
| 06-06 | `8e301fe` | Improve serial menu editing flows |
| 06-06 | `1ded8d1` | Add global BLE enable menu control |

### Phase 3 — Deep sleep / wake advertising
| Date | Commit | Summary |
|---|---|---|
| 06-07 | `d8883a4` | Fix BTHome BLE publishing after deep sleep |
| 06-07 | `3e61cbf` | Improve BTHome wake advertising in deep sleep |
| 06-07 | `564062a` | Harden BLE wake settings RTC retention |
| 06-09 | `4d0fb68` | Improve BTHome wake advertising reliability |
| 06-09 | `d69ee6f` | Add BTHome battery voltage payload (0x0C) |
| 06-09 | `fcee0ed`, `f3f7ab9`, `83b6c2c` | Prime Sensirion BLE advertisement sample |

### Phase 4 — Status icon
| Date | Commit | Summary |
|---|---|---|
| 06-08 | `cd0942d` | Add BTHome status icon |
| 06-08 | `1049034` | Show BTHome status icon by mode |
| 06-13 | `030fb45` | Clear display status icon strip before redraw |

### Phase 5 — Independence from the BLE feature gate / Web UI
| Date | Commit | Summary |
|---|---|---|
| 06-11 | `fc68d25` | Add WebUI gating for BTHome preferences |
| 06-12 | `f604321` | Keep BTHome independent in preferences UI |
| 06-12 | `c30369f` | Preserve BTHome when Sensirion BLE is disabled |
| 06-12 | `64641b8` | Keep global BLE enabled for BTHome output |
| 06-13 | `d361e6f` | Keep BTHome independent from BLE feature gate |

### Phase 6 — Particulate matter + counter hardening
| Date | Commit | Summary |
|---|---|---|
| 06-12 | `ab75aea` | Add PM2.5/PM10 to BTHome BLE advertisements |
| 06-12 | `cf9bdc8` | Harden BTHome encryption counter handling |
| 06-12 | `bb9cf5d` | Remove duplicated BLE wake RTC state |
| 06-14 | `048dfad` | Keep BTHome encryption counters monotonic |

### Phase 7 — Live refresh without BLE teardown
| Date | Commit | Summary |
|---|---|---|
| 06-14 | `5e42b55` | Fix BtHome |
| 06-14 | `b9d9c1c` | Fix live BTHome encryption refresh |
| 06-14 | `3c8e6bc` | Restore serial password visibility recovery |
| 06-14 | `02dc76b` | Avoid BLE teardown for BTHome settings refresh |
| 06-14 | `605e4a6` | Restore BTHome preferences visibility |
| 06-14 | `3725e96` | Reuse BTHome wake payload across advert slots |

### Phase 8 — Availability-aware, user-selectable sensors
| Date | Commit | Summary |
|---|---|---|
| 06-21 | `508da35` | Availability-aware, user-selectable BTHome sensors |
| 06-21 | `360d8b2` | Refine grouped sensor selection |
| 06-21 | `26947be` | Respect selected sensors and publish thresholds |
| 06-21 | `ce90eb2` | Emulate selectable BTHome sensors (webEmu) |
| 06-21 | `95d3dd2` | Reject unavailable sensor selections |
| 06-21 | `796e414` | Clear unavailable BTHome selections (webEmu) |
| 06-21 | `eea62f0` | Sanitize persisted sensor selections |

### Phase 9 — Hardening & correctness
| Date | Commit | Summary |
|---|---|---|
| 06-22 | `9e54238` | Harden runtime state and bind-key access |
| 06-22 | `f7e18fe` | Clarify encryption and payload overflow states |
| 06-22 | `0ee6246` | Stabilize BTHome sensor tooltips |
| 06-22 | `778b48a` | Correct temp/hum availability, freshness reset, menu mapping |
| 06-23 | `9222123` | Add low-power-aware availability, pressure-on-wake, clearer selection UI |
| 06-23 | `1835906` | Emulate low-power BTHome availability and reason field |

> Interspersed `Merge branch 'modernization/v2'/'development'` merges are omitted.
> The current branch name is `feat-bthome-ble`.

---

## 14. File map

| File | BTHome responsibility |
|---|---|
| `CO2_Gadget.ino` | Global state, selection bitmask/enum, RTC wake struct field. |
| `CO2_Gadget_BLE.h` | Shared BLE orchestration: `initBLE`/`publishBLE`/`refreshBLEOutputs`/`BLELoop`, `bleThresholdState_t` gating; includes `CO2_Gadget_BTHome.h`. |
| `CO2_Gadget_BTHome.h` | BTHome internals: measurement table, encoding, fit/budget, encryption, payload build, advert install. |
| `CO2_Gadget_Sensors.h` | Read sensors, `pressureHpa`, freshness mask. |
| `CO2_Gadget_Preferences.h` | NVS load/save, bind-key ensure/normalize, settings JSON, save-from-JSON. |
| `CO2_Gadget_DeepSleep.h` | RTC wake-settings retention, low-power sensor read + freshness. |
| `CO2_Gadget_Menu.h` | Serial/on-device BTHome menus and sensor-selection toggles. |
| `CO2_Gadget_WIFI.h` | `/getBTHomeBindKey` HTTP endpoint. |
| `CO2_Gadget_TFT.h`, `icons.h` | BTHome status icon. |
| `libs/CO2_Gadget_Thresholds` | Per-dimension threshold evaluation. |
| `webserver/preferences.{html,js}`, `style.css` | Web UI: selection list, budget hint, bind-key controls. |
| `webEmu/server.py` | Emulator mirroring firmware BTHome behaviour. |
