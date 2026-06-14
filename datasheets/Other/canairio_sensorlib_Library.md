
[![PlatformIO](https://github.com/kike-canaries/canairio_sensorlib/workflows/PlatformIO/badge.svg)](https://github.com/kike-canaries/canairio_sensorlib/actions/) [![Build Status](https://travis-ci.com/kike-canaries/canairio_sensorlib.svg?branch=master)](https://travis-ci.com/kike-canaries/canairio_sensorlib.svg?branch=master) ![ViewCount](https://views.whatilearened.today/views/github/kike-canaries/canairio_sensorlib.svg) 

# Air Quality Sensors Library

Generic sensor manager, abstractions and bindings of multiple sensors [libraries](https://github.com/kike-canaries/canairio_sensorlib/blob/master/unified-lib-deps.ini): Honeywell, Plantower, Panasonic, Sensirion, etc. and CO2 sensors. Also it's handling others environment sensors. This library is for general purpose, but also is the sensors library base of [CanAirIO project](https://canair.io/docs).

For developers also you can check the complete library documentation [here](http://hpsaturn.com/canairio_sensorlib_doc/html/classSensors.html)

<table>
	<tr>
		<td>
			Don't forget to star ⭐ this repository
		</td>
	</tr>
</table>

# Supported sensors

### PM sensors

| Sensor model  | UART  | I2C  | Detection mode | Status |  
|:----------------------- |:-----:|:-----:|:-------:|:----------:|
| Honeywell HPMA115S0 | Yes | --- | Auto | DEPRECATED |
| Panasonic SN-GCJA5L | Yes | Yes | Auto | STABLE |
| Plantower models    | Yes | --- | Auto | STABLE |
| Nova SDS011         | Yes | --- | Auto | STABLE |
| IKEA Vindriktning   | Yes | --- | Select | STABLE |
| Sensirion SPS30     | Yes | Yes | Select / Auto | STABLE |

NOTE:  
Panasonic via UART in ESP8266 maybe needs select in detection.  

### CO2 sensors

| Sensor model  | UART  | I2C | Detection mode | Status |  
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
| DfRobot SEN0469 NH3  | i2c |  Auto | TESTING |
| DFRobot SEN0466 CO | i2c |  Auto | TESTING |
| DFRobot SEN0471 NO2 | i2c |  Auto | TESTING |
| Geiger CAJOE | GPIO | Select | TESTING |
| DHTxx       | TwoWire |  Select | DISABLED |

NOTE:  
DHT22 is supported but is not recommended. Please see the documentation.  

### Platforms supported

| Platform  | Variants  | Notes | Status |  
|:----------------------- |:-----:|:-------:|:----------:|
| ESP32  | WROVER* | ESP32Devkit and similar (recommended) | STABLE  |
| ESP32S3  | LilyGo TDisplay | In testing | STABLE |
| ESP32C3  | Devkit v3 | In testing | STABLE |
| ESP8266  | 12 |  D1MINI tested and similar (old) | STABLE |
| Atmelsam  | seeed_wio_terminal | Only works via i2c on left port | STABLE |
| Arduino | Atmel  | Some third party libraries fails | IN PROGRESS |



# Features

- Unified variables and getters for all sensors 
- Auto UART port selection (Hw, Sw, UART1, UART2, etc)
- Multiple i2c sensors and one UART sensor supported at the same time
- Two I2C channel supported (Wire and Wire1)
- Real time registry of sensor units (see multivariable)
- Get vendor names of all devices detected
- Preselected main stream UART pins from popular boards
- Auto config UART port for Plantower, Honeywell and Panasonic sensors
- Unified calibration trigger for all CO2 sensors
- Unified CO2 Altitude compensation
- Unified temperature offset for CO2 and environment sensors
- Add support for Kelvin and Fahrenheit on environment and CO2 sensors
- Public access to main objects of each library (full methods access)
- Get unit symbol and name and each sub-sensor
- Get the main group type: NONE, PM, CO2 and ENV.
- Basic debug mode support toggle in execution

Full list of all sub libraries supported [here](https://github.com/kike-canaries/canairio_sensorlib/blob/master/unified-lib-deps.ini)

# Quick implementation

```Java
sensors.setOnDataCallBack(&onSensorDataOk);   // all data read callback
sensors.init();                               // start all sensors and
```

# Full implementation

You can review a full implementation on [CanAirIO project firmware](https://github.com/kike-canaries/canairio_firmware/blob/master/src/main.cpp), but a little brief is the next:

```Java
/// sensors data callback
void onSensorDataOk() {
    Serial.print("PM2.5: " + String(sensors.getPM25()));
    Serial.print(" CO2: "  + String(sensors.getCO2()));
    Serial.print(" CO2H: " + String(sensors.getCO2humi()));
    Serial.print(" CO2T: " + String(sensors.getCO2temp()));
    Serial.print(" H: "    + String(sensors.getHumidity()));
    Serial.println(" T: "  + String(sensors.getTemperature()));
}

/// sensors error callback
void onSensorDataError(const char * msg){
    Serial.println("Sensor read error: "+String(msg));
}

void setup() {

    sensors.setOnDataCallBack(&onSensorDataOk);     // all data read callback
    sensors.setOnErrorCallBack(&onSensorDataError); // [optional] error callback
    sensors.setSampleTime(15);                      // [optional] sensors sample time (default 5s)
    sensors.setTempOffset(cfg.toffset);             // [optional] temperature compensation
    sensors.setCO2AltitudeOffset(cfg.altoffset);    // [optional] CO2 altitude compensation
    sensors.setSeaLevelPressure(1036.25);           // [optional] Set sea level pressure in hpa
    sensors.setDebugMode(false);                    // [optional] debug mode to get detailed msgs
    sensors.detectI2COnly(true);                    // [optional] force to only i2c sensors
    sensors.setTemperatureUnit(TEMPUNIT::KELVIN);   // comment it for Celsius or set Fahrenheit
    sensors.init();                                 // Auto detection to UART and i2c sensors

    // Alternatives only for UART sensors (TX/RX):

    // sensors.init(SENSORS::Auto);                 // Auto detection to UART sensors (Honeywell, Plantower, Panasonic)
    // sensors.init(SENSORS::SGCJA5);               // Force UART detection to Panasonic sensor
    // sensors.init(SENSORS::SSPS30);               // Force UART detection to Sensirion sensor
    // sensors.init(SENSORS::SMHZ19);               // Force UART detection to Mhz14 or Mhz19 CO2 sensor
    // sensors.init(SENSORS::SDS011);               // Force UART detection to SDS011 sensor
    // sensors.init(SENSORS::IKEAVK);               // Force UART detection to IKEA Vindriktning sensor
    // sensors.init(SENSORS::SCM1106);              // Force UART detection to CM1106 CO2 sensor
    // sensors.init(SENSORS::SAIRS8);               // Force UART detection to SenseAirS8 CO2 sensor
    // sensors.init(SENSORS::Auto,PMS_RX,PMS_TX);   // Auto detection on custom RX,TX
  
    // Also you can access to sub-library objects, and perform for example calls like next:

    // sensors.sps30.sleep()
    // sensors.bme.readPressure();
    // sensors.mhz19.getRange();
    // sensors.scd30.getTemperatureOffset();
    // sensors.aht10.readRawData();
    // sensors.s8.set_ABC_period(period)
    // ...

    delay(500);
}

void loop() {
    sensors.loop();  // read sensor data and showed it
}
```

## Multivariable demo

In this [demo](https://www.youtube.com/watch?v=-5Va47Bap48) on two different devices with multiple sensors, you can choose the possible sub sensors units or variables:

[![CanAirIO multivariable demo](https://img.youtube.com/vi/-5Va47Bap48/0.jpg)](https://www.youtube.com/watch?v=-5Va47Bap48)

In this [demo](https://www.youtube.com/watch?v=uxlmP905-FE) on a simple sketch you could have a dinamyc list of variables of multiple sensors brands:

[![CanAirIO Sensors Lib DEMO with M5CoreInk](https://img.youtube.com/vi/i15iEF47CbY/0.jpg)](https://youtu.be/i15iEF47CbY)

## Multivariable alternative implementation

The last version added new getters to have the current status of each unit of each sensor connected to the device in real time. Also you can retrieve the list of device names and other stuff:  

For example:

```cpp
#include <Arduino.h>
#include <Sensors.hpp>

void printSensorsDetected() {
    uint16_t sensors_count =  sensors.getSensorsRegisteredCount();
    uint16_t units_count   =  sensors.getUnitsRegisteredCount();
    Serial.println("-->[MAIN] Sensors detected count\t: " + String(sensors_count));
    Serial.println("-->[MAIN] Sensors units count  \t: "  + String(units_count));
    Serial.print(  "-->[MAIN] Sensors devices names\t: ");
    int i = 0;
    while (sensors.getSensorsRegistered()[i++] != 0) {
        Serial.print(sensors.getSensorName((SENSORS)sensors.getSensorsRegistered()[i - 1]));
        Serial.print(",");
    }
    Serial.println();
}

void printSensorsValues() {
    Serial.println("\n-->[MAIN] Preview sensor values:");
    UNIT unit = sensors.getNextUnit();
    while(unit != UNIT::NUNIT) {
        String uName = sensors.getUnitName(unit);
        float uValue = sensors.getUnitValue(unit);
        String uSymb = sensors.getUnitSymbol(unit);
        Serial.print("-->[MAIN] " + uName + ": " + String(uValue) + " " + uSymb);
        unit = sensors.getNextUnit();
    }
}

void onSensorDataOk() {
    Serial.println("======= E X A M P L E   T E S T =========");
    printSensorsDetected();
    printSensorsValues(); 
    Serial.println("=========================================");
}

/******************************************************************************
*  M A I N
******************************************************************************/

void setup() {
    Serial.begin(115200);
    delay(100);
    sensors.setSampleTime(5);                       // config sensors sample time interval
    sensors.setOnDataCallBack(&onSensorDataOk);     // all data read callback
    sensors.setDebugMode(true);                     // [optional] debug mode
    sensors.detectI2COnly(false);                   // disable force to only i2c sensors
    sensors.init();                                 // Auto detection to UART and i2c sensors
}

void loop() {
    sensors.loop();  // read sensor data and showed it
}
```

## UART detection demo

[![CanAirIO auto configuration demo](https://img.youtube.com/vi/hmukAmG5Eec/0.jpg)](https://www.youtube.com/watch?v=hmukAmG5Eec)

CanAirIO sensorlib auto configuration demo on [Youtube](https://www.youtube.com/watch?v=hmukAmG5Eec)

# Wiring

The current version of library supports 3 kinds of wiring connection, UART, i2c and TwoWire, in the main boards the library using the defaults pins of each board, but in some special cases the pins are:

## UART

### Predefined UART

The library has [pre-defined some UART pin configs](https://github.com/kike-canaries/canairio_sensorlib/blob/master/src/Sensors.hpp#L19-L52), these are selected on compiling time. Maybe you don't need change anything with your board, and maybe the nexts alternatives works for you:

| Board model    |  TX   | RX  |      Notes |
|:---------------|:---:|:---:|:------------------:|
| ESP32GENERIC   | 1  | 3  | ESP32 Pio defaults
| TTGOT7 / ESP32DEVKIT / D1MINI / NODEFINED | 16 | 17 | CanAirIO devices **
| TTGO_TDISPLAY  | 12 | 13 | |
| M5COREINK      | 14 | 13 | |
| TTGO TQ        | 18 | 13 | |
| HELTEC         | 18 | 17 | |
| WEMOSOLED      | 15 | 13 | |
| ESP32PICOD4    | 3  | 1  | |
  
** This pines are when you compile your project without specific any build variable or you board isn't in the list.  

### Custom UART

Also you could define a custom UART pins in the init() method and select specific sensors model, like this:

```cpp
sensors.init(SENSORS::SDS011,yourRX,yourTX); // custom RX, custom TX pines.
```

## I2C (recommended)

We are using the default pins for each board, some times it's pins are 21,22, please check your board schematic.

## TwoWire (deprecated soon)

For now we are using it only for DHT sensors in PIN 23. For more info please review the next lines [here](https://github.com/kike-canaries/canairio_sensorlib/blob/master/src/Sensors.hpp#L19-L52).

# Examples

### PlatformIO (recommended)

We recommended PlatformIO because is more easy than Arduino IDE. For that, please install first [PlatformIO](http://platformio.org/) and its command line tools (Windows, MacOs and Linux), **pio** command, then connect your compatible board to the USB and run the next command:

```python
pio run -e esp32 --target upload
```

Also you can see some examples than have `platformio.ini` files for your project.

### Arduino IDE

Only import the `ino` file of the sample and install the libraries listed on `library.json` and this library. Complete list of libraries used [here](https://github.com/kike-canaries/canairio_sensorlib/blob/master/unified-lib-deps.ini)

### Arduino CLI

For run the examples, you first need to  install **arduino-cli** or the **Arduino IDE** with the libraries referenced in **lib_deps** on the file [platformio.ini](https://github.com/kike-canaries/canairio_sensorlib/blob/master/platformio.ini), becuase **Arduino don't install it automatically** like PlatformIO. Then put CanAirIO sensor library in your library directory, you can download it from [releases](https://github.com/kike-canaries/canairio_sensorlib/releases) section.

Also you need to add the **alternative links** for supporting the ESP32 boards:

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

From `arduino-cli` you can run the basic example in a ESP32 board following these steps:

```javascript
arduino-cli core update-index
arduino-cli core install esp32:esp32:lolin32
arduino-cli compile --fqbn esp32:esp32:lolin32 basic
arduino-cli upload --fqbn esp32:esp32:lolin32:UploadSpeed=115200 -p /dev/ttyUSB0 basic
```

where `basic` is the basic example on examples directory.

# Supporting the project

If you want to contribute to the code or documentation, consider posting a bug report, feature request or a pull request.

When creating a pull request, we recommend that you do the following:

- Clone the repository
- Create a new branch for your fix or feature. For example, git checkout -b fix/my-fix or git checkout -b feat/my-feature.
- Run to any clang formatter if it is a code, for example using the `vscode` formatter. We are using Google style. More info [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- Document the PR description or code will be great
- Target your pull request to be merged with `devel` branch

Also you can make a donation, be a patreon or buy a device:  

<a href="https://raw.githubusercontent.com/kike-canaries/canairio_firmware/master/images/ethereum_donation_address.png" target="_blank"><img src="https://raw.githubusercontent.com/kike-canaries/canairio_firmware/master/images/ethereum_donation_address.png" align="right" width="220" margin-left="10px" ></a>

- Via **Ethereum**: `0x1779cD3b85b6D8Cf1A5886B2CF5C53a0E072C108`
- Via **Liberapay**: [CanAirIO in LiberaPay](https://liberapay.com/CanAirIO)
- **Buy a device**: [CanAirIO Bike in Tindie](https://www.tindie.com/products/hpsaturn/canairio-bike/)
- [Inviting us **a coffee**](https://www.buymeacoffee.com/hpsaturn) 

# TODO

- [x] Auto detection for UART sensors (Honeywell, Panasonic and Plantower)
- [x] Added SPS30 library with auto UART detection
- [x] Disable/enable logs (debug mode flag)
- [x] Added bme280, bmp280, aht10, sht31, am2320 i2c sensors
- [x] Exposed public sub-libraries objects, sps30, aht10, etc.
- [x] Added old DHT sensors
- [x] Added CO2 sensors: MHZ19, SCD30, CM1106 via UART
- [x] Added SDS011 particle metter
- [x] BME680 support
- [x] Added Sensirion SPS30 and Panasonic SN-GCJA5 via i2c
- [x] Enable/Disable UART detection for force only i2c
- [x] Temperature and Altitude compensation
- [x] SenseAir S8 via UART support
- [x] Multivariable selection (getNextUnit(),getUnitName(),etc)
- [x] Two I2C channel supported for M5Stack Devices (M5StickC tested)
- [x] Added CO, NO2 and NH3 sensors
- [x] Added Geiger sensor support
- [ ] New IKEA VINDSTYRKA device support
- [ ] Sea level setting for Pressure sensors and others
- [ ] Support to second UART port

# Projects using this Library

- [CanAirIO Device](https://github.com/kike-canaries/canairio_firmware): ESP32 Air quality device for mobile and fixed stations. (PM2.5 and CO2)
- [CO2 Gadget](https://emariete.com/medidor-co2-display-tft-color-ttgo-t-display-sensirion-scd30): Un medidor de CO2 de alta calidad con pantalla en color. (CO2)  
- [M5CoreInk Multi Sensor](https://github.com/hpsaturn/co2_m5coreink#readme): Wall CO2, T, H, P, Alt, sensor with low consumption (30 days)

# Credits

Thanks to all collaborators and [CanAirIO](https://canair.io) community for testing and reports. Visit us on [Telegram](https://t.me/canairio)

---
