# Hardware Pinout Reference

> GPIO pin mapping for all supported CO2-Gadget board flavors.
> **Audience:** Developers and advanced users building or modifying hardware.

---

## Pin Mapping Table

| Flavor | Chip | Display | RX/TX | I2C SDA/SCL | UP/DWN | GPIO Orange | GPIO Red | Battery | Buzzer | Neopixel |
|---|---|---|---|---|---|---|---|---|---|---|
| **TTGO_TDISPLAY** | ESP32 | TFT 240×135 (ST7789) | 13/12 | 21/22 | 35/0 | 32 | 33 | 34 | 2 | 26 |
| **TTGO_TDISPLAY_SANDWICH** | ESP32 | TFT 240×135 (ST7789) | 13/12 | *22/21* | 35/0 | 32 | 33 | 34 | 2 | 26 |
| **TDISPLAY_S3** | ESP32-S3 | TFT 320×170 (ST7789) | 18/17 | 43/44 | 14/0 | 3 | 1 | 4 | 2 | 16 |
| **esp32dev** | ESP32 | None | 17/16 | 21/22 | 15/0 | 32 | 33 | 34 | 2 | 26 |
| **esp32dev_OLED** | ESP32 | OLED 128×64 (SSH1106) | 17/16 | 21/22 | 15/0 | 32 | 33 | 34 | 2 | 26 |
| **esp32dev-ST7789_240x320** | ESP32 | TFT 240×320 (ST7789) | 17/16 | 21/22 | 19/0 | 32 | 33 | 34 | 2 | 26 |
| **ttgo-t5-EINKBOARDGDEM0213B74** | ESP32 | E-Ink 2.13″ GDEM0213B74 | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t5-EINKBOARDDEPG0213BN** | ESP32 | E-Ink 2.13″ DEPG0213BN | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t5-EINKBOARDGDEW0213M21** | ESP32 | E-Ink 2.13″ GDEW0213M21 | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t7-EINKBOARDGDEM029T94** | ESP32 | E-Ink 2.9″ GDEM029T94 | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t7-WEACT_GDEH0154D67** | ESP32 | E-Ink 1.54″ GDEH0154D67 | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t7-WEACT_DEPG0213BN** | ESP32 | E-Ink 2.13″ DEPG0213BN | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |
| **ttgo-t7-WEACT_GxEPD2_290_BS** | ESP32 | E-Ink 2.9″ GDEM029C90 | 15/14 | 21/22 | *-1/-1* | 32 | 33 | 35 | 2 | 26 |

> *Italic values indicate differences from the default TTGO_TDISPLAY layout.*

---

## Pin Descriptions

| Pin Group | Function |
|---|---|
| **RX/TX** | UART pins for serial CO₂ sensors (MH-Z19, CM1106, Senseair S8). Connect sensor TX→board RX and sensor RX→board TX. |
| **I2C SDA/SCL** | I²C bus for auto-detected sensors (SCD30, SCD40/41, BME280, SHT31, etc.) and OLED displays (SSH1106, SSD1306). |
| **UP/DWN** | Push button pins for on-screen menu navigation (`-1` = no button, fully functional without buttons). |
| **GPIO Orange/Red** | Configurable output pins for relays, alarms, or RGB LEDs. Triggered at configurable CO₂ threshold levels. |
| **Battery** | ADC input for battery voltage measurement. **Warning:** Most ESP32 ADC pins accept max 1 V or 3.3 V — a resistive voltage divider is required. |
| **Buzzer** | Passive piezo buzzer pin. A built-in transistor driver is recommended. |
| **Neopixel** | Data pin for WS2812B (NeoPixel) addressable LED strips. |

---

## Notes

### TTGO_TDISPLAY_SANDWICH
- I2C pins are **swapped** (SDA=22, SCL=21) compared to standard TTGO_TDISPLAY.
- Used in the "sandwich" compact enclosure.

### TDISPLAY_S3
- Chip is ESP32-S3 (not ESP32). Requires ESP32-S3 toolchain.
- Backlight controlled via DW8904 IC (not PWM). Uses EN pin hold-low (>3 ms) protocol for complete shutdown.
- USB CDC serial (no hardware UART bridge).
- PSRAM available.

### E-Ink Boards (ttgo-t5-*, ttgo-t7-*)
- Button pins set to `-1` (no buttons) — E-Ink boards typically don't have user buttons.
- Battery ADC on GPIO **35** (not GPIO 34 like TFT boards).
- Uses GxEPD2 library variant for each panel model.

### For boards not listed
Check the `platformio.ini` file for the specific firmware version you are using, or open an issue on GitHub.
