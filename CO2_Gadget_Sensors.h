#ifndef CO2_Gadget_Sensors_h
#define CO2_Gadget_Sensors_h

#include <Sensors.hpp>

volatile bool pendingCalibration = false;
volatile bool newReadingsAvailable = false;
uint16_t calibrationValue = 415;
uint16_t customCalibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;
uint16_t altitudeMeters = 600;
bool autoSelfCalibration = false;
float tempOffset = 0.0f;

volatile uint16_t co2 = 0;
volatile uint16_t previousCO2Value = 0;
float temp, tempFahrenheit, hum = 0;
String mainDeviceSelected = "";

String sensorsGetMainDeviceSelected() {
    return mainDeviceSelected;
}

void printSensorsDetected() {
    uint16_t sensors_count = sensors.getSensorsRegisteredCount();
    uint16_t units_count = sensors.getUnitsRegisteredCount();
    Serial.println("-->[SENS] Sensors detected count\t: " + String(sensors_count));
    Serial.println("-->[SENS] Sensors units count  \t: " + String(units_count));
    Serial.print("-->[SENS] Sensors devices names\t: ");
    int i = 0;
    while (sensors.getSensorsRegistered()[i++] != 0) {
        Serial.print(sensors.getSensorName((SENSORS)sensors.getSensorsRegistered()[i - 1]));
        mainDeviceSelected = sensors.getSensorName((SENSORS)sensors.getSensorsRegistered()[i - 1]);
        Serial.print(",");
    }
    Serial.println();
    if (mainDeviceSelected == "SCD4X") {
        Serial.print("-->[SENS] SCD4X model detected\t: ");
        sensors.scd4x.stopPeriodicMeasurement();
        mainDeviceSelected = sensors.getSCD4xModel();
        sensors.scd4x.startPeriodicMeasurement();
        Serial.println(mainDeviceSelected);
    }
}

