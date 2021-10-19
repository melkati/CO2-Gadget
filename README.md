# CO2-Gadget

An easy to build CO2 Monitor/Meter with a variety of communication options (BLE, WIFI, MQTT, ESP-Now) and many supported sensors.

This repository is mainly addressed to developers. If you are an end user willing to build the CO2 Gadget you will find complete instructions at [my blog](https://emariete.com/en/meter-co2-display-tft-color-ttgo-t-display-sensirion-scd30-2/) including instructions to build the hardware and load the firmware very easily, with one click in your browser (without having edit files, compile or install anything on your computer).

# Disclaimer

I made this repository public not because it's ready for prime time but because some persons will be helping on the first version. The intent is not of presenting it as the final 1.0 version. Anyway, if you want to start contributing right now you will be very welcome.

I apologize in advance because right now there are almost no documentation. I'll work on it while we get to 1.0 version so I hope it will be finish in few weeks. You will be more than welcome if willing to contribute.

# Features (WIP)

- Supports the Air Quality App Sensirion MyAmbiance for iOS and Android
- Management and configuration via on screen menu, serial port and web page
- Local data logger with upload to phone by BLE
- WIFI connection
- Sending of data via MQTT
- Receiving remote commands via MQTT
- Receiving remote commands via MQTT
- Over the air updates
- Many popular sensors supported

# Supported hardware and build

This project support a large selection of boards, displays and sensors.

At the moment you can find a very detailed tutorial with step-by-step video on how to build the CO2 Gadget with a TTGO T-Display board and a high quality Sensirion SCD30 dual channel NDIR CO2 sensor.

Supporting any other ESP32 board is very esasy. Also the are a large number of SPI TFT displays already supported.

# Supported sensors

This project uses the CanAirIO sensorlib library to support as many sensors as possible. Thanks to @hpsaturn and CanAirIO team.

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
| MHZ19      | Yes | --- | Select | STABLE |
| CM1106    | Yes | --- | Select | STABLE |
| SenseAir S8 | Yes | --- | Select | STABLE |
| Sensirion SCD4x    | --- | Yes | Auto | TESTING |


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

### PlatformIO (recommended)

#### Compiling and Installing

We recommended PlatformIO because is more easy than Arduino IDE. For this, please install first [PlatformIO](http://platformio.org/) and its command line tools (Windows, MacOs and Linux), **pio** command, then connect your compatible board to the USB and run the next command:

```python
pio run --target upload
```

### Arduino

#### Prerequisites

For run the examples, you first need to  install **arduino-cli** or the **Arduino IDE** with the libraries referenced in **lib_deps** on the file [platformio.ini](https://github.com/kike-canaries/canairio_sensorlib/blob/fix_ondataready_cb/platformio.ini), becuase **Arduino don't install it automatically** like PlatformIO. Then put CanAirIO sensor library in your library directory, you can download it from [releases](https://github.com/kike-canaries/canairio_sensorlib/releases) section.

Also, you need to add the **alternative links** for supporting the ESP32 boards:

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

From `arduino-cli` you can run the CO2 Gadget in an ESP32 board following these steps:

```javascript
arduino-cli core update-index
arduino-cli core install esp32:esp32:esp32
arduino-cli compile --fqbn esp32:esp32:esp32 CO2_Gadget.ino
arduino-cli upload --fqbn esp32:esp32:esp32:UploadSpeed=115200 -p /dev/ttyUSB0 basic
```

# Supporting the project

If you want to contribute to the code or documentation, consider posting a bug report, feature request or a pull request.

When creating a pull request, we recommend that you do the following:

- Clone the repository
- Create a new branch for your fix or feature. For example, git checkout -b fix/my-fix or git checkout -b feat/my-feature.
- Run to any clang formatter if it is a code, for example using the `vscode` formatter. We are using Google style. More info [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- Document the PR description or code will be great
- Target your pull request to be merged with `development` branch

# TODO

- [ ] Test and enable WIFI feature
- [ ] Test and enable MQTT feature
- [ ] Test and enable ESP-Now feature
- [ ] Test and enable OLED Display support
- [ ] Implement full support for PM
- [ ] Enable use of features without recompiling 

# Credits

Thanks to all collaborators and [eMariete](https://emariete.com) community for testing and reports.

---
