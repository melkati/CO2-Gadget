[![PlatformIO](https://github.com/melkati/CO2-Gadget/workflows/PlatformIO/badge.svg)](https://github.com/melkati/CO2-Gadget/actions/)
[![Telegram Group](https://img.shields.io/endpoint?color=neon&style=flat-square&url=https%3A%2F%2Ftg.sumanjay.workers.dev%2Femariete_chat)](https://t.me/emariete_chat)
![Twitter Follow](https://img.shields.io/twitter/follow/e_mariete?style=social)

👉 **Please, due to a issue with memory, use [development branch](https://github.com/melkati/CO2-Gadget/tree/development) while problem gets sorted out.** It uses a [modified version of the Sensirion Gadget BLE Arduino Lib]( https://github.com/melkati/arduino-ble-gadget.git#setSampleIntervalMs)

# CO2-Gadget

An easy to build CO2 Monitor/Meter with cell phone App for real time visualization and charting of air data, datalogger, a variety of communication options (BLE, WIFI, MQTT, ESP-Now) and many supported sensors.

This repository is mainly addressed at developers. If you are an end user willing to build the CO2 Gadget you will find complete instructions at [my blog](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/) including instructions in how to build the hardware and to load the firmware very easily, with one click in your browser (without having edit files, compile or install anything on your computer).

![CO2 Gadget](https://emariete.com/wp-content/uploads/2021/09/Medidor-CO2-Sensirion-MyAmbience-eMariete.png)

# Features (WIP)

- Many popular CO2 sensors supported: Sensirion SCD30, Sensirion SCD40, Sensirion SCD41, Senseair S8 LP, MH-Z19, Cubic CM1106
- Supports the Air Quality App Sensirion MyAmbiance for iOS and Android with real time visualization, charting and access to historycal data
- Real time visualization on display, serial port and web page
- Management and configuration via on screen menu, serial port and web page
- Local data logger with upload to phone by BLE
- WIFI connection
- Sending of data via MQTT
- Receiving remote commands via MQTT
- Over the air updates OTA

# Supported hardware and build

This project support a large selection of boards, displays and sensors.

As an example you can find a very detailed tutorial with step-by-step video on how to build a very compact CO2 Gadget with a TTGO T-Display board and a high quality Sensirion SCD30 dual channel NDIR CO2 sensor and support for battery [here](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/).

Supporting any other ESP32 board is very easy. Also, the are a large number of displays already supported.

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

First you should edit some files with your own data and preferences.

Edit the credentials.h file substituting "MyWiFiSSID" with your own WiFi SSID and "some_secret_password" with your own WiFi password in:

  ```
  #define  WIFI_SSID_CREDENTIALS  "MyWiFiSSID"
  #define  WIFI_PW_CREDENTIALS    "some_secret_password"
  ```
If you prefer not to keep sensitive data in a file, you can define this data in environment variables so you don't need a credentials.h file with your secrets. See platformio.ini file for instructions.

If you are planning to use MQTT with authentication, substitute "MyUser" with your own MQTT user and "another_secret_password" with your own MQTT user password:

  ```
  #define MQTT_USER_CREDENTIAL "MyUser"
  #define MQTT_PW_CREDENTIAL "another_secret_password"
  ```
You are done with credentials. Save the file credentials.h

### PlatformIO (recommended)

Edit the file platformio.ini

If necessary adjust the upload and minitoring port configuration to match your situation.

  ```
  upload_speed = 921600
  monitor_speed = 115200
  upload_port = COM13
  monitor_port = COM13
  ```
  
If you are planning to use MQTT, edit the IP of your MQTT broker, substituting 192.168.1.145 with your broker IP address:
  
  ```
  -D MQTT_BROKER_SERVER="\"192.168.1.146"\"
  ```
Save the file platformio.ini

#### Compiling and Installing

We recommend PlatformIO because it is more easy than Arduino IDE. For this, please install first [PlatformIO](http://platformio.org/) and its command line tools (Windows, MacOs and Linux), **pio** command, then connect your compatible board to the USB and run the next command:

```python
pio run --target upload
```
If using PlatformIO GUI, to compile and upload CO2-Gadget into your board, press the"Alien head" -> Project tasks -> Upload and Monitor (in the image the sanwitch version is selected).

### Arduino

These Arduino instructions are outdated. Sou must adapt it to your situation while I write some new instructions.

#### Prerequisites

To **compile this project with Arduino IDE** (the integrated development enviroment), you have to **install Arduino IDE**, and **add the libraries** referenced in **lib_deps** in the file [platformio.ini](https://github.com/melkati/CO2-Gadget/blob/master/platformio.ini), as **Arduino won't install it automatically** like PlatformIO does.

To **compile this project with Arduino CLI** (the command line version of Arduino), you first need to install **arduino-cli** or the **Arduino IDE** with the libraries referenced in **lib_deps** in the file [platformio.ini](https://github.com/melkati/CO2-Gadget/blob/master/platformio.ini), because **Arduino won't install it automatically** like PlatformIO does.

Also, you need to add the **alternative links** for supporting the ESP32 boards:

For Arduino IDE: Adding the line **https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json** in: File > Preferences > Additional Boards Manager URLs.

For Arduino CLI: Follow the next steps:

```bash
arduino-cli config init
```

in the `.arduino15/arduino-cli.yaml` file add:

```yml
board_manager:
  additional_urls:
    - https://arduino.esp8266.com/stable/package_esp8266com_index.json
    - https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
``` 

#### Compiling and Installing

With Arduino IDE you just have to select Program > Upload

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

- [x] Test and enable WIFI feature
- [x] Test and enable MQTT feature
- [ ] Test and enable ESP-Now feature
- [ ] Test and enable OLED Display support
- [ ] Implement full support for PM
- [x] Enable enable and disable BLE, WiFi and MQTT without recompiling by activating/deactivating on the buit in menu

# Credits

Thanks to all collaborators, contributors and [eMariete](https://emariete.com) community for testing and reports.

---
## License

    Copyright (C) 2021 CO2 Gadget Contributors
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
