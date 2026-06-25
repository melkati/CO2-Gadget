# Firmware Architecture

> Overview of the CO2-Gadget firmware structure, boot flow, and major subsystems.
> **Audience:** Developers contributing to or modifying the firmware.

---

## File Structure

```
CO2_Gadget.ino                        # Entry point: setup() + loop()
CO2_Gadget_Sensors.h                  # Sensor initialization, reading callbacks, PM variables
CO2_Gadget_Battery.h                  # Battery voltage reading and level calculation
CO2_Gadget_Outputs.h                  # Unified outputs (Buzzer + NeoPixel + GPIO relays)
CO2_Gadget_Buzzer.h                   # Piezo buzzer driver
CO2_Gadget_Neopixel.h                 # WS2812B addressable LED driver
CO2_Gadget_Preferences.h              # NVS preferences persistence (save/load/JSON import-export)
CO2_Gadget_Thresholds.h               # ThresholdManager for gated publishing (in libs/)
CO2_Gadget_DeepSleep.h                # Low power / deep sleep management
CO2_Gadget_Menu.h                     # On-screen menu system (ArduinoMenu library)
CO2_Gadget_Buttons.h                  # Push button handling (Button2 library)
CO2_Gadget_WIFI.h                     # WiFi + Web server + REST API + Captive Portal
CO2_Gadget_Improv.h                   # Improv WiFi serial configuration
CO2_Gadget_BLE.h                      # BLE (Sensirion MyAmbiance App)
CO2_Gadget_MQTT.h                     # MQTT client + Home Assistant Discovery
CO2_Gadget_ESP-NOW.h                  # ESP-NOW communication
CO2_Gadget_TFT.h                      # TFT display driver (TFT_eSPI)
CO2_Gadget_OLED.h                     # OLED display driver (U8g2)
CO2_Gadget_EINK.h                     # E-Ink display driver (GxEPD2)
CO2_Gadget_CircularBufferManager.h    # Rolling data buffer for web charts
GxEPD2Out.h                           # ArduinoMenu output driver for GxEPD2 e-ink displays
libs/CO2_Gadget_Thresholds/           # Threshold manager library (NVS persistence)
webserver/                            # Web UI source files (HTML, JS, CSS)
data/                                 # SPIFFS binary assets (minified + gzipped)
```

---

## Boot Flow

```
Power On / Reset
      │
      ▼
setup()
  ├── Serial.begin()
  ├── initPreferences()           # Load all NVS settings → RAM globals
  ├── initSensors()               # CanAirIO auto-detect + configure
  │     └── applyMeasurementIntervalToSensors()
  ├── initDisplay()               # TFT / OLED / E-Ink (board-specific)
  ├── initOutputs()               # GPIO, NeoPixel, Buzzer
  ├── initBLE()                   # (if SUPPORT_BLE)
  ├── initWIFI()                  # WiFi + Captive Portal + Web Server
  ├── initMQTT()                  # (if SUPPORT_MQTT)
  ├── initESPNow()                # (if SUPPORT_ESPNOW)
  ├── initMenu()                  # ArduinoMenu system
  └── fromDeepSleepTimer()?       # Check wake reason
        ├── Yes: skip full init, restore RTC flags
        └── No: cold boot, run full sequence
```

---

## Main Loop

```
loop()
  ├── readingsLoop()              # Every loop iteration
  │     ├── sensors.loop()        # CanAirIO sensor polling
  │     ├── applyMeasurementIntervalToSensors()  # Runtime interval changes
  │     ├── publishBLE()          # (if SUPPORT_BLE, gated by thresholds)
  │     ├── publishMQTT()         # (if SUPPORT_MQTT, gated by thresholds)
  │     ├── publishESPNow()       # (if SUPPORT_ESPNOW, gated by thresholds)
  │     ├── checkWakeOnCO2Alert() # Auto-wake display at orange threshold
  │     └── updateDisplay()       # Refresh display if values changed
  │
  ├── outputsLoop()               # Buzzer + NeoPixel + relay state machine
  ├── server.handleClient()       # Async web server
  ├── mqttLoop()                  # MQTT connection + incoming commands
  ├── menuLoop()                  # ArduinoMenu input processing
  ├── handleBLE()                 # BLE connection state
  └── checkDeepSleep()            # Enter deep sleep if conditions met
```

---

## Deep Sleep Cycle

```
HIGH_PERFORMANCE mode
  └── Continuous operation, no sleep

LOW_POWER mode
  └── loop() runs normally
        └── After `timeSleeping` seconds of inactivity:
              └── toDeepSleep()
                    ├── Save critical state to RTC memory
                    ├── Configure wake timer
                    └── esp_deep_sleep_start()

Wake (timer)
  └── setup() → fromDeepSleepTimer()
        ├── Restore WiFi/BLE/MQTT flags from RTC (or NVS fallback)
        ├── Cycle counters: bootCount++, wakeCount++
        ├── WiFi connect every N wake cycles
        ├── MQTT publish every N wake cycles
        ├── Display redraw every N wake cycles
        └── Resume loop()
```

### RTC Memory
Variables declared with `RTC_DATA_ATTR` are preserved across deep sleep:
- `measurementInterval` — Sensor sampling interval
- WiFi/BLE/MQTT wake flags (with NVS fallback in case of RTC corruption)
- Boot and wake cycle counters
- Calibration warm-up progress

---

## ThresholdManager

The `ThresholdManager` library (`libs/CO2_Gadget_Thresholds/`) provides gated publishing to reduce bandwidth and power consumption.

### Architecture

```
ThresholdConfig (per output type)
  ├── co2: { absolute: 50, percentage: 0 }    # PPM or %
  ├── temp: { absolute: 2.0, percentage: 0 }  # °C or %
  └── hum: { absolute: 5.0, percentage: 0 }   # %RH or %

Output types:
  ├── DISPLAY_SHOW  → TFT/OLED/E-Ink redraw
  ├── MQTT_SEND     → MQTT publish
  ├── BLE_SEND      → BLE data update
  └── ESPNOW_SEND   → ESP-NOW transmit
```

Thresholds are persisted in NVS and configurable via web UI (`low_power.html`) or REST API (`/saveThresholds`).

---

## Display System

Three independent display drivers, selected at compile time via feature flags:

| Driver | Library | Board Flavors |
|---|---|---|
| `CO2_Gadget_TFT.h` | TFT_eSPI | TTGO_TDISPLAY, TDISPLAY_S3, esp32dev-ST7789 |
| `CO2_Gadget_OLED.h` | U8g2 | esp32dev_OLED |
| `CO2_Gadget_EINK.h` | GxEPD2 | All ttgo-t5/ttgo-t7 e-ink variants |

`GxEPD2Out.h` provides an ArduinoMenu-compatible output driver for e-ink displays with partial refresh support.
