#ifndef CO2_Gadget_DeepSleep_h
#define CO2_Gadget_DeepSleep_h

#include "esp32-hal-gpio.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                    DEEP SLEEP                                     *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <Sensors.hpp>

#define DEEP_SLEEP_DEBUG

void SetDeepSleepEnabled(bool enabled) {
    deepSleepEnabled = enabled;
    if (deepSleepEnabled) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] Deep sleep enabled");
#endif
    } else {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] Deep sleep disabled");
#endif
    }
}

void SetTimeBetweenDeepSleep(uint16_t time) {
    timeBetweenDeepSleep = time;
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP] Time between deep sleep: " + String(timeBetweenDeepSleep) + " seconds");
#endif
}

void resetReason() {
    esp_reset_reason_t reason = esp_reset_reason();
#ifdef DEEP_SLEEP_DEBUG
    switch (reason) {
        case ESP_RST_UNKNOWN:
            Serial.println("-->[DEEP] Unknown reset reason");
            break;
        case ESP_RST_POWERON:
            Serial.println("-->[DEEP] Power-on reset");
            break;
        case ESP_RST_EXT:
            Serial.println("-->[DEEP] External reset");
            break;
        case ESP_RST_SW:
            Serial.println("-->[DEEP] Software reset");
            break;
        case ESP_RST_PANIC:
            Serial.println("-->[DEEP] Exception reset");
            break;
        case ESP_RST_INT_WDT:
            Serial.println("-->[DEEP] Watchdog reset (core)");
            break;
        case ESP_RST_TASK_WDT:
            Serial.println("-->[DEEP] Watchdog reset (task)");
            break;
        case ESP_RST_WDT:
            Serial.println("-->[DEEP] Watchdog reset (other)");
            break;
        case ESP_RST_DEEPSLEEP:
            Serial.println("-->[DEEP] Deep sleep reset");
            break;
        case ESP_RST_BROWNOUT:
            Serial.println("-->[DEEP] Brownout reset");
            break;
        case ESP_RST_SDIO:
            Serial.println("-->[DEEP] SDIO reset");
            break;
        default:
            Serial.println("-->[DEEP] Other reset reason");
            break;
    }
#endif
}

// Save GPIO pins configuration before deep sleep
void saveGPIOConfig() {
    // Save the state of GPIO pins
    sensors.lowPowerData.gpioConfig = 0;
    for (gpio_num_t i = GPIO_NUM_0; i < GPIO_NUM_MAX; i = static_cast<gpio_num_t>(i + 1)) {
        if (gpio_get_level(i)) {
            sensors.lowPowerData.gpioConfig |= (1 << i);
        }
    }
}

// Restore GPIO pins configuration after deep sleep
void restoreGPIOConfig() {
    // Restore the state of GPIO pins
    for (gpio_num_t i = GPIO_NUM_0; i < GPIO_NUM_MAX; i = static_cast<gpio_num_t>(i + 1)) {
        if (sensors.lowPowerData.gpioConfig & (1 << i)) {
            gpio_set_level(i, HIGH);
        } else {
            gpio_set_level(i, LOW);
        }
    }
}

void toDeepSleep() {
#ifdef TIMEDEBUG
    Serial.printf("-->[DEEP] Time awake: %d ms\n", timerAwake.read());
#endif
    Serial.printf("-->[DEEP] Going into deep sleep for %d seconds with LowPowerMode: %d\n", timeBetweenDeepSleep, sensors.getLowPowerMode());
    Serial.flush();
    // Experimental: Turn off green LED and display on S3 board
    // #if defined(CONFIG_IDF_TARGET_ESP32S3)
    // digitalWrite(TFT_POWER_ON_BATTERY, LOW);
    // #endif
    saveGPIOConfig();
    deepSleepData.lowPowerMode = sensors.getLowPowerMode();

    // Pull up pin 13 to put flash memory into deep sleep
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    // gpio_hold_en(gpio_num_t(13));

    esp_deep_sleep_disable_rom_logging();
#ifdef BTN_WAKEUP
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_WAKEUP), BTN_WAKEUP_ON);  // 1 = High, 0 = Low
#else
    if (BTN_UP >= 0) {
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_UP), LOW);  // 1 = High, 0 = Low
    }
    if (BTN_DWN >= 0) {
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_DWN), LOW);  // 1 = High, 0 = Low
    }
