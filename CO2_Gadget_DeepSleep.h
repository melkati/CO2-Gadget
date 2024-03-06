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
        Serial.println("-->[DEEP] Deep sleep enabled");
    } else {
        Serial.println("-->[DEEP] Deep sleep disabled");
    }
}

void SetTimeBetweenDeepSleep(uint16_t time) {
    timeBetweenDeepSleep = time;
    Serial.println("-->[DEEP] Time between deep sleep: " + String(timeBetweenDeepSleep) + " seconds");
}

void resetReason() {
    esp_reset_reason_t reason = esp_reset_reason();

    switch (reason) {
        case ESP_RST_UNKNOWN:
            Serial.println("Unknown reset reason");
            break;
        case ESP_RST_POWERON:
            Serial.println("Power-on reset");
            break;
        case ESP_RST_EXT:
            Serial.println("External reset");
            break;
        case ESP_RST_SW:
            Serial.println("Software reset");
            break;
        case ESP_RST_PANIC:
            Serial.println("Exception reset");
            break;
        case ESP_RST_INT_WDT:
            Serial.println("Watchdog reset (core)");
            break;
        case ESP_RST_TASK_WDT:
            Serial.println("Watchdog reset (task)");
            break;
        case ESP_RST_WDT:
            Serial.println("Watchdog reset (other)");
            break;
        case ESP_RST_DEEPSLEEP:
            Serial.println("Deep sleep reset");
            break;
        case ESP_RST_BROWNOUT:
            Serial.println("Brownout reset");
            break;
        case ESP_RST_SDIO:
            Serial.println("SDIO reset");
            break;
        default:
            Serial.println("Other reset reason");
            break;
    }
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
    Serial.printf("-->[DEEP] Going into deep sleep for %d seconds with LowPowerMode: %d\n", timeBetweenDeepSleep, sensors.getLowPowerMode());
    Serial.flush();
    delay(1000);
    saveGPIOConfig();
    deepSleepData.lowPowerMode = sensors.getLowPowerMode();
    // deepSleepData.co2Sensor = sensors.getMainDeviceSelected();

    esp_sleep_enable_timer_wakeup(timeBetweenDeepSleep * 1000000);
    esp_deep_sleep_start();
    // esp_deep_sleep(timeBetweenDeepSleep * 1000000);
}

bool isDataReadySingleShot() {
    bool dataReadyFlag = false;
    uint16_t error = 0;
    error = sensors.scd4x.getDataReadyFlag(dataReadyFlag);
    if (error != 0) {
        Serial.printf("-->[DEEP] isDataReadySingleShot() error: %d\n", error);
    }
    return dataReadyFlag;
}

void fromDeepSleep() {    
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2 = 0;
    float temperature = 0;
    float humidity = 0;
    // restoreGPIOConfig();
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            Serial.println("Wakeup caused by external signal using RTC_CNTL");
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("Wakeup caused by timer");
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            Serial.println("Wakeup caused by touchpad");
            break;
        case ESP_SLEEP_WAKEUP_ULP:
            Serial.println("Wakeup caused by ULP program");
            break;
        default:
            Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
            break;
    }

    // Set lowPowerMode using the correct type
    sensors.setLowPowerMode(static_cast<LowPowerMode>(deepSleepData.lowPowerMode));
    // gpioConfig;

    // Re-Initialize I2C bus
#ifdef I2C_SDA &&defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#else
    Wire.begin();
    sensors.scd4x.begin(Wire);
#endif

    switch (sensors.getLowPowerMode()) {
        case NO_LOWPOWER:
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: NO_LOWPOWER\n");
            break;
        case BASIC_LOWPOWER:
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: BASIC_LOWPOWER\n");
            break;
        case MEDIUM_LOWPOWER:
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: MEDIUM_LOWPOWER\n");
            Serial.println("-->[DEEP] Waking up from deep sleep. measureSingleShot() begin...");
            error = sensors.scd4x.measureSingleShot();
            if (error != 0) {
                Serial.printf("-->[DEEP] Waking up from deep sleep. measureSingleShot() error: %d\n", error);
            } else {
                Serial.println("-->[DEEP] Waking up from deep sleep. measureSingleShot() end...");
            }
            while (!isDataReadySingleShot()) {
                unsigned long currentMillis = millis();
                if (currentMillis - previousMillis >= 1000) {
                    previousMillis = currentMillis;
                    Serial.print("+");
                    delay(100);
                }
            }
            Serial.println("-->[DEEP] Waking up from deep sleep. measureSingleShot() data ready...");
            error = sensors.scd4x.readMeasurement(co2, temperature, humidity);
            if (error != 0) {
                Serial.printf("-->[DEEP] Waking up from deep sleep. readMeasurement() error: %d\n", error);
            } else {
                Serial.printf("-->[DEEP] Waking up from deep sleep. CO2: %d CO2temp: %.2f CO2humi: %.2f\n", co2, temperature, humidity);
            }
            toDeepSleep();

            break;
        case MAXIMUM_LOWPOWER:
            Serial.printf("-->[DEEP] Waking up from deep sleep. LowPowerMode: MAXIMUM_LOWPOWER\n");
            sensors.scd4x.wakeUp();
            sensors.scd4x.measureSingleShot();
            break;
        default:
            sensors.init();
            break;
    }

    Serial.flush();
}

#endif  // CO2_Gadget_DeepSleep_h