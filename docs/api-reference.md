# REST API Reference

> Complete reference of HTTP endpoints provided by the CO2-Gadget web server.
> **Audience:** Developers integrating with external systems (Home Assistant, Node-RED, Grafana, etc.).

**Base URL:** `http://<device-ip>/` or `http://<hostname>.local/`

---

## Sensor Readings (GET)

| Endpoint | Returns | Example |
|---|---|---|
| `/readCO2` | Current CO₂ in PPM | `892` |
| `/readTemperature` | Current temperature (°C) | `24.5` |
| `/readHumidity` | Current relative humidity (%) | `45.2` |
| `/readBatteryVoltage` | Battery voltage (mV) | `4100` |

**Note:** Temperature is always in Celsius regardless of display unit setting.

---

## System Information (GET)

| Endpoint | Returns |
|---|---|
| `/status` | JSON: `co2`, `temp`, `hum`, `battery`, `rssi`, `uptime`, `freeHeap`, `minFreeHeap`, `lastResetReason`, `wifiStatus`, `mqttStatus`, `bleStatus`, `bthomeStatus`, `espNowStatus` |
| `/getVersion` | JSON: `version` (firmware), `build` (timestamp) |
| `/getFeaturesAsJson` | JSON: enabled features as boolean flags |
| `/getFreeHeap` | Free heap memory in bytes |
| `/getMinFreeHeap` | Minimum free heap since boot |
| `/getMeasurementInterval` | Current measurement interval in seconds |
| `/pingServer` | `"pong"` |

### `/status` example response:
```json
{
  "co2": 892,
  "temp": 24.5,
  "hum": 45.2,
  "battery": 4100,
  "rssi": -65,
  "uptime": 123456,
  "freeHeap": 84200,
  "minFreeHeap": 62100,
  "lastResetReason": "DEEP_SLEEP_WAKE",
  "wifiStatus": "connected",
  "mqttStatus": "connected",
  "bleStatus": "active",
  "bthomeStatus": "active",
  "espNowStatus": "disabled"
}
```

---

## Configuration

### Get All Preferences (GET)
```
GET /getActualSettingsAsJson
```
Returns JSON object with all current settings (sensor type, thresholds, WiFi config, display settings, etc.).

### Set Individual Preference (GET)
```
GET /settings?Parameter=Value
```
Sets a single preference and applies it immediately where possible.

**Common parameters:**
| Parameter | Value | Description |
|---|---|---|
| `MeasurementInterval` | 10–3600 | Sensor sampling interval (seconds) |
| `CalibrateCO2` | 400–2000 | Forced Re-Calibration target (PPM) |
| `Brightness` | 0–255 | Display brightness |
| `TimeToOff` | 0–3600 | Display auto-off timeout (seconds, 0=always on) |
| `OffOnUSB` | true/false | Turn display off when on USB power |
| `Orientation` | Normal/Reversed | Display orientation |
| `ShowTemperature` | true/false | Show temperature on display |
| `ShowHumidity` | true/false | Show humidity on display |
| `ShowBattery` | true/false | Show battery on display |
| `ShowStatusIcons` | true/false | Show WiFi/BLE/MQTT icons on display |
| `WakeOnCO2Alert` | true/false | Auto-wake display on CO₂ orange threshold |
| `LowPowerMode` | 0/1 | 0=HIGH_PERFORMANCE, 1=LOW_POWER |
| `CO2OrangeRange` | 400–5000 | Orange threshold (PPM) |
| `CO2RedRange` | 400–5000 | Red threshold (PPM) |
| `TempUnit` | Celsius/Fahrenheit | Display temperature unit |
| `TempOffset` | 0–50 | Temperature offset (°C) |
| `BatteryFull` | 3000–5000 | Full battery voltage (mV) |
| `BatteryEmpty` | 2000–4000 | Empty battery voltage (mV) |
| `VoltageRef` | 800–1200 | ADC voltage reference (mV) |
| `BuzzerMode` | 0/1/2 | 0=OFF, 1=One time, 2=Every N seconds |
| `BuzzerTone` | 300–2000 | Buzzer frequency (Hz) |
| `BuzzerSpan` | 50–300 | Buzzer duration (ms) |
| `NeopixBright` | 0–255 | NeoPixel brightness |
| `HistoryInterval` | 10–3600 | Circular buffer sampling interval (seconds) |

### Get BTHome Bind Key (POST)
```
POST /getBTHomeBindKey
```
Returns the BTHome AES-CCM bind key (32 hex characters). Requires a confirmation prompt on the client; response includes `Cache-Control: no-store` headers.
```json
{"bindKey": "a1b2c3d4e5f67890..."}
```
**Security note:** The bind key is deliberately excluded from `/getActualSettingsAsJson` and is only accessible via this dedicated endpoint.

