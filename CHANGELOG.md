# Changelog

All notable changes to CO2-Gadget are documented in this file.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [Unreleased] — v0.15.000-dev (branch: modernization/v2)

### Sprint 1 — 7-10 May 2026

This release represents a major recovery and modernization sprint after ~20 months of accumulated work in side branches, 18 closed issues, a complete web UI redesign, and BLE reactivation.

---

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

#### Firmware — Features merged from development-low-power
- **Deep sleep / Low power mode** (`CO2_Gadget_DeepSleep.h`): full deep sleep cycle management, wake-up reason tracking, boot/wake time counters
- **Battery detection** (`CO2_Gadget_Battery.h`): improved threshold calculation with voltage hysteresis; `hasBattery` / `workingOnExternalPower` logic
- **Low power web page** (`webserver/low_power.html`): 4-tab interface (Display, Bluetooth, MQTT, ESP-NOW) for threshold configuration
- **ThresholdManager** (`libs/CO2_Gadget_Thresholds/`): NVS-persisted per-output thresholds (absolute + percentage) for CO2, temperature, humidity
- **Wake display on CO2 alert** (`wakeDisplayOnCO2Alert` preference): automatically wakes display when CO2 rises above orange threshold; hysteresis prevents repeated wakes
- **e-Ink redraw fixes**: `cyclesToRedrawDisplay` / `cyclesLeftToRedrawDisplay` split and cleaned; no longer saved to NVS on every cycle
- **Display reverse on wake**: display orientation restored correctly after deep sleep wake
- **Low power icon** in web server navbar

#### Firmware — New features (Sprint 1)
- **Threshold-gated publishing** (#212): `ThresholdManager.evaluateThresholds()` now called in `publishMQTT()`, `publishBLE()`, `publishESPNow()` — saves bandwidth and power by suppressing publishes when values haven't changed enough. `forcePublish=true` bypasses thresholds.
- **SPS30 particulate sensor** (#97): PM1.0, PM2.5, PM4.0, PM10 values read from `canairio_sensorlib` and published via MQTT. MQTT HA discovery topics added with correct `device_class` and `state_class`.
- **Unified outputs** (#176): `CO2_Gadget_Outputs.h` created — single include for Buzzer, Neopixel, GPIO relay outputs. `initOutputs()` replaces `initGPIO()` + `initNeopixel()` + `initBuzzer()`.
- **GxEPD2Out ArduinoMenu driver** (#225): `GxEPD2Out.h` — template ArduinoMenu output driver for GxEPD2 e-paper displays, with `flush()` calling `epd.display(true)` for partial update. Menu now works on e-ink displays.
- **Show/hide status icons** (#237): `showStatusIcons` NVS preference controls visibility of WiFi/MQTT/BLE status icons on TFT/OLED/e-ink displays.
- **BLE re-enabled**: `SUPPORT_BLE` reactivated with `melkati/arduino-ble-gadget#historyInterval-v2` and `h2zero/NimBLE-Arduino @ ^2.3.4`.
- **SCD4x calibration via BLE**: `FRC` command handled in `CO2_Gadget_BLE.h` — allows forced recalibration from MyAmbiance App.
- **WiFi settings via BLE**: SSID/password can be changed over BLE from MyAmbiance App.

#### Firmware — e-ink improvements (from development-low-power)
- New e-ink board support: WeAct DEPG0213BN, GxEPD2_290_BS
- New logo and icon for e-ink splash screen
- Font size and alignment improvements
- Serial menu responsiveness fix for e-ink displays

#### Infrastructure
- Migrated to ESP32Async ecosystem: `ESP32Async/AsyncTCP @ ^3.4.10`, `ESP32Async/ESPAsyncWebServer @ ^3.11.0`
- ElegantOTA updated with async mode via `-DELEGANTOTA_USE_ASYNC_WEBSERVER=1`
- ArduinoJson pinned to `7.0.1` (7.4.x broke `VariantOperators` API)
- ArduinoMenu pinned to `4.21.4` (newer versions require `streamFlow`)
- C++17 enabled: `-std=gnu++17` build flag (required by Sensirion UPT Core 1.3.0)
- Build unflags added to remove framework's default C++11/14 flags
- Log files removed from git tracking (`.gitignore` updated)

### Fixed

- **#219** — iOS CNA (Captive Network Assistant) could not load `index.html.gz`: new `serveGzippedFile()` function detects iOS User-Agent and serves uncompressed fallback HTML.
- **#90** — MQTT topic length limited by intermediate buffer copy: replaced with direct-to-`JsonDocument` approach; JSON pool size increased from 1024 to 4096 bytes.
- **#218 / #223** — T-Display S3 backlight never fully powered off: DW8904 backlight IC complete shutdown via EN pin held LOW for >3 ms; wake restores to prior brightness level.
- **#80** — Display staying off when CO2 rises to dangerous levels: `wakeDisplayOnCO2Alert` triggers display wake at orange threshold with hysteresis.
- **#237** — Status icon line not hideable: `showStatusIcons` preference implemented for TFT, OLED and e-ink drivers.
- Fixed MQTT icon hidden when MQTT is *active* (reversed logic — was showing when inactive).
- Fixed serial menu being slow/unresponsive with e-ink displays.
- Fixed e-ink `cyclesToRedrawDisplay` vs `cyclesLeftToRedrawDisplay` confusion (separate tracking variables).
- Fixed SCD30 deep sleep re-initialization (`initialized` made `static`).
- Fixed captive portal web server not starting after wake-up with button press.

### Changed

- Navbar updated across all pages: Preferences accessed via gear icon, Charts and Calibration added.
- `CO2_Gadget.ino`: replaced dual `#include` (Neopixel + Buzzer) with single `#include "CO2_Gadget_Outputs.h"`.
- JsonDocument pool size: 1024 → 4096 bytes in MQTT preferences handler.
- `-fpermissive` compiler flag removed (no longer needed with ESP32Async/AsyncTCP 3.x).
- T-Display S3 monitor/upload port updated to COM17.

### Removed

- Removed intermediate buffer copy in MQTT topic preference setter.
- Removed `initGPIO()`, `initNeopixel()`, `initBuzzer()` as top-level calls (now inside `initOutputs()`).

---

## [0.14.006] — Oct 2024

Last stable release on `master` before the modernization sprint.

- Previous stable baseline
- BLE support active (older arduino-ble-gadget API)
- No deep sleep / low power
- Basic web UI (no charts, no calibration page)
- No ThresholdManager
- AsyncWebServer 2.x ecosystem