void onSensorDataOk() {
    previousCO2Value = co2;
    co2 = sensors.getCO2();
    hum = sensors.getHumidity();
    if (hum == 0.0) hum = sensors.getCO2humi();
    temp = sensors.getTemperature();
    if (temp == 0.0) temp = sensors.getCO2temp();  // TO-DO: temp could be 0.0
    tempFahrenheit = (temp * 1.8 + 32);
    deepSleepData.lastCO2Value = co2;
    deepSleepData.lastTemperatureValue = temp;
    deepSleepData.lastHumidityValue = hum;
    if (!inMenu) {
        Serial.printf("-->[SENS] CO2: %d CO2temp: %.2f CO2humi: %.2f H: %.2f T: %.2f\n", co2, sensors.getCO2temp(), sensors.getCO2humi(), sensors.getHumidity(), sensors.getTemperature());
    }
    newReadingsAvailable = true;
    // Serial.printf("-->[SENS] Free heap: %d\n", ESP.getFreeHeap());
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

void storeSensorSelectedInRTC() {
    if (!sensorsGetMainDeviceSelected().isEmpty()) {
        Serial.println("-->[SENS] Storing main device selected to RTC Memory: [" + sensorsGetMainDeviceSelected() + "]");
        if (sensorsGetMainDeviceSelected() == "SCD40") {
            deepSleepData.co2Sensor = CO2Sensor_SCD40;
            Serial.println("-->[SENS][SCD4X] Sensor stored: CO2Sensor_SCD40");
            return;
        }

        if (sensorsGetMainDeviceSelected() == "SCD41") {
            deepSleepData.co2Sensor = CO2Sensor_SCD41;
            Serial.println("-->[SENS][SCD4X] Sensor stored: CO2Sensor_SCD41");
            return;
        }

        if ((sensorsGetMainDeviceSelected()) == "CM1106") {
            char softver[CM1106_LEN_SOFTVER];
            sensors.cm1106->get_software_version(softver);
            String softverStr(softver);
            Serial.println("-->[SENS][CM1106] Sensor stored: CO2Sensor_CM1106");
            // Serial.println("-->[SENS-CM1106] CM1106 version detected 1\t: " + softverStr);

            if (softverStr.length() >= 10 && softverStr.endsWith("SL-NS")) {
                deepSleepData.co2Sensor = CO2Sensor_CM1106SL_NS;
                Serial.println("-->[SENS][CM1106] Sensor stored: CO2Sensor_CM1106SL_NS");
            } else if (softverStr.endsWith("CM")) {
                deepSleepData.co2Sensor = CO2Sensor_CM1106;
                Serial.println("-->[SENS][CM1106] Sensor stored: CO2Sensor_CM1106");
            } else {
                deepSleepData.co2Sensor = CO2Sensor_NONE;
                Serial.println("-->[SENS][CM1106] Sensor stored: CO2Sensor_NONE");
            }
            return;
        }

        if ((sensorsGetMainDeviceSelected()) == "SCD30") {
            deepSleepData.co2Sensor = CO2Sensor_SCD30;
            Serial.println("-->[SENS][SCD30] Sensor stored: CO2Sensor_SCD30");
            return;
        }

        if ((sensorsGetMainDeviceSelected()) == "MHZ19") {
            deepSleepData.co2Sensor = CO2Sensor_MHZ19;
            Serial.println("-->[SENS][MHZ19] Sensor stored: CO2Sensor_MHZ19");
            return;
        }

        if ((sensorsGetMainDeviceSelected()) == "SENSEAIRS8") {
            deepSleepData.co2Sensor = CO2Sensor_SENSEAIRS8;
            Serial.println("-->[SENS][SENSEAIRS8] Sensor stored: CO2Sensor_SENSEAIRS8");
            return;
        }

        if ((sensorsGetMainDeviceSelected()) == "NONE") {
            deepSleepData.co2Sensor = CO2Sensor_NONE;
            Serial.println("-->[SENS][NONE][ERROR] Sensor stored: CO2Sensor_NONE");
            delay(10000);
            return;
        }
    }
}

void initSensorsLowPower() {
    const int8_t None = -1, AUTO = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6, DEMO = 127;
    if (selectedCO2Sensor == AUTO) {
        Serial.println("-->[SENS] Trying to init CO2 sensor in Low Power Mode: AutoSensor (I2C)");

               deepSleepData.measurementsStarted = false;
        sensors.initCO2LowPowerMode(static_cast<LowPowerMode>(deepSleepData.lowPowerMode));  // Cast deepSleepData.lowPowerMode to the appropriate type before passing it to the function
    }
}

void initSensors() {
    const int8_t None = -1, AUTO = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6, DEMO = 127;
    int16_t period;
    uint8_t smooth;

// Initialize sensors
#ifdef I2C_SDA &&defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#else
    Wire.begin();
#endif

    Serial.println("-->[SENS] Detecting sensors...");
    sensors.setOnDataCallBack(&onSensorDataOk);      // all data read callback
    sensors.setOnErrorCallBack(&onSensorDataError);  // [optional] error callback
    sensors.setDebugMode(debugSensors);              // [optional] debug mode
    sensors.setTempOffset(tempOffset);
    sensors.setCO2AltitudeOffset(altitudeMeters);
    // sensors.setAutoSelfCalibration(false); // TO-DO: Implement in CanAirIO Sensors Lib
    sensors.setSampleTime(measurementInterval);

    Serial.println("-->[SENS] Selected CO2 Sensor: " + sensors.getSensorName(static_cast<SENSORS>(selectedCO2Sensor)));
    Serial.println("-->[SENS] Measurement Interval: " + String(sensors.getSampleTime()));

    if ((deepSleepData.lowPowerMode) && (!interactiveMode)) {
        displayNotification("Init sensors", "Trying Low Power Mode: " + String(deepSleepData.lowPowerMode), notifyInfo);
        Serial.println("-->[SENS] Trying to init sensors in Low Power Mode: " + String(deepSleepData.lowPowerMode));
        initSensorsLowPower();
    } else {
        displayNotification("Init sensors", notifyInfo);
        if (selectedCO2Sensor == AUTO) {
            Serial.println("-->[SENS] Trying to init CO2 sensor: AutoSensor (I2C)");
            sensors.detectI2COnly(true);
            sensors.init();
        } else if (selectedCO2Sensor == MHZ19) {
            Serial.println("-->[SENS] Trying to init CO2 sensor: MHZ19(A/B/C/D)");
            sensors.detectI2COnly(false);
            sensors.init(MHZ19);
        } else if (selectedCO2Sensor == CM1106) {
            Serial.println("-->[SENS] Trying to init CO2 sensor: CM1106");
            sensors.detectI2COnly(false);
#ifdef CM1106_ENABLE_PIN
            pinMode(CM1106_ENABLE_PIN, OUTPUT);
            digitalWrite(CM1106_ENABLE_PIN, HIGH);
#endif
#ifdef CM1106_READY_PIN
            pinMode(CM1106_READY_PIN, INPUT);
#endif
#if defined(UART_RX_GPIO) && defined(UART_TX_GPIO)
            sensors.init(CM1106, UART_RX_GPIO, UART_TX_GPIO);
#else
            sensors.init(CM1106);
#endif
            sensors.cm1106->set_working_status(CM1106_CONTINUOUS_MEASUREMENT);
            sensors.cm1106->get_measurement_period(&period, &smooth);
            Serial.println("-->[SENS] CM1106 period: " + String(period) + " smooth: " + String(smooth));
            sensors.cm1106->set_measurement_period(measurementInterval, 1);
            sensors.cm1106->get_measurement_period(&period, &smooth);
            Serial.println("-->[SENS] CM1106 period: " + String(period) + " smooth: " + String(smooth));
            // sensors.cm1106->set_working_status(CM1106_SINGLE_MEASUREMENT);
        } else if (selectedCO2Sensor == SENSEAIRS8) {
            Serial.println("-->[SENS] Trying to init CO2 sensor: SENSEAIRS8");
            sensors.detectI2COnly(false);
            sensors.init(SENSEAIRS8);
        }
    }

    printSensorsDetected();
    storeSensorSelectedInRTC();
}

void sensorSCD30LoopLowPower() {
}

void sensorCM1106SL_NSLoopLowPower() {
}

void sensorSCD4XLoopLowPower() {
    bool dataReadyFlag = false;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    // Serial.println("-->[SENS] sensorSCD4XLoopLowPower() " + (sensors.getSCD4xModel() == "SCD40" ? "SCD40" : "SCD41"));

    error = sensors.scd4x.getDataReadyFlag(dataReadyFlag);
    if (error != 0) {
        Serial.printf("-->[DEEP] SCD4X sensorSCD4XLoopLowPower() --> getDataReadyFlag() error: %d\n", error);
    }

    if (dataReadyFlag) {
        error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
        if (error != 0) {
            Serial.printf("-->[DEEP] sensorSCD4XLoopLowPower() error: %d\n", error);
        } else {
            // Serial.println("-->[SENS] sensorSCD4XLoopLowPower() " + (sensors.getSCD4xModel() == "SCD40" ? "SCD40" : "SCD41"));
            Serial.printf("-->[SENS] CO2: %d CO2temp: %.2f CO2humi: %.2f H: %.2f T: %.2f Sensor: (%s)\n", co2value, temperature, humidity, humidity, temperature, sensors.getSCD4xModel().c_str());
            co2 = co2value;
            temp = temperature;
            hum = humidity;
        }
    }
}

void sensorsLoopLowPower() {
    if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD30)) {
        // Serial.println("sensorsLoopLowPower() SCD30");
        sensorSCD30LoopLowPower();
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_CM1106SL_NS)) {
        // Serial.println("sensorsLoopLowPower() CM1106SL_NS");
        sensorCM1106SL_NSLoopLowPower();
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD41)) {
        // Serial.println("sensorsLoopLowPower() SCD41");
        sensorSCD4XLoopLowPower();
    }
}

void sensorsLoop() {
    static unsigned long lastDotPrintTime = 0;
    if ((!interactiveMode) && (deepSleepData.lowPowerMode == MEDIUM_LOWPOWER) || (deepSleepData.lowPowerMode == MAXIMUM_LOWPOWER)) {
        if (millis() - lastDotPrintTime >= 100) {
            Serial.print("[-]");        // Print a - every loop to show that the device is alive
            lastDotPrintTime = millis();
        }
        sensorsLoopLowPower();
    } else if (!buzzerBeeping) {  // Avoid affecting beep sound
        // if (millis() - lastDotPrintTime >= 100) {
        //     Serial.print("[+] ");        // Print a + every loop to show that the device is alive
        //     lastDotPrintTime = millis();
        // }
        sensors.loop();
    }
}

#endif  // CO2_Gadget_Sensors_h