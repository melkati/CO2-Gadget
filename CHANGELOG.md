# Changelog

All notable changes to CO2-Gadget are documented in this file.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [Unreleased] — v0.16.014-beta (branch: development)

> **Note:** Versioning reset as of 10 May 2026. The previous v0.14.x line is superseded by v0.15.x on this branch.

### Fixed

#### Deep sleep & power
- **#283** — SCD41 low-power sensor readiness: replaced blocking `measureSingleShot(false)` with non-blocking `measureSingleShot(true)` before deep sleep to eliminate hidden 5-second delay; consume prepared measurement immediately on next wake, skipping redundant 5-second light sleep when data is already ready; fall back to fresh measurement when no prepared result exists
- **#283** — CM1106SL-NS RDY polarity fix: corrected active-low RDY wait logic (was waiting while LOW, now waits while HIGH); added 2-second timeout; power-down sensor after communication (EN LOW); validate CO2 > 0 before accepting reading
- **#283** — SCD40/SCD30 readiness optimization: skip readiness wait when data is already available, avoiding unnecessary light-sleep cycles
- **#249** — `measurementInterval` propagation: centralized `applyMeasurementIntervalToSensors()` as single source of truth; web handler now applies changes immediately; RTC persistence (`RTC_DATA_ATTR`) survives deep sleep; CM1106 low-power path receives measurement period; JSON export deduplicated (`measInterval` vs `measurementInterval`)
- **#276** — `sendMQTTOnWake` silently fails when `activeWifiOnWake` is disabled: auto-enable WiFi in `doSavePreferences()` before persisting to NVS
- **SCD41 warmup delay**: added 5-second warmup delay before first `sensors.loop()` call to ensure valid readings on cold boot
- **SCD41 wake reliability** (#266): SCD41 now runs in non-blocking mode on deep sleep timer wakes; `measureSingleShot(false)` starts measurement immediately on wake
- **SCD41 measurements survive deep sleep**: removed `powerDown()` call so SCD41 measurement state is preserved across sleep cycles
- **SCD41 watchdog reset**: interrupt watchdog fed during `scd41HandleFromDeepSleep()` polling loop to prevent spurious resets
- **Deep sleep cycle counters**: reload from NVS on wake; `0` now means "disabled" (was previously ambiguous)
- **WiFi/BLE/MQTT wake flags**: correctly reloaded from NVS on every wake (fixes lost configuration after first sleep cycle)
- **ESP32-S3 deep sleep** (TDISPLAY_S3): fixed deep sleep for ESP32-S3 variant
- **TTGO T-Display power sequencing**: reduced deep sleep current draw; TFT power-on sequencing corrected
- **WiFi credentials**: avoid repeated NVR writes on every wake cycle
- **E-Ink redraw triggers**: fixed redraw not triggering after deep sleep on GDEM0213B74
- **E-Ink partial refresh**: fixed partial refresh behavior on GDEM0213B74
- **E-Ink threshold consumption**: thresholds no longer consumed during BLE download (fixes alert triggering unexpectedly)
- **WiFi radio stop path**: split and stabilized for cleaner shutdown

#### Web UI
- **Charts timeout**: added timeout and `response.ok` check on circular buffer fetch
- **Preferences robustness**: retry with exponential backoff and error banner on load failure
- **Status polling**: separate polling intervals for different data sources; `response.ok` checks added
- **Captive portal polling**: reduced from 1s to 5s to reduce unnecessary traffic
- **Fetch helper**: `fetchWithTimeout` helper applied to all fetch calls for better reliability
- **Navbar consolidation**: unified navbar across all pages; fix status.html structure and missing links in low_power/ota
- **B74 E-Ink forced redraw**: use full refresh instead of partial update for forced redraws

#### Home Assistant MQTT
- **Battery device class**: fixed MQTT battery discovery metadata (`device_class: voltage`)
- **Entity IDs**: fixed Home Assistant MQTT discovery entity IDs

#### Low power mode
- **Binary simplification** (#248): simplified low power mode from 4-mode to binary (`HIGH_PERFORMANCE=0` / `LOW_POWER=1`); removed stale `MEDIUM_LOWPOWER` references throughout codebase
- **Runtime apply**: low power mode changes now applied at runtime without requiring reboot
- **Serial menu**: low power settings moved to dedicated submenu; explicit save required to persist changes

### Added

#### BTHome BLE (Home Assistant)
- **BTHome v2 advertising** (#285, by @Tazmania0) — new BLE protocol independent from Sensirion MyAmbiance. Home Assistant auto-discovers the gadget without MQTT or WiFi.
- **10 selectable measurement types**: CO₂, Temperature, Humidity, Battery (%, mV), Pressure, PM1.0/PM2.5/PM4.0/PM10. Granular selection in Web UI and serial menu with live byte-budget projection.
- **Optional AES-CCM encryption** — 32-hex bind key auto-generated on first boot; configurable via Web UI (Reveal/Copy/Regenerate) and serial menu.
- **Low-power-aware mode** — PM measurements are omitted during deep sleep (fan warm-up incompatible). Pressure on wake requires `SUPPORT_LOW_POWER_PRESSURE`. Selections are retained and resume automatically in HIGH_PERFORMANCE mode.
- **Wake advertisement from deep sleep** — BTHome publishes immediately on timer/sensor wake with whichever sensor data was read (CO₂+temp+hum for SCD sensors, CO₂-only for CM1106/S8).
- **Coexistence with MyAmbiance** — BTHome can run with Sensirion service on or off; when both active, BTHome rides in the scan response.
- **`POST /getBTHomeBindKey` endpoint** — dedicated HTTP endpoint with confirmation prompt and no-store headers to reveal the bind key when encryption is active.
- **TFT/E-Ink status icon** — 16×16 BTHome icon in the status icon strip.

- **Firmware build date/time in web UI**: version string now includes build timestamp (`/status` endpoint)
- **TFT screenshot capture**: new web endpoint `GET /screenshot` returns PNG screenshot of the display

### Changed

- **Web UI**: comprehensive GUI refactor — navbar restructure, responsive gauges, CO2 color unification, low power & WHO presets in preferences
- **Large-screen dashboard**: fluid layout for wide screens; unified Charts toolbar
- **Charts export**: CSV and JSON export now available directly from Charts page
- **Calibration live readings**: CO2 gauge shows live readings during calibration flow
- **Low power mode display text**: corrected text on status page for low power mode
- **Serial menu editing**: improved flow for editing low power settings; defer persistence to save action
- **Uptime retention**: uptime now retained across deep sleep cycles (not reset on wake)
- **RSSI cache**: WiFi RSSI cached in RTC memory for low-power display updates
- **BLE advertisement sampling**: Sensirion BLE advertisement sample primed for faster first reading

---

## v0.15.000 — 10 May 2026 (branch: modernization/v2)

> Major recovery and modernization sprint after ~20 months of accumulated work in side branches, 18 closed issues, a complete web UI redesign, and BLE reactivation.

### Added

#### Web UI — Apple-inspired redesign
- Full dark/light mode with CSS custom properties (`--bg`, `--surface`, `--text`, `--accent`, etc.)
- Premium card-based layout with subtle shadows and border-radius throughout
- Animated theme toggle button (sun/moon) persisted via `localStorage`
- Responsive design improvements for mobile (hamburger nav on small screens)
- SVG inline tooltip icons — consistent blue (#0071E3) across all pages, no emoji rendering artifacts
- Colored feature badges on status page (green/yellow/red based on feature state)
- Preferences page: gear icon in navbar instead of text link; OTA moved to a "Maintenance" card inside Preferences
- Status page: value+unit wrapped in single `<span>` for correct alignment

#### Web UI — New pages
- `charts.html` — Real-time circular buffer data visualization with:
  - Responsive Chart.js graphs (CO2, temperature, humidity)
  - Date/time range filter
  - CSV and JSON data export
  - Cache-busting JavaScript loading
- `calibration.html` — Dedicated calibration page with:
  - Live CO2 readings display
  - Forced Re-Calibration (FRC) form with 400–2000 ppm validation
  - Current `pendingCalibration` status indicator
  - Navbar link from all other pages

#### Firmware — Features
- **Deep sleep / Low power mode** (`CO2_Gadget_DeepSleep.h`): full deep sleep cycle management, wake-up reason tracking, boot/wake time counters
- **Battery detection** (`CO2_Gadget_Battery.h`): improved threshold calculation with voltage hysteresis; `hasBattery` / `workingOnExternalPower` logic
- **Low power web page** (`webserver/low_power.html`): 4-tab interface (Display, Bluetooth, MQTT, ESP-NOW) for threshold configuration
- **ThresholdManager** (`libs/CO2_Gadget_Thresholds/`): NVS-persisted per-output thresholds (absolute + percentage) for CO2, temperature, humidity
- **Wake display on CO2 alert** (`wakeDisplayOnCO2Alert`): automatically wakes display when CO2 rises above orange threshold; hysteresis prevents repeated wakes
- **e-Ink redraw fixes**: `cyclesToRedrawDisplay` / `cyclesLeftToRedrawDisplay` split and cleaned; no longer saved to NVS on every cycle
- **Display reverse on wake**: display orientation restored correctly after deep sleep wake
- **Low power icon** in web server navbar
- **Threshold-gated publishing** (#212): `ThresholdManager.evaluateThresholds()` suppresses MQTT/BLE/ESP-NOW publishes when values haven't changed enough
- **SPS30 particulate sensor** (#97): PM1.0–PM10 published via MQTT with Home Assistant discovery
- **Unified outputs** (#176): `CO2_Gadget_Outputs.h` — single include for Buzzer, Neopixel, GPIO relay outputs
- **GxEPD2Out ArduinoMenu driver** (#225): Menu now works on e-ink displays
- **Show/hide status icons** (#237): `showStatusIcons` preference for TFT/OLED/e-ink
- **BLE re-enabled**: `SUPPORT_BLE` reactivated with `melkati/arduino-ble-gadget#historyInterval-v2`
- **SCD4x calibration via BLE**: FRC command handled over BLE from MyAmbiance App
- **WiFi settings via BLE**: SSID/password changeable from MyAmbiance App

#### e-ink boards
- New board support: WeAct DEPG0213BN, GxEPD2_290_BS
- New logo and icon for e-ink splash screen
- Font size and alignment improvements

#### Infrastructure
- Migrated to ESP32Async ecosystem: `AsyncTCP @ ^3.4.10`, `ESPAsyncWebServer @ ^3.11.0`
- ElegantOTA with async mode (`-DELEGANTOTA_USE_ASYNC_WEBSERVER=1`)
- ArduinoJson pinned to `7.0.1`; ArduinoMenu pinned to `4.21.4`
- C++17 enabled (`-std=gnu++17`)

### Fixed

- **#219** — iOS CNA could not load `index.html.gz`: `serveGzippedFile()` detects iOS User-Agent and serves uncompressed fallback
- **#90** — MQTT topic length: direct-to-`JsonDocument` approach; pool size 1024 → 4096 bytes
- **#218 / #223** — T-Display S3 backlight: DW8904 complete shutdown via EN pin; wake restores prior brightness
- **#80** — Display staying off on CO2 alert: `wakeDisplayOnCO2Alert` triggers wake at orange threshold
- **#237** — Status icons not hideable: `showStatusIcons` preference implemented
- MQTT icon reversed logic fixed (was showing when inactive)
- Serial menu unresponsive with e-ink displays fixed
- SCD30 deep sleep re-initialization fixed (`initialized` made `static`)
- Captive portal web server not starting after button-press wake fixed

### Changed

- Navbar updated: Preferences via gear icon, Charts and Calibration added
- `#include "CO2_Gadget_Outputs.h"` replaces dual Neopixel + Buzzer includes
- `-fpermissive` compiler flag removed
- T-Display S3 port updated to COM17

---

## [0.14.006] — Oct 2024

Last stable release on `master` before the modernization sprint.

- Previous stable baseline
- BLE support active (older arduino-ble-gadget API)
- No deep sleep / low power
- Basic web UI (no charts, no calibration page)
- No ThresholdManager
- AsyncWebServer 2.x ecosystem
