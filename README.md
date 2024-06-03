[![PlatformIO](https://github.com/melkati/CO2-Gadget/workflows/PlatformIO/badge.svg)](https://github.com/melkati/CO2-Gadget/actions/)
[![Telegram Group](https://img.shields.io/endpoint?color=neon&style=flat-square&url=https%3A%2F%2Ftg.sumanjay.workers.dev%2Femariete_chat)](https://t.me/emariete_chat)
![Twitter Follow](https://img.shields.io/twitter/follow/e_mariete?style=social)
[![GitHub stars](https://img.shields.io/github/stars/melkati/CO2-Gadget.svg?style=social&label=Star)](https://github.com/melkati/CO2-Gadget/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/melkati/CO2-Gadget.svg?style=social&label=Fork)](https://github.com/melkati/CO2-Gadget/network)

<table align="center">
	<tr>
		<td>
			Don't forget to star ⭐ this repository NOW!
		</td>
	</tr>
</table>

<table align="center">
	<tr>
		<td>
			<a href="https://github.com/melkati/CO2-Gadget/issues/198">🌟 Calling All Tech Enthusiasts! Join the CO2 Gadget Testing Adventure 🌟</a>
		</td>
	</tr>
</table>


# CO2-Gadget

**CO2-Gadget** is an advanced firmware for CO2 Monitors/Meters based on ESP32. It's designed for flexibility, supporting a variety of popular sensors, displays, and communication protocols. With features like real-time visualization, data logging, and over-the-air updates, CO2-Gadget is perfect for both developers and end-users looking to monitor air quality effectively.

It's really flexible, you can use this firmware with **any supported CO2 Monitor/Meter** based on ESP32 (99% of them).

With cell phone App for real time visualization and charting of air quality data, datalogger, a variety of communication options (BLE, WIFI, MQTT, ESP-NOW) and many supported popular sensors.

This repository is mainly addressed at developers. If you are an end user willing to install and use the CO2 Gadget firmware, you will find complete instructions at [my blog](https://emariete.com/en/co2-meter-gadget/) including instructions in how to install the firmware very easily, with just two clicks in your browser (without having edit files, compile or install anything on your computer).

If you don't have a CO2 Monitor you will also find some complete tutorials to build your own.

<p align="center">
  <img src="https://github.com/melkati/CO2-Gadget/assets/11509521/8a81094c-5814-4246-955f-c48ef3087033">
</p>

# Features

- Many popular CO2 sensors supported: Sensirion SCD30, Sensirion SCD4x (SCD40 and SCD41), Senseair S8 LP, Winsen MH-Z19 (A/B/C/D/E), Cubic CM1106 & CM1106SL-NS (ultra low power)
- Support for popular color and monochrome displays
- Fully functional without display (display not needed)
- Supports the Air Quality App Sensirion MyAmbiance for iOS and Android with real time visualization, charting and access to historycal data
- Real time visualization on display, serial port and web page
- Management and configuration via on screen menu, internal web page and console (USB/serial port)
- Local data logger with upload to phone by BLE
- CO2 Sensor calibration via via on screen menu, internal web page, console (USB/serial port), Bluetooth or MQTT
- WIFI connection
- Easy configuration of WIFI from your phone via Bluetooth or from the web page (Captive Portal)
- Sending of data via MQTT (measurement, battery, alarms, etc)
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

<p align="center">
	<img src="https://github.com/melkati/CO2-Gadget/assets/11509521/6d577770-b83c-4f3e-9c87-a00ac28ae72c">
</p>

For latest information on other hardware use (boards, sensors, displays, etc), please check options and GPIO to use at [my blog CO2 Gadget firmware page](https://emariete.com/en/co2-meter-gadget/)

## TFT Displays

CO2 Gadget right now has support for a lot of different TFT displays. There are precompiled versions for TFT 240x135 and 320x170 pixels displays (included in ready made all in one boards as TTGO T-Display, T-Display S3 and others).

<p align="center">
	<img src="https://github.com/melkati/CO2-Gadget/assets/11509521/073d5f17-1e77-4692-833b-2cf2b838d72a">
</p>

## E-INK Displays

You can use many different E-INK displays. There are precompiled versions for most popular E-INK displays and all in one boards with E-INK displays as TTGO T5. Check support for your e-ink display at [CO2 Gadget installation page](https://emariete.com/en/co2-meter-gadget/#Instalacion_de_CO2_Gadget_Advanced)

<p align="center">
	<img src="https://github.com/melkati/CO2-Gadget/assets/11509521/643c08ae-6a17-427b-be10-0d11e8b75ca6">
</p>

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

# Installing

Installing the CO2 Gadget advanced firmware on the board with the ESP32is super easy. You don't have to download or install anything on your PC.

Just click the button corresponding to the version you want to install, select the port where your board is connected and click "Connect". It's as simple as that, the uploading into the ESP32 will be done in a few seconds without any further complication with only a few clicks...

Visit the [CO2 Gadget page](https://emariete.com/en/meter-co2-gadget/) to install from the comfort of your internet browser.


# Building from Repository

## Using PlatformIO (Recommended)

If you're looking to compile and install CO2 Gadget from its repository, PlatformIO is the preferred method. However, if you only aim to install CO2 Gadget onto your board, you can do so directly from [here](https://emariete.com/en/meter-co2-gadget/) without compiling the firmware.

### Installation Steps

You have two options for utilizing PlatformIO: either through its Command Line Interface (CLI) or via Visual Studio Code (VSCode) with PlatformIO's Graphical User Interface (GUI). Detailed installation instructions can be found [here](https://docs.platformio.org/en/latest/integration/ide/vscode.html).

### Preparing PlatformIO

Before proceeding, make sure to configure your preferences by editing the `platformio.ini` file, which is well-documented for easy customization. If required, adjust the upload and monitoring port configurations to match your setup.

```ini
upload_speed = 921600
monitor_speed = 115200
upload_port = COM13
monitor_port = COM13
```

Save the changes made to the `platformio.ini` file.

#### Compiling and Installation Process

For ease of use, PlatformIO is recommended over the Arduino IDE. After installing PlatformIO and its command line tools on your system (compatible with Windows, MacOS, and Linux), connect your compatible board via USB and execute the following command:

```bash
pio run -e TTGO_TDISPLAY --target upload
```

Ensure to replace "TTGO_TDISPLAY" with the desired flavor of CO2 Gadget to be compiled and uploaded. You can find these flavors defined in the `platformio.ini` file or define your own.

If using the PlatformIO GUI, follow these steps to compile and upload CO2-Gadget onto your board: press the "Alien head" icon, navigate to Project tasks, select your desired flavor, and choose Upload and Monitor.

## Using Arduino

**Note:**
At present, Arduino IDE is not supported. Should you wish to compile using the Arduino IDE, you will need to address includes, dependencies, and defines independently.

It's recommended to utilize VS Code with PlatformIO instead. Numerous tutorials are available online, simplifying the process considerably.
# Getting Involved

Everyone is welcome to contribute to CO2 Gadget, regardless of their skill level or background. Whether you're tech-savvy, passionate about community development, or simply want to make a difference, there’s a role for you.

Contribute: See our Issues section to find current tasks or share your project ideas. Join us in creating the world's more advanced firmware for CO2 Monitoring.

If you want to contribute to the code or documentation, consider posting a bug report, feature request or a pull request.

[![GitHub stars](https://img.shields.io/github/stars/melkati/CO2-Gadget.svg?style=social&label=Star)](https://github.com/melkati/CO2-Gadget/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/melkati/CO2-Gadget.svg?style=social&label=Fork)](https://github.com/melkati/CO2-Gadget/network)

When creating a pull request, we recommend that you do the following:

- Clone the repository
- Create a new branch for your fix or feature. For example, git checkout -b fix/my-fix or git checkout -b feat/my-feature.
- Run to any clang formatter if it is a code, for example using the `vscode` formatter. We are using Google style. More info [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- Document the PR description and code is a must
- Target your pull request to be merged with the `development` branch

There are also private groups for committed and dedicated Alpha and Beta Testers. Read more about it [here](https://github.com/melkati/CO2-Gadget/issues/198) 🌟 We need testers! 🌟

# Supporting the project

Do you want to support the CO2-Gadget project? Thank you! While financial contributions are not requested, material donations are highly appreciated to aid further development. You can also help by spreading the word about CO2-Gadget and sharing your experiences with others.

<a href=https://s.click.aliexpress.com/e/_DmqesWN target="_blank"><img src=//ae01.alicdn.com/kf/Sd1a317e7b72f425b92032c780d79d89a2.jpg_140x140.jpg /></a>
<a href=https://s.click.aliexpress.com/e/_DmJQ1VX target="_blank"><img src=//ae01.alicdn.com/kf/H308d07827d0e44d9a9609f47092b8885V.jpg_140x140.jpg /></a>

# TO DO

- [ ] [Implement low power functionality](https://github.com/melkati/CO2-Gadget/issues/188)
- [ ] [Implement full support for PM 2.5](https://github.com/melkati/CO2-Gadget/issues/170)

# Useful information

- [Everything about the Sensirion SCD30 NDIR dual-channel CO2 sensor](https://emariete.com/en/sensor-co2-ndir-sensirion-scd30-dual-channel/)
- [Everything about the Sensirion SCD40 and the SCD41 RPAS CO2 sensors](https://emariete.com/en/sensor-co2-sensirion-scd40-scd41/)
- [Everything about the Winsen MH-Z19 NDIR CO2 sensors](https://emariete.com/en/sensor-co2-mh-z19b/)
- [Everything about the Winsen Low Consumption CO2 Sensor MH-Z1311A](https://emariete.com/en/sensor-co2-low-consumption-mh-z1311a-winsen/)
- [MH-Z19B vs Senseair S8 NDIR sensors](https://emariete.com/en/comparative-co2-sensors-mh-z19b-vs-senseair-s8/)
- [Tutorial to build a CO2 Monitor with a TTGO T-Display board](https://emariete.com/en/co2-meter-co2-display-tft-colour-ttgo-t-display-sensirion-scd30/)

# Credits

Thanks to all collaborators, contributors and to the [eMariete](https://emariete.com) community for testing and reports.

# Special thanks to:


All the [contributors that helped improving CO2 Gadget](https://github.com/melkati/CO2-Gadget/graphs/contributors). You are awesome!

<a href="https://github.com/melkati/CO2-Gadget/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=melkati/CO2-Gadget" />
</a>

---
## License


    ▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
    ▐       ____ ___ ____     ____           _            _        ▌
    ▐      / ___/ _ \___ \   / ___| __ _  __| | __ _  ___| |_      ▌
    ▐     | |  | | | |__) | | |  _ / _` |/ _` |/ _` |/ _ \ __|     ▌
    ▐     | |__| |_| / __/  | |_| | (_| | (_| | (_| |  __/ |_      ▌
    ▐      \____\___/_____|  \____|\__,_|\__,_|\__, |\___|\__|     ▌
    ▐                                          |___/               ▌
    ▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
    
    Copyright (C) 2021-2024 Mariete & CO2 Gadget Contributors
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

    ▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
    ▐                                _      _                                 ▌
    ▐       ___ _ __ ___   __ _ _ __(_) ___| |_ ___   ___ ___  _ __ ___       ▌
    ▐      / _ \ '_ ` _ \ / _` | '__| |/ _ \ __/ _ \ / __/ _ \| '_ ` _ \      ▌
    ▐     |  __/ | | | | | (_| | |  | |  __/ ||  __/| (_| (_) | | | | | |     ▌
    ▐      \___|_| |_| |_|\__,_|_|  |_|\___|\__\___(_)___\___/|_| |_| |_|     ▌
    ▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