#endif
    esp_sleep_enable_timer_wakeup(timeBetweenDeepSleep * 1000000);
    delay(10);
    gpio_deep_sleep_hold_en();
    esp_deep_sleep_start();
}

bool isDataReadySingleShot() {
    bool dataReadyFlag = false;
    uint16_t error = 0;
    error = sensors.scd4x.getDataReadyFlag(dataReadyFlag);
    if (error != 0) {
        Serial.printf("-->[DEEP] SCD4X isDataReadySingleShot() error: %d\n", error);
    }
    return dataReadyFlag;
}

void doDeepSleepMQTTConnect() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("-->[DEEP] Initializing MQTT to broker IP: %s\n", mqttBroker.c_str());
        mqttClient.setServer(mqttBroker.c_str(), 1883);
        mqttClient.setCallback(callbackMQTT);
        mqttClient.setBufferSize(1024);
        if (mqttClient.connect((mqttClientId).c_str(), (mqttUser).c_str(), (mqttPass).c_str())) {
            Serial.printf("connected\n");
            Serial.print("-->[DEEP] rootTopic: ");
            Serial.println(rootTopic);
        }
    }
}

void doDeepSleepWiFiConnect() {
    initPreferences();
    initBattery();
    connectToWiFi();
#ifdef SUPPORT_ESPNOW
    initESPNow();
#endif
#ifdef SUPPORT_MQTT
    doDeepSleepMQTTConnect();
#endif
    deepSleepData.cyclesToWiFiConnect = deepSleepWiFiConnectEach;
}

void displayFromDeepSleep(bool forceRedraw = false) {
#ifdef SUPPORT_EINK
    initDisplayFromDeepSleep(forceRedraw);
    displayShowValues();
#endif
}

void scd4xHandleFromDeepSleep() {
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    // Re-Initialize I2C bus
#ifdef I2C_SDA &&defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#else
    Wire.begin();
    sensors.scd4x.begin(Wire);
#endif

    error = sensors.scd4x.measureSingleShot(true);
    if (error != 0) {
        Serial.printf("-->[DEEP] Waking up from deep sleep. measureSingleShot() error: %d\n", error);
    }
    while (!isDataReadySingleShot()) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            Serial.print("+");
            delay(10);
        }
    }
    error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
    co2 = co2value;
    temp = temperature;
    hum = humidity;
    if (error != 0) {
        Serial.printf("-->[DEEP] Waking up from deep sleep. readMeasurement() error: %d\n", error);
    } else {
        displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
    }
}

void cm1106HandleFromDeepSleep() {
    pinMode(CM1106_ENABLE_PIN, OUTPUT);
    digitalWrite(CM1106_ENABLE_PIN, HIGH);
    pinMode(CM1106_READY_PIN, INPUT);
    Serial.println("-->[DEEP] Waking up from deep sleep. CM1106 not supported yet");
#if defined(UART_RX_GPIO) && defined(UART_TX_GPIO)
    sensors.init(CM1106, UART_RX_GPIO, UART_TX_GPIO);
#else
    sensors.init(CM1106);
#endif

    while (true) {
        if (digitalRead(CM1106_READY_PIN) == HIGH) {
            co2 = sensors.cm1106->get_co2();
            Serial.printf("[DEEP] CO2 value: %d ppm\n", co2);
            displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
        }
    }
}

void scd30HandleFromDeepSleep() {
}

