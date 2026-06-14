# CO2-Gadget Datasheets & Documentation

This folder contains official datasheets, manuals, and technical documentation for all hardware components supported by the CO2-Gadget project.

> **⚠️ Note:** Only components that are **actually implemented** in the code are included. For example, while the `canairio_sensorlib` library supports many PM sensors, only those used by CO2-Gadget are listed here.

---

## 📋 Table of Contents

- [ESP32 Microcontrollers](#esp32-microcontrollers)
- [Displays](#displays)
  - [TFT Displays](#tft-displays)
  - [OLED Displays](#oled-displays)
  - [E-Ink Displays](#e-ink-displays)
- [CO2 Sensors](#co2-sensors)
- [Particulate Matter (PM) Sensors](#particulate-matter-pm-sensors)
- [LEDs & Lighting](#leds--lighting)
- [Other Components](#other-components)
- [Libraries & Software](#libraries--software)
- [📄 Missing Documentation](#-missing-documentation)

---

## ESP32 Microcontrollers

| Component | Description | File |
|-----------|-------------|------|
| **ESP32** | Main ESP32 chip datasheet | [ESP32_Datasheet.pdf](ESP32/ESP32_Datasheet.pdf) |
| **ESP32-S3** | ESP32-S3 chip datasheet (used in T-Display-S3) | [ESP32-S3_Datasheet.pdf](ESP32/ESP32-S3_Datasheet.pdf) |

---

## Displays

### TFT Displays

| Component | Resolution | Controller | Manufacturer | File |
|-----------|------------|------------|--------------|------|
| **ST7789** | 240×135 / 170×320 / 240×320 | ST7789 | Sitronix | ⚠️ *Not found - please provide* |

### OLED Displays

| Component | Manufacturer | Resolution | Interface | File |
|-----------|-------------|------------|-----------|------|
| **SSD1306** | Solomon Systech | 128×64 | I2C | [SSD1306_Datasheet.pdf](Displays/SSD1306_Datasheet.pdf) |
| **SH1106** | Sinadax | 128×64 | I2C | ⚠️ *Not found - please provide* |

### E-Ink Displays

| Component | Resolution | Controller | Manufacturer | File |
|-----------|------------|------------|--------------|------|
| **DEPG0213BN** | 250×122 | SSD1608 | Good Display | ⚠️ *Not found - please provide* |
| **GDEM0213B74** | 250×122 | - | Good Display | *(See library docs)* |
| **GDEW0213M21** | 212×104 | SSD1608 | Good Display | ⚠️ *Not found - please provide* |
| **GDEM029T94** | 296×128 | SSD1680 | Good Display | ⚠️ *Not found - please provide* |
| **GDEH0154D67** | 200×200 | SSD1681 | Good Display | ⚠️ *Not found - please provide* |

---

## CO2 Sensors

| Component | Manufacturer | Type | Protocol | File |
|-----------|---------------|------|----------|------|
| **SCD30** | Sensirion | NDIR | I2C | [SCD30_Datasheet.pdf](CO2_Sensors/SCD30_Datasheet.pdf) |
| **SCD40** | Sensirion | NDIR | I2C | [SCD40_SCD41_Datasheet.pdf](CO2_Sensors/SCD40_SCD41_Datasheet.pdf) |
| **SCD41** | Sensirion | NDIR | I2C | [SCD40_SCD41_Datasheet.pdf](CO2_Sensors/SCD40_SCD41_Datasheet.pdf) |
| **MH-Z19C** | Winsen | NDIR | UART | [MH-Z19C_Datasheet.pdf](CO2_Sensors/MH-Z19C_Datasheet.pdf) |
| **S8** | SenseAir | NDIR | UART | [S8_Datasheet.pdf](CO2_Sensors/S8_Datasheet.pdf) |
| **CM1106** | SenseAir | NDIR | UART | ⚠️ *Not found - please provide* |
| **CM1106SL-NS** | SenseAir | NDIR | UART | [CM1106SL-NS_Datasheet.pdf](CO2_Sensors/CM1106SL-NS_Datasheet.pdf) |

---

## Particulate Matter (PM) Sensors

| Component | Manufacturer | Type | File |
|-----------|-------------|------|------|
| **SPS30** | Sensirion | PM Sensor | [SPS30_Datasheet.pdf](PM_Sensors/SPS30_Datasheet.pdf) |

---

## LEDs & Lighting

| Component | Manufacturer | Type | File |
|-----------|-------------|------|------|
| **WS2812B** | Worldsemi | RGB LED Strip (NeoPixel) | [WS2812B_Datasheet.pdf](Other/WS2812B_Datasheet.pdf) |

---

## Other Components

| Component | Description | File |
|-----------|-------------|------|
| **Battery Sense** | Battery voltage sensing | [BatterySense_Library.md](Other/BatterySense_Library.md) |

---

## Libraries & Software

| Library | Documentation |
|---------|---------------|
| **Button2** | [Button2_Library.md](Other/Button2_Library.md) |
| **Adafruit NeoPixel** | [NeoPixel_Library.md](Other/NeoPixel_Library.md) |
| **GxEPD2** | [GxEPD2_Library.md](Other/GxEPD2_Library.md) |
| **TFT_eSPI** | [TFT_eSPI_Library.md](Other/TFT_eSPI_Library.md) |
| **U8g2** | [U8g2_Library.md](Other/U8g2_Library.md) |
| **PubSubClient** | [PubSubClient_Library.md](Other/PubSubClient_Library.md) |
| **NimBLE-Arduino** | [NimBLE-Arduino_Library.md](Other/NimBLE-Arduino_Library.md) |
| **ElegantOTA** | [ElegantOTA_Library.md](Other/ElegantOTA_Library.md) |
| **ArduinoJson** | [ArduinoJson_Library.md](Other/ArduinoJson_Library.md) |

---

## ⚠️ Missing Documentation

The following datasheets could not be found online. If you have access to these documents, please add them to the appropriate folder:

### Displays
- **SH1106** OLED controller datasheet
- **SSD1608** e-paper controller datasheet
- **SSD1680** / **SSD1681** e-paper controller datasheets

### CO2 Sensors
- **SenseAir CM1106** datasheet

### Other
- **LilyGo T-Display-S3** board schematic
- **TTGO T7 Mini32** board schematic

### How to Contribute
If you have any of these documents, please:
1. Download or locate the official datasheet
2. Rename it appropriately (e.g., `SH1106_Datasheet.pdf`)
3. Place it in the corresponding folder
4. Submit a PR or share it so we can update this README

---

## 📂 Folder Structure

```
datasheets/
├── ESP32/
│   ├── ESP32_Datasheet.pdf
│   └── ESP32-S3_Datasheet.pdf
├── Displays/
│   ├── SSD1306_Datasheet.pdf
│   └── (SH1106, ST7789, SSD1608 - missing)
├── CO2_Sensors/
│   ├── SCD30_Datasheet.pdf
│   ├── SCD40_SCD41_Datasheet.pdf
│   ├── MH-Z19C_Datasheet.pdf
│   ├── S8_Datasheet.pdf
│   └── (CM1106, CM1106SL-NS - missing)
├── PM_Sensors/
│   └── SPS30_Datasheet.pdf
├── Battery/
│   └── (empty - battery sense is software)
├── Connectivity/
│   └── (empty - no specific hardware)
├── Other/
│   ├── WS2812B_Datasheet.pdf
│   ├── BatterySense_Library.md
│   ├── Button2_Library.md
│   ├── NeoPixel_Library.md
│   ├── GxEPD2_Library.md
│   ├── TFT_eSPI_Library.md
│   ├── U8g2_Library.md
│   ├── PubSubClient_Library.md
│   ├── NimBLE-Arduino_Library.md
│   ├── ElegantOTA_Library.md
│   └── ArduinoJson_Library.md
└── README.md (this file)
```

---

*Last updated: June 2026*
