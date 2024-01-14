[![PlatformIO](https://github.com/melkati/CO2-Gadget/workflows/PlatformIO/badge.svg)](https://github.com/melkati/CO2-Gadget/actions/)
[![Telegram Group](https://img.shields.io/endpoint?color=neon&style=flat-square&url=https%3A%2F%2Ftg.sumanjay.workers.dev%2Femariete_chat)](https://t.me/emariete_chat)
![Twitter Follow](https://img.shields.io/twitter/follow/e_mariete?style=social)

# CO2-Gadget

An easy to build CO2 Monitor/Meter with cell phone App for real time visualization and charting of air quality data, datalogger, a variety of communication options (BLE, WIFI, MQTT, ESP-NOW) and many supported popular sensors.

This repository is mainly addressed at developers. If you are an end user willing to build the CO2 Gadget you will find complete instructions at [my blog](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/) including instructions in how to build the hardware and load the firmware very easily, with just two clicks in your browser (without having edit files, compile or install anything on your computer).

<p align="center">
<img width="810" alt="Medidor-de-CO2-Display-TFT-Color-con-TTGO-T-Display-Sensirion-SCD30-Bluetooth-LE-2-ENG" src="https://user-images.githubusercontent.com/11509521/153776844-bd79fdda-4fb0-41c5-b816-5bfd2c443ccc.png">
</p> 

# Features

- Many popular CO2 sensors supported: Sensirion SCD30, Sensirion SCD4x (SCD40 and SCD41), Senseair S8 LP, Winsen MH-Z19 (A/B/C/D/E), Cubic CM1106
- Support for popular color and monochrome displays
- Fully functional without display (display not needed)
- Supports the Air Quality App Sensirion MyAmbiance for iOS and Android with real time visualization, charting and access to historycal data
- Real time visualization on display, serial port and web page
- Management and configuration via on screen menu, internal web page and console (USB/serial port)
- Local data logger with upload to phone by BLE
- WIFI connection
- Sending of data via MQTT
- Receiving remote commands via MQTT
- MQTT Discovery protocol for Home Assistant (and others supporting it as HomeSeer with mcsMQTT)
- ESP-NOW communications protocol from Espressif for long range and low power consuption ([more info here](https://emariete.com/en/gateway-esp-now-mqtt/))
- Over the air updates OTA
- Support for Neopixel (WS2812B) addressable LEDs (RGB, GBR and RGBW)
- Support for RGB LEDs
- GPIO outputs to, for example, activation of air circulation on CO2 concentration threshold with hysteresis. Check GPIO to use at [my blog CO2 Gadget firmware page](https://emariete.com/medidor-co2-gadget/)
- ~~-LoRa/LoRaWAN in study. If you are interested, please [join this conversation](https://github.com/melkati/CO2-Gadget/issues/35).~~

# Supported hardware and build

This project support a large selection of ESP32 boards, displays and sensors.

As an example you can find a very detailed tutorial with step-by-step video on how to build a very compact CO2 Gadget with a TTGO T-Display board and a high quality Sensirion SCD30 dual channel NDIR CO2 sensor and support for battery [here](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/).

![image](https://user-images.githubusercontent.com/11509521/146636210-ee11a49a-5ebc-4e3c-a11e-91e2d8676410.png)

For latest information on other hardware use (boards, sensors, displays, etc), please check options and GPIO to use at [my blog CO2 Gadget firmware page](https://emariete.com/medidor-co2-gadget/)

## OLED Displays

CO2 Gadget right now has support for many different OLED displays (by using the U8g2 library). There are precompiled versions for OLED I2C 1.3" 128x64 pixels display for real time measurements.
![CO2 Gadget OLED MH-Z1311A](https://user-images.githubusercontent.com/11509521/154486542-703653f0-ba0c-4bca-9616-ee5c35d4d19c.jpg)

## ESP32 Boards

Supporting any other ESP32 board is very easy. Yoy just have to setup the pines accordly.

These are the GPIOs used by each predefined board:

| Flavor | Display | RX/TX | I2C | UP/DWN  | GPIO EN | GPIO Green | GPIO Orange  | GPIO Red | GPIO Battery | GPIO Neopixel
|:-----------------------|:----------------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|
| TTGO_TDISPLAY	TFT      | TFT 240×135      | 13/12   | 21/22 | 35/0 | 27 | 25 | 32 | 33 | 34 | 26
| TTGO_TDISPLAY_SANDWICH | TFT 240×135      | 13/12   | 22/21 | 35/0 | 27 | 25 | 32 | 33 | 34 | 26
| esp32dev_OLED	SSH1106  | SSH1106 128×64   | 17/16   | 21/22 | 15/0 | 27 | 25 | 32 | 33 | 34 | 26
| esp32dev_OLED_OTA      | SSH1106 128×64   | 17/16	  | 21/22 | 15/0 | 27 | 25 | 32 | 33 | 34 | 26
| esp32dev               | No display       | 17/16	  | 21/22 | 15/0 | 27 | 25 | 32 | 33 | 34 | 26
| esp32dev-sandwich      | No display       | 17/16	  | 22/21 | 15/0 | 27 | 25 | 32 | 33 | 34 | 26

- Flavor: Name of the firmware variant.
- Display: Display supported by each flavor.
- RX / TX: Pins (GPIO) used for sensors connected by serial port.
- I2C: Pins (GPIO) corresponding to the I2C bus for connection of I2C sensors and displays.
- UP / DWN: Pins (GPIO) to which to connect the "Up" and "Down" buttons. They are optional as CO2 Gadget is fully functional with no buttons attached.
- EN: Pin (GPIO) that supplies an ENABLE signal for switching the sensors on and off.
- Green GPIO: Pin (GPIO) corresponding to the output before reaching the orange level (for relays, alarms, and RGB LED).
- GPIO Orange: Pin (GPIO) corresponding to the output when the orange level is reached (for relays, alarms, and RGB LED).
- GPIO Red: Pin (GPIO) corresponding to the output when the orange level is reached (for relays, alarms, and RGB LED).
- GPIO Battery: Pin for battery voltage measurement.
- Neopixel GPIO: Pin to which you must connect the data line of the Neopixel (WS2812B) LEDs.

# Supported sensors

This project uses the CanAirIO sensorlib library to support as many sensors as possible out of the box. Thanks to @hpsaturn and CanAirIO team.

CanAirIO sensorlib right now supports:

### PM sensors

| Sensor model  | UART  | I2C  | Detection mode | Status |  
|:----------------------- |:-----:|:-----:|:-------:|:----------:|
| Honeywell HPMA115S0 | Yes | --- | Auto | DEPRECATED |
| Panasonic SN-GCJA5L | Yes | Yes | Auto | STABLE |
| Plantower models    | Yes | --- | Auto | STABLE |
| Nova SDS011         | Yes | --- | Auto | STABLE |
| Sensirion SPS30     | Yes | Yes | Select / Auto | STABLE |

NOTE: Panasonic via UART in ESP8266 maybe needs select in detection

### CO2 sensors

| Sensor model  | UART  | i2c  | Detection mode | Status |  
|:----------------------- |:-----:|:-----:|:-------:|:----------:|
| Sensirion SCD30    | --- | Yes | Auto | STABLE |
| Sensirion SCD4x    | --- | Yes | Auto | TESTING |
| MHZ19      | Yes | --- | Select | STABLE |
| CM1106    | Yes | --- | Select | STABLE |
| SenseAir S8 | Yes | --- | Select | STABLE |

### Environmental sensors

| Sensor model  | Protocol  | Detection mode | Status |  
|:----------------------- |:-----:|:-------:|:----------:|
| AM2320      | i2c |  Auto | STABLE |
| SHT31       | i2c |  Auto | STABLE |
| AHT10       | i2c |  Auto | STABLE |
| BME280      | i2c |  Auto | STABLE |
| BME680      | i2c |  Auto | STABLE |
| DHTxx       | TwoWire |  Auto | DEPRECATED |

NOTE: DHT22 is supported but is not recommended

Full details on CanAirIO sensorlib [here](https://github.com/kike-canaries/canairio_sensorlib/)


# Building from repository

## With PlatformIO (recommended)

**Note:** If all you want is to flash CO2 Gadget into your board go [here:](https://emariete.com/en/meter-co2-gadget/). You don't need to compile the firmware.

### Install PlatformIO

You can use PlatfomIO CLI or VSCode with PlatformIO GUI (recomended). For installation see [PlatformIO installation instructions:](https://docs.platformio.org/en/latest/integration/ide/vscode.html).
### Prepare PlatformIO

First you must edit the file platformio.ini to setup your preferences.

If necessary adjust the upload and monitoring port configuration to match your situation.

  ```
  upload_speed = 921600
  monitor_speed = 115200
  upload_port = COM13
  monitor_port = COM13
  ```
  
Save the file platformio.ini

#### Compiling and Installing

I recommend PlatformIO because it is more easy than Arduino IDE. For this, please install first [PlatformIO](http://platformio.org/) and its command line tools (Windows, MacOs and Linux), **pio** command, then connect your compatible board to the USB and run the next command:

```python
pio run pio run -e TTGO_TDISPLAY_SANDWICH --target upload
```
You must replace "TTGO_TDISPLAY_SANDWICH" with the flavor of CO2 Gadget you want compiled and uploaded (they are defined in platformio.ini or you can define your own).

If using PlatformIO **GUI**, to compile and upload CO2-Gadget into your board, press the "Alien head" -> Project tasks -> Choose flavour -> Upload and Monitor .

## With Arduino

**NOTE:**
Currently neither the code is ready to compile with the Arduino IDE nor these instructions are up to date. If you want to compile with the Arduino IDE, you will have to solve includes, dependencies and defines yourself.

When the code is more stable and changes less often, I will update the code the instructions to compile with the Arduino IDE. Right now it is modified very often with bugfixes and new functionalities and it takes me a lot of time to be aware of maintaining compatibility with the Arduino IDE.

I recommend that you use VS Code with PlatformIO. You have many tutorials on the internet, and it is not as difficult at all as it seems.

#### Prerequisites

To **compile this project with Arduino IDE** (the integrated development enviroment), you have to **install Arduino IDE**, and **add the libraries** referenced in **lib_deps** in the file [platformio.ini](https://github.com/melkati/CO2-Gadget/blob/master/platformio.ini), as **Arduino won't install it automatically** like PlatformIO does.

Also, you need to add support for ESP32 boards:

For Arduino IDE: Adding the line **https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json** in: File > Preferences > Additional Boards Manager URLs.

In Arduino IDE select your board, COM port, etc. in the Tools menu and to upload to your board select Program -> Upload or click on the "upload" icon.

To **compile this project with Arduino CLI** (the command line version of Arduino), you first need to install **arduino-cli** or the **Arduino IDE** with the libraries referenced in **lib_deps** in the file [platformio.ini](https://github.com/melkati/CO2-Gadget/blob/master/platformio.ini), because **Arduino won't install it automatically** like PlatformIO does.

You must first add support for ESP32 boards in Arduino CLI: Follow the next steps:

```bash
arduino-cli config init
```

in the `.arduino15/arduino-cli.yaml` file add:

```yml
board_manager:
  additional_urls:
    - https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
``` 

From `arduino-cli` you can load CO2 Gadget in an ESP32 board following these steps:

```javascript
arduino-cli core update-index
arduino-cli core install esp32:esp32:esp32
arduino-cli compile --fqbn esp32:esp32:esp32 CO2_Gadget.ino
arduino-cli upload --fqbn esp32:esp32:esp32:UploadSpeed=115200 -p /dev/ttyUSB0 basic
```

# Supporting the project

If you want to contribute to the code or documentation, consider posting a bug report, feature request or a pull request.

[![GitHub stars](https://img.shields.io/github/stars/melkati/CO2-Gadget.svg?style=social&label=Star)](https://github.com/melkati/CO2-Gadget/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/melkati/CO2-Gadget.svg?style=social&label=Fork)](https://github.com/melkati/CO2-Gadget/network)

When creating a pull request, we recommend that you do the following:

- Clone the repository
- Create a new branch for your fix or feature. For example, git checkout -b fix/my-fix or git checkout -b feat/my-feature.
- Run to any clang formatter if it is a code, for example using the `vscode` formatter. We are using Google style. More info [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- Document the PR description or code will be great
- Target your pull request to be merged with `development` branch

# TODO

- [ ] Implement full support for PM 2.5
- [ ] Full configuration vía web page

# Useful information

- [Everything about the Sensirion SCD30 NDIR dual-channel CO2 sensor](https://emariete.com/en/sensirion-scd30-co2-sensor-ndir-dual-channel/)
- [Everything about the Sensirion SCD40 and the SCD41 RPAS CO2 sensors](https://emariete.com/en/sensor-co2-sensirion-scd40-scd41-2/)
- [Everything about the Winsen MH-Z19 NDIR CO2 sensors](https://emariete.com/en/sensor-co2-mh-z19b/)
- [Everything about the Winsen Low Consumption CO2 Sensor MH-Z1311A](https://emariete.com/en/sensor-co2-low-consumption-mh-z1311a-winsen/)
- [MH-Z19B vs Senseair S8 NDIR sensors](https://emariete.com/en/comparison-co2-sensors-mh-z19b-vs-senseair-s8/)

# Credits

Thanks to all collaborators, contributors and [eMariete](https://emariete.com) community for testing and reports.

---
## License

    Copyright (C) 2021-2024 CO2 Gadget Contributors
    Contact: https://emariete.com

    This file is part of the CO2 Gadget firmware.

    The CO2 Gadget firmware is free software: you can
    redistribute it and/or modify it under the terms of the GNU Lesser General
    Public License as published by the Free Software Foundation, either version
    3 of the License, or (at your option) any later version.

    The emariete.com Website and Documentation is distributed in the hope
    that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the emariete.com Website and Documentation. If not, see
    <http://www.gnu.org/licenses/>.