### Save Multiple Preferences (POST)
```
POST /savePreferences
Content-Type: application/json

{"MeasurementInterval": 30, "CO2OrangeRange": 800, "CO2RedRange": 1200}
```
Returns `200 OK` on success.

BTHome sensor selection is accepted as a partial JSON object `{"bthomeSensors": {"co2": true, "pm25": false}}` — only changed keys are applied.

### Save Thresholds (POST)
```
POST /saveThresholds
Content-Type: application/json

{
  "display": {"co2": 50, "temp": 2.0, "hum": 5.0},
  "mqtt": {"co2": 100, "temp": 3.0, "hum": 10.0},
  "ble": {"co2": 50, "temp": 2.0, "hum": 5.0},
  "espnow": {"co2": 100, "temp": 3.0, "hum": 10.0}
}
```

### Get Thresholds (GET)
```
GET /getThresholdsAsJson
```
Returns JSON with current threshold configuration for all output types.

---

## Calibration

| Endpoint | Description |
|---|---|
| `GET /getCalibrationStatus` | Returns JSON: `state` (idle/warming/ready/completed), `progress` (0–100%), `message` |
| `GET /settings?CalibrateCO2=400` | Start FRC at 400 PPM. Valid range: 400–2000. |

### Calibration states:
- `idle` — No calibration in progress
- `warming` — Sensor warm-up in progress (datasheet-compliant)
- `ready` — Warm-up complete, ready for calibration
- `completed` — Calibration applied successfully

Calibration warm-up progress is preserved across deep sleep cycles.

---

## Circular Buffer / Charts

| Endpoint | Description |
|---|---|
| `GET /circularBufferData` | Returns JSON array of historical readings: `[{"t": timestamp, "c": co2, "T": temp, "h": hum}, ...]` |
| `GET /getCircularBufferConfig` | Returns JSON: `capacity`, `interval`, `size` |
| `POST /updateCircularBufferConfig` | Update buffer settings. Body: `{"capacity": 1440, "interval": 60}` |

---

## Screenshot (TFT only)

| Endpoint | Description |
|---|---|
| `GET /screenshot` | Returns PNG image of current TFT display content |
| `POST /api/screenshot/start` | Initiate screenshot capture |
| `GET /api/screenshot/status` | Returns JSON with progress (0–100%) |
| `POST /api/screenshot/reset` | Cancel screenshot capture |
| `GET /screenshot.bmp` | Legacy BMP format (deprecated) |

**Note:** `readPixel()` on TDISPLAY_S3 (8-bit parallel) produces ~290 colors — a hardware limitation.

---

## Operational Commands (GET)

| Endpoint | Description |
|---|---|
| `/restart` | Reboot the ESP32 |
| `/goLowPower` | Immediately enter deep sleep |
| `/setPreferencesValue?key=val` | Set a single NVS preference (low-level) |

---

## Web Pages

These are HTML pages served from SPIFFS (gzipped, with iOS fallback):

| URL | Description |
|---|---|
| `/` or `/index.html` | Real-time CO₂/temperature/humidity dashboard |
| `/preferences.html` | Full device settings (gear icon in navbar) |
| `/status.html` | System status with colored feature badges |
| `/calibration.html` | Live CO₂ readings and FRC calibration form |
| `/charts.html` | Historical data with interactive Chart.js graphs, CSV/JSON export |
| `/low_power.html` | Deep sleep threshold configuration (4-tab interface) |
| `/ota.html` | OTA firmware upload (linked from Preferences → Maintenance) |

All pages support dark/light mode (auto-detected + manual toggle, persisted in `localStorage`).

---

## Web Assets (Static Files)

| URL | Content-Type | Description |
|---|---|---|
| `/index.min.js` | `application/javascript` | Home page logic |
| `/preferences.min.js` | `application/javascript` | Preferences logic |
| `/status.min.js` | `application/javascript` | Status page logic |
| `/calibration.min.js` | `application/javascript` | Calibration logic |
| `/charts.min.js` | `application/javascript` | Charts page logic |
| `/low_power.min.js` | `application/javascript` | Low power page logic |
| `/ota.min.js` | `application/javascript` | OTA upload logic |
| `/style.css` | `text/css` | Apple-inspired design system |
| `/themes.js` | `application/javascript` | Dark/light mode handler |
| `/common.js` | `application/javascript` | Shared utilities |
| `/favicon.ico` | `image/png` | Browser tab icon |

All JS/CSS files use cache-busting query parameters (`?v=YYYYMMDD`).
