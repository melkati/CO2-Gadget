[![PlatformIO](https://github.com/melkati/CO2-Gadget/workflows/PlatformIO/badge.svg)](https://github.com/melkati/CO2-Gadget/actions/)
[![Telegram Group](https://img.shields.io/endpoint?color=neon&style=flat-square&url=https%3A%2F%2Ftg.sumanjay.workers.dev%2Femariete_chat)](https://t.me/emariete_chat)
![Twitter Follow](https://img.shields.io/twitter/follow/e_mariete?style=social)

<table>
	<tr>
		<td>
			Don't forget to star ⭐ this repository
		</td>
	</tr>
</table>

# CO2-Gadget

An advanced fimware for CO2 Monitor/Meter. It's really flexible, you can use this firmware with **any supported CO2 Monitor/Meter** based on ESP32 (99,99% of them).

With cell phone App for real time visualization and charting of air quality data, datalogger, a variety of communication options (BLE, WIFI, MQTT, ESP-NOW) and many supported popular sensors.

This repository is mainly addressed at developers. If you are an end user willing to install and use the CO2 Gadget firmware, you will find complete instructions at [my blog](https://emariete.com/en/co2-meter-gadget/) including instructions in how to install the firmware very easily, with just two clicks in your browser (without having edit files, compile or install anything on your computer).

If you don't have a CO2 Monitor you will also find some complete tutorials to build your own.

![CO2_Gadget_DIY_CO2_Monitor](https://github.com/melkati/CO2-Gadget/assets/11509521/58e1f306-af46-416f-a399-5900965e8c10)

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
- Easy installing via web browser
- Easy WiFi setup via web browser on install and anytime (supports Improv-WiFi)
- Easy WiFi setup via bluetooth with the MyAmbiance App in iOS and Android
- ESP-NOW communications protocol from Espressif for long range and low power consuption ([more info here](https://emariete.com/en/gateway-esp-now-mqtt/))
- Over the air updates OTA
- Support for buzzer alarms on CO2 level
- Support for Neopixel (WS2812B) addressable LEDs (RGB, GBR and RGBW)
- Support for RGB LEDs
- GPIO outputs for alarms and activation of air circulation on CO2 concentration threshold with hysteresis. Check GPIO to use at [my blog CO2 Gadget firmware page](https://emariete.com/en/co2-meter-gadget/)

# Supported hardware and build

This project support a large selection of ESP32 boards, displays and sensors.

As an example you can find a very detailed tutorial with step-by-step video on how to build a very compact CO2 Gadget with a TTGO T-Display board and a high quality Sensirion SCD30 dual channel NDIR CO2 sensor (and battery support) [here](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/).

![image](https://user-images.githubusercontent.com/11509521/146636210-ee11a49a-5ebc-4e3c-a11e-91e2d8676410.png)

For latest information on other hardware use (boards, sensors, displays, etc), please check options and GPIO to use at [my blog CO2 Gadget firmware page](https://emariete.com/en/co2-meter-gadget/)

## OLED Displays

CO2 Gadget right now has support for many different OLED displays. There are precompiled versions for OLED I2C 1.3" 128x64 pixels display.
![CO2 Gadget OLED MH-Z1311A](https://user-images.githubusercontent.com/11509521/154486542-703653f0-ba0c-4bca-9616-ee5c35d4d19c.jpg)

## ESP32 Boards

Supporting any other ESP32 board is very easy. Yoy just have to setup the pines accordly.

These are the GPIOs used by each predefined board:

| Flavor | Display | RX/TX | I2C SDA/SCL | UP/DWN  | GPIO Orange  | GPIO Red | GPIO Battery | GPIO Neopixel | GPIO Buzzer
|:-----------------------|:----------------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|
| TTGO_TDISPLAY	TFT      | TFT 240×135      | 13/12   | 21/22 | 35/0 | 32 | 33 | 34 | 26 | 2
| TTGO_TDISPLAY_SANDWICH | TFT 240×135      | 13/12   | 22/21 | 35/0 | 32 | 33 | 34 | 26 | 2
| TDISPLAY_S3            | TFT 320x170      | 18/17   | 43/44 | 14/0 | 03 | 01 | 04 | 16 | 2
| esp32dev_OLED	SSH1106  | SSH1106 128×64   | 17/16   | 21/22 | 15/0 | 32 | 33 | 34 | 26 | 2
| esp32dev               | No display       | 17/16	  | 21/22 | 15/0 | 32 | 33 | 34 | 26 | 2
| esp32dev-sandwich      | No display       | 17/16	  | 22/21 | 15/0 | 32 | 33 | 34 | 26 | 2
| esp32dev-ST7789_240x320 | ST7789_240x320 | 17/16	  | 21/22 | 19/0 | 32 | 33 | 34 | 26 | 2

- Flavour: Name of the firmware variant.
- Display: Display supported by each flavour.
- RX / TX: Pins (GPIO) used for sensors connected by serial port.
- I2C: Pins (GPIO) corresponding to the I2C bus for connection of I2C sensors and displays.
- UP / DWN: Pins (GPIO) to which to connect the "Up" and "Down" buttons. They are optional as CO2 Gadget is fully functional with no buttons attached.
- GPIO Orange: Pin (GPIO) corresponding to the output when the orange level is reached (for relays, alarms, and RGB LED).
- GPIO Red: Pin (GPIO) corresponding to the output when the orange level is reached (for relays, alarms, and RGB LED).
- GPIO Battery: Pin for battery voltage measurement.
- Neopixel GPIO: Pin to which you must connect the data line of the Neopixel (WS2812B) LEDs.
- Buzzer: Pin to connect a passive buzzer for CO2 level sound alarms (built in transistor recommended).

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
| IKEA Vindriktning   | Yes | --- | Select | STABLE
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
| BMP280      | i2c |  Auto | STABLE |
| BME680      | i2c |  Auto | STABLE |
| DfRobot SEN0469 NH3 | i2c |  Auto   | TESTING |
| DFRobot SEN0466 CO  | i2c |  Auto   | TESTING |
| Geiger CAJOE        | i2c |  Select | TESTING |
| DHTxx       | TwoWire |  Auto | DEPRECATED |

NOTE: DHT22 is supported but is not recommended

Full details on CanAirIO sensorlib [here](https://github.com/kike-canaries/canairio_sensorlib/)


# Building from repository

## With PlatformIO (recommended)

**Note:** If all you want is to install CO2 Gadget into your board go [here:](https://emariete.com/en/meter-co2-gadget/). You can install it from your web browser and don't need to compile the firmware.

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
You must replace "TTGO_TDISPLAY_SANDWICH" with the flavour of CO2 Gadget you want compiled and uploaded (they are defined in platformio.ini or you can define your own).

If using PlatformIO **GUI**, to compile and upload CO2-Gadget into your board, press the "Alien head" -> Project tasks -> Choose flavour -> Upload and Monitor .

## With Arduino

**NOTE:**
Currently Arduino IDE is not supported. If you want to compile with the Arduino IDE, you will have to solve includes, dependencies and defines yourself.

I recommend that you use VS Code with PlatformIO. You have many tutorials on the internet, and it is not as difficult at all as it seems.

# Supporting the project

If you want to contribute to the code or documentation, consider posting a bug report, feature request or a pull request.

[![GitHub stars](https://img.shields.io/github/stars/melkati/CO2-Gadget.svg?style=social&label=Star)](https://github.com/melkati/CO2-Gadget/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/melkati/CO2-Gadget.svg?style=social&label=Fork)](https://github.com/melkati/CO2-Gadget/network)

When creating a pull request, we recommend that you do the following:

- Clone the repository
- Create a new branch for your fix or feature. For example, git checkout -b fix/my-fix or git checkout -b feat/my-feature.
- Run to any clang formatter if it is a code, for example using the `vscode` formatter. We are using Google style. More info [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- Document the PR description and code is a must
- Target your pull request to be merged with the `development` branch

# TODO

- [ ] Implement full support for PM 2.5

# Useful information

- [Everything about the Sensirion SCD30 NDIR dual-channel CO2 sensor](https://emariete.com/en/sensirion-scd30-co2-sensor-ndir-dual-channel/)
- [Everything about the Sensirion SCD40 and the SCD41 RPAS CO2 sensors](https://emariete.com/en/sensor-co2-sensirion-scd40-scd41-2/)
- [Everything about the Winsen MH-Z19 NDIR CO2 sensors](https://emariete.com/en/sensor-co2-mh-z19b/)
- [Everything about the Winsen Low Consumption CO2 Sensor MH-Z1311A](https://emariete.com/en/sensor-co2-low-consumption-mh-z1311a-winsen/)
- [MH-Z19B vs Senseair S8 NDIR sensors](https://emariete.com/en/comparison-co2-sensors-mh-z19b-vs-senseair-s8/)
- [Tutorial to build a CO2 Monitor with a TTGO T-Display board](https://emariete.com/en/co2-meter-co2-display-tft-colour-ttgo-t-display-sensirion-scd30/)

# Credits

Thanks to all collaborators, contributors and to the [eMariete](https://emariete.com) community for testing and reports.

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