void fromDeepSleepTimer() {
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    --deepSleepData.cyclesToWiFiConnect;
    --deepSleepData.cyclesToRedrawDisplay;
    Serial.printf("-->[DEEP] Cycles left to redraw display: %d\n", deepSleepData.cyclesToRedrawDisplay);
    Serial.printf("-->[DEEP] Cycles left to connect to WiFi: %d\n", deepSleepData.cyclesToWiFiConnect);

    // restoreGPIOConfig();

    // Set lowPowerMode using the correct type
    // sensors.setLowPowerMode(static_cast<LowPowerMode>(deepSleepData.lowPowerMode));
    // gpioConfig;

    switch (sensors.getLowPowerMode()) {
        case NO_LOWPOWER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: NO_LOWPOWER\n");
#endif
            break;

        case BASIC_LOWPOWER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: BASIC_LOWPOWER\n");
#endif
            break;

        case MEDIUM_LOWPOWER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: MEDIUM_LOWPOWER\n");
#endif
            if (deepSleepData.cyclesToWiFiConnect == 0) {
                doDeepSleepWiFiConnect();
            }

            Serial.printf("-->[DEEP] deepSleepData.co2Sensor: %d\n", deepSleepData.co2Sensor);

            if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD30)) {
                scd30HandleFromDeepSleep();
            } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_CM1106SL_NS)) {
                cm1106HandleFromDeepSleep();
            } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD41)) {
                scd4xHandleFromDeepSleep();
            } else {
                Serial.printf("-->[DEEP] deepSleepData.co2Sensor: Unknown\n");
                sensors.init();
            }

#ifdef DEEP_SLEEP_DEBUG
            Serial.printf("-->[DEEP] CO2: %d CO2temp: %.2f CO2humi: %.2f\n", co2, temp, hum);
#endif

#ifdef SUPPORT_MQTT
            if (WiFi.status() == WL_CONNECTED) {
                Serial.printf("-->[DEEP] MQTT connected. Publishing measurements.\n");
                publishMQTT(true);
            }
#endif
            break;
        case MAXIMUM_LOWPOWER:
            break;
    }
#ifdef TIMEDEBUG
    Serial.printf("-->[DEEP] Time awake: %d ms\n", timerAwake.read());
#endif
    Serial.flush();
}

void fromDeepSleepRTC_IO() {
    return;

    if (isButtonPressedOnWakeUp()) {
        initDisplay(true);
        Serial.println("-->[DEEP] Button pressed on wakeup. Entering interactive mode.");
        delay(1000);
        return;
    }
}

void fromDeepSleep() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    // initButtons();
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("");
    Serial.println("");
#endif
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
        #ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by external signal using RTC_IO");
        #endif
            // fromDeepSleepRTC_IO();
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            #ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by external signal using RTC_CNTL");
            #endif
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
            #ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by timer");
            #endif
            fromDeepSleepTimer();
            turnOffDisplay();
            displaySleep(true);
            toDeepSleep();
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
        #ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by touchpad");
        #endif
            break;
        case ESP_SLEEP_WAKEUP_ULP:
        #ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by ULP program");
        #endif
            break;
        default:
        #ifdef DEEP_SLEEP_DEBUG
            Serial.printf("-->[DEEP] Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        #endif
            break;
    }
}

void initDeepSleep() {
    deepSleepData.cyclesToWiFiConnect = deepSleepWiFiConnectEach;
    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;
}

void deepSleepLoop() {
    if (inMenu) {
        startTimerToDeepSleep = millis();
    } else {
        if ((sensors.getLowPowerMode() != NO_LOWPOWER)) {
            if (millis() - startTimerToDeepSleep < waitToGoDeepSleepOnFirstBoot) {
                // Serial.printf("-->[MAIN] Waiting to go to deep sleep in: %d seconds\n", (waitToGoDeepSleepOnFirstBoot - (millis() - startTimerToDeepSleep)) / 1000);
                // Serial.flush();
            } else {
                turnOffDisplay();
                displaySleep(true);
                toDeepSleep();
            }
        }
    }
}

#endif  // CO2_Gadget_DeepSleep_h