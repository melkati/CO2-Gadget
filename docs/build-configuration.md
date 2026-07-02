# Build Configuration Reference

> Complete guide to CO2-Gadget build options, feature flags, and platformio.ini configuration.
> **Audience:** Developers compiling from source.

---

## Feature Flags

Define these in `platformio.ini` under `[features]` → `build_flags`. Comment out any line to disable the feature.

| Flag | Default | Purpose |
|---|---|---|
| `SUPPORT_BLE` | ON | BLE / Sensirion MyAmbiance App via NimBLE-Arduino |
| `SUPPORT_BTHOME_BLE` | ON | BTHome v2 BLE advertisements for Home Assistant. Requires `SUPPORT_BLE`. |
| `SUPPORT_LOW_POWER_PRESSURE` | **OFF** | Read BME280 pressure on deep-sleep wake (forced I²C per wake). Needs `SUPPORT_BTHOME_BLE` + `SUPPORT_LOW_POWER`. |
| `SUPPORT_BUZZER` | ON | Passive piezo buzzer for CO₂ alerts |
| `SUPPORT_ESPNOW` | **OFF** | ESP-NOW long-range radio protocol |
| `SUPPORT_MDNS` | ON | mDNS hostname resolution (`CO2-Gadget.local`) |
| `SUPPORT_MQTT` | ON | MQTT client for publishing + commands |
| `SUPPORT_MQTT_DISCOVERY` | ON | Home Assistant auto-discovery via MQTT |
| `SUPPORT_OTA` | ON | Over-the-air firmware updates via ElegantOTA |
| `SUPPORT_LOW_POWER` | ON | Deep sleep / low power mode support |
| `SUPPORT_CIRCULAR_BUFFER` | ON | In-memory rolling data buffer for web charts |
| `SUPPORT_TFT` | Board-specific | TFT color display (ST7789, ST7735) |
| `SUPPORT_OLED` | Board-specific | OLED display (SSH1106, SSD1306) |
| `SUPPORT_EINK` | Board-specific | E-Ink display (GxEPD2) |
| `SUPPORT_CAPTIVE_PORTAL` | **Always ON** | Captive portal for first-boot WiFi config (defined in `CO2_Gadget.ino`) |

---

## Debug Flags

Uncomment in `platformio.ini` under `[debug]` → `build_flags`:

| Flag | Purpose |
|---|---|
| `DEBUG_EINK` | E-Ink display debug output |
| `DEBUG_NEOPIXEL` | NeoPixel LED debug |
| `DEBUG_PREFERENCES` | Print preferences on load/save |
| `DEBUG_WIFI_EVENTS` | WiFi connection events |
| `DEBUG_BLE` | BLE service/advertising debug |
| `DEBUG_THRESHOLDS` | Threshold evaluation debug |
| `DEBUG_IMPROV_WIFI` | Improv Serial WiFi config debug |
| `DEBUG_CAPTIVE_PORTAL` | Captive portal DNS/HTTP debug |
| `WIFI_PRIVACY` | Hide passwords in serial output |
| `CORE_DEBUG_LEVEL` | ESP-IDF log level: 0=None, 1=Error, 2=Warning, 3=Info, 4=Debug, 5=Verbose |

---

## Board Flavors (Build Environments)

All available environments are listed under `[platformio]` → `default_envs` in `platformio.ini`.

| Flavor | Chip | Display | BLE Support |
|---|---|---|---|
| `esp32dev` | ESP32 | None | ✅ |
| `esp32dev_OLED` | ESP32 | OLED 128×64 (SSH1106) | ✅ |
| `esp32dev-ST7789_240x320` | ESP32 | TFT 240×320 (ST7789) | ✅ |
| `TTGO_TDISPLAY` | ESP32 | TFT 240×135 (ST7789) | ✅ ⚠️ Flash tight (93.1%) |
| `TTGO_TDISPLAY_SANDWICH` | ESP32 | TFT 240×135 (ST7789) | ✅ |
| `TDISPLAY_S3` | ESP32-S3 | TFT 320×170 (ST7789) | ✅ |
| `ttgo-t5-EINKBOARDGDEM0213B74` | ESP32 | E-Ink 2.13″ (GDEM0213B74) | ✅ |
| `ttgo-t5-EINKBOARDDEPG0213BN` | ESP32 | E-Ink 2.13″ (DEPG0213BN) | ✅ |
| `ttgo-t5-EINKBOARDGDEW0213M21` | ESP32 | E-Ink 2.13″ (GDEW0213M21) | ✅ |
| `ttgo-t7-EINKBOARDGDEM029T94` | ESP32 | E-Ink 2.9″ (GDEM029T94) | ✅ |
| `ttgo-t7-WEACT_GDEH0154D67` | ESP32 | E-Ink 1.54″ (GDEH0154D67) | ✅ |
| `ttgo-t7-WEACT_DEPG0213BN` | ESP32 | E-Ink 2.13″ (DEPG0213BN) | ✅ |
| `ttgo-t7-WEACT_GxEPD2_290_BS` | ESP32 | E-Ink 2.9″ (GDEM029C90) | ✅ |

**Build example:**
```bash
pio run -e TTGO_TDISPLAY --target upload
```

---

## Library Dependencies

| Library | Version | Source |
|---|---|---|
| AsyncTCP | `^3.4.10` | ESP32Async |
| ESPAsyncWebServer | `^3.11.0` | ESP32Async |
| ArduinoJson | `7.4.3` | bblanchon |
| ArduinoMenu | `4.21.4` (exact) | neu-rah |
| TFT_eSPI | `^2.5.43` | bodmer |
| GxEPD2 | `^1.5.6` | zinggjm |
| U8g2 | `^2.32.6` | olikraus |
| Adafruit NeoPixel | `^1.10.3` | adafruit |
| NimBLE-Arduino | `^2.3.4` | h2zero |
| arduino-ble-gadget | `#historyInterval-v2` | melkati (fork) |
| canairio_sensorlib | latest | kike-canaries |

---

## Known Limitations

### TTGO_TDISPLAY Flash Usage
With BLE enabled: **93.1%** flash used (1.65 MB / 1.77 MB app partition), only ~120 KB headroom. Any significant feature addition may overflow. Consider:
- Disabling BLE on TTGO_TDISPLAY builds if not needed
- Using a custom partition table with a larger app partition

### TDISPLAY_S3 Screenshot
`TFT_eSPI::readPixel()` is inherently unstable on the 8-bit parallel interface used by TDISPLAY_S3, producing approximately 290 colors instead of 65K. This is a hardware limitation, not a bug.
