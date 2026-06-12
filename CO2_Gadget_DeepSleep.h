#ifndef CO2_Gadget_DeepSleep_h
#define CO2_Gadget_DeepSleep_h

#include "esp32-hal-gpio.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                    DEEP SLEEP                                     *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <Sensors.hpp>

const uint16_t DEEP_SLEEP_SECONDS_MIN = 15;
const uint16_t DEEP_SLEEP_SECONDS_MAX = 900;
const uint16_t DEEP_SLEEP_SECONDS_DEFAULT = 60;

#ifdef DEBUG_DEEP
#define DEEP_SLEEP_DEBUG
#endif

#ifdef DEBUG_DEEP_RTC
#define DEEP_SLEEP_DEBUG2
#endif

// CO2 sensors enum
// typedef enum {
//     CO2Sensor_NONE = -1,
//     CO2Sensor_SCD30 = 1,
//     CO2Sensor_SCD40 = 2,
//     CO2Sensor_SCD41 = 3,
//     CO2Sensor_MHZ19 = 4,
//     CO2Sensor_CM1106 = 5,
//     CO2Sensor_CM1106SL_NS = 6,
//     CO2Sensor_SENSEAIRS8 = 7,
//     CO2Sensor_DEMO = 127
// } CO2SENSORS_t;

String getLowPowerModeName(uint16_t mode) {
    switch (mode) {
        case HIGH_PERFORMANCE:
            return "HIGH_PERFORMANCE";
        case 1:
            return "LOW_POWER";
        default:
            return "UNKNOWN";
    }
}

String getDeepSleepDataCo2SensorName() {
    switch (deepSleepData.co2Sensor) {
        case CO2Sensor_NONE:
            return "NONE";
        case CO2Sensor_SCD30:
            return "SCD30";
        case CO2Sensor_SCD40:
            return "SCD40";
        case CO2Sensor_SCD41:
            return "SCD41";
        case CO2Sensor_MHZ19:
            return "MHZ19";
        case CO2Sensor_CM1106:
            return "CM1106";
        case CO2Sensor_CM1106SL_NS:
            return "CM1106SL_NS";
        case CO2Sensor_SENSEAIRS8:
            return "SENSEAIRS8";
        case CO2Sensor_DEMO:
            return "DEMO";
        default:
            return "UNKNOWN";
    }
}

void printWakeupTouchpad() {
    touch_pad_t touchPin;
    touchPin = esp_sleep_get_touchpad_wakeup_status();

#if CONFIG_IDF_TARGET_ESP32
    switch (touchPin) {
        case 0:
            Serial.println("-->[DEEP] Touch detected on GPIO 4");
            break;
        case 1:
            Serial.println("-->[DEEP] Touch detected on GPIO 0");
            break;
        case 2:
            Serial.println("-->[DEEP] Touch detected on GPIO 2");
            break;
        case 3:
            Serial.println("-->[DEEP] Touch detected on GPIO 15");
            break;
        case 4:
            Serial.println("-->[DEEP] Touch detected on GPIO 13");
            break;
        case 5:
            Serial.println("-->[DEEP] Touch detected on GPIO 12");
            break;
        case 6:
            Serial.println("-->[DEEP] Touch detected on GPIO 14");
            break;
        case 7:
            Serial.println("-->[DEEP] Touch detected on GPIO 27");
            break;
        case 8:
            Serial.println("-->[DEEP] Touch detected on GPIO 33");
            break;
        case 9:
            Serial.println("-->[DEEP] Touch detected on GPIO 32");
            break;
        default:
            Serial.println("-->[DEEP] Wakeup not by touchpad");
            break;
    }
#else
    if (touchPin < TOUCH_PAD_MAX) {
        Serial.printf("-->[DEEP] Touch detected on GPIO %d\n", touchPin);
    } else {
        Serial.println("-->[DEEP] Wakeup not by touchpad");
    }
#endif
}

String getWakeupCause() {
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_UNDEFINED:
            return "Undefined";
        case ESP_SLEEP_WAKEUP_ALL:
            return "All";
        case ESP_SLEEP_WAKEUP_EXT0:
            return "External signal using RTC_IO";
        case ESP_SLEEP_WAKEUP_EXT1:
            return "External signal using RTC_CNTL";
        case ESP_SLEEP_WAKEUP_TIMER:
            return "Timer";
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            return "Touchpad";
        case ESP_SLEEP_WAKEUP_ULP:
            return "ULP program";
        case ESP_SLEEP_WAKEUP_GPIO:
            return "GPIO";
        case ESP_SLEEP_WAKEUP_UART:
            return "UART";
        case ESP_SLEEP_WAKEUP_WIFI:
            return "WIFI";
        case ESP_SLEEP_WAKEUP_COCPU:
            return "COCPU int";
        case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG:
            return "COCPU crash";
        case ESP_SLEEP_WAKEUP_BT:
            return "BT";
        default:
            return "Unknown";
    }
}

String getResetReason() {
    switch (esp_reset_reason()) {
        case ESP_RST_UNKNOWN:
            return "Unknown reset reason";
        case ESP_RST_POWERON:
            return "Power on reset";
        case ESP_RST_EXT:
            return "External reset";
        case ESP_RST_SW:
            return "Software reset";
        case ESP_RST_PANIC:
            return "Software reset due to exception/panic";
        case ESP_RST_INT_WDT:
            return "Reset due to interrupt watchdog";
        case ESP_RST_TASK_WDT:
            return "Reset due to task watchdog";
        case ESP_RST_WDT:
            return "Reset due to other watchdogs";
        case ESP_RST_DEEPSLEEP:
            return "Reset after exiting deep sleep mode";
        case ESP_RST_BROWNOUT:
            return "Brownout reset";
        case ESP_RST_SDIO:
            return "Reset over SDIO";
        default:
            return "Unknown reset reason";
    }
}

void printResetReason() {
#ifdef DEEP_SLEEP_DEBUG
    // Serial.println("-->[DEEP] Reason for reset: " + getResetReason());
#endif
}

void printRTCMemoryEnter() {
#ifdef DEEP_SLEEP_DEBUG2
    Serial.println("-->[DEEP][ENTER DeepSleep] lowPowerMode: " + String(deepSleepData.lowPowerMode));
    Serial.println("-->[DEEP][ENTER DeepSleep] co2Sensor: " + String(deepSleepData.co2Sensor));
    Serial.println("-->[DEEP][ENTER DeepSleep] waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot));
    Serial.println("-->[DEEP][ENTER DeepSleep] timeSleeping: " + String(deepSleepData.timeSleeping));
    Serial.println("-->[DEEP][ENTER DeepSleep] gpioConfig: " + String(deepSleepData.gpioConfig));
    Serial.println("-->[DEEP][ENTER DeepSleep] waitingForDataReady: " + String(deepSleepData.waitingForDataReady));
    Serial.println("-->[DEEP][ENTER DeepSleep] cyclesLeftToWiFiConnect: " + String(deepSleepData.cyclesLeftToWiFiConnect));
    Serial.println("-->[DEEP][ENTER DeepSleep] cyclesLeftToRedrawDisplay: " + String(deepSleepData.cyclesLeftToRedrawDisplay));
    Serial.println("-->[DEEP][ENTER DeepSleep] lastCO2Value: " + String(deepSleepData.lastCO2Value));
    Serial.println("-->[DEEP][ENTER DeepSleep] lastTemperatureValue: " + String(deepSleepData.lastTemperatureValue));
    Serial.println("-->[DEEP][ENTER DeepSleep] lastHumidityValue: " + String(deepSleepData.lastHumidityValue));
    Serial.println("-->[DEEP][ENTER DeepSleep] activeBLEOnWake: " + String(deepSleepData.activeBLEOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] activeWifiOnWake: " + String(deepSleepData.activeWifiOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] sendMQTTOnWake: " + String(deepSleepData.sendMQTTOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] sendESPNowOnWake: " + String(deepSleepData.sendESPNowOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] displayOnWake: " + String(deepSleepData.displayOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] displayReverseOnWake: " + String(deepSleepData.displayReverseOnWake));
    Serial.println("-->[DEEP][ENTER DeepSleep] Wake up times: " + String(deepSleepData.bootTimes));
#endif
}

void printRTCMemoryExit() {
#ifdef DEEP_SLEEP_DEBUG2
    Serial.println("-->[DEEP][EXIT DeepSleep] lowPowerMode: " + String(deepSleepData.lowPowerMode));
    Serial.println("-->[DEEP][EXIT DeepSleep] co2Sensor: " + String(deepSleepData.co2Sensor));
    Serial.println("-->[DEEP][EXIT DeepSleep] waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot));
    Serial.println("-->[DEEP][EXIT DeepSleep] timeSleeping: " + String(deepSleepData.timeSleeping));
    Serial.println("-->[DEEP][EXIT DeepSleep] gpioConfig: " + String(deepSleepData.gpioConfig));
    Serial.println("-->[DEEP][EXIT DeepSleep] waitingForDataReady: " + String(deepSleepData.waitingForDataReady));
    Serial.println("-->[DEEP][EXIT DeepSleep] cyclesLeftToWiFiConnect: " + String(deepSleepData.cyclesLeftToWiFiConnect));
    Serial.println("-->[DEEP][EXIT DeepSleep] cyclesLeftToRedrawDisplay: " + String(deepSleepData.cyclesLeftToRedrawDisplay));
    Serial.println("-->[DEEP][EXIT DeepSleep] lastCO2Value: " + String(deepSleepData.lastCO2Value));
    Serial.println("-->[DEEP][EXIT DeepSleep] lastTemperatureValue: " + String(deepSleepData.lastTemperatureValue));
    Serial.println("-->[DEEP][EXIT DeepSleep] lastHumidityValue: " + String(deepSleepData.lastHumidityValue));
    Serial.println("-->[DEEP][EXIT DeepSleep] activeBLEOnWake: " + String(deepSleepData.activeBLEOnWake));
    Serial.println("-->[DEEP][EXIT DeepSleep] activeWifiOnWake: " + String(deepSleepData.activeWifiOnWake));
    Serial.println("-->[DEEP][EXIT DeepSleep] sendMQTTOnWake: " + String(deepSleepData.sendMQTTOnWake));
    Serial.println("-->[DEEP][EXIT DeepSleep] sendESPNowOnWake: " + String(deepSleepData.sendESPNowOnWake));
    Serial.println("-->[DEEP][EXIT DeepSleep] displayOnWake: " + String(deepSleepData.displayOnWake));    
    Serial.println("-->[DEEP][EXIT DeepSleep] displayReverseOnWake: " + String(deepSleepData.displayReverseOnWake));
    Serial.println("-->[DEEP][EXIT DeepSleep] Wake up times: " + String(deepSleepData.bootTimes));
#endif
}

void restartTimerToDeepSleep() {
    startTimerToDeepSleep = millis();
#ifdef DEEP_SLEEP_DEBUG
    // Serial.println("-->[DEEP] Restarting timer to deep sleep.");
#endif
}

void callbackTouch() {
    // placeholder callback function
}

uint32_t calculateBLEWakeSettingsChecksum() {
    uint32_t checksum = 2166136261UL;
    auto mixByte = [&checksum](uint8_t value) {
        checksum ^= value;
        checksum *= 16777619UL;
    };

    mixByte(static_cast<uint8_t>(bleWakeSettingsRTC.enableBLEOnWake));
    mixByte(static_cast<uint8_t>(bleWakeSettingsRTC.sensirionBLEOnWake));
    mixByte(static_cast<uint8_t>(bleWakeSettingsRTC.activeBTHomeOnWake));
    mixByte(static_cast<uint8_t>(bleWakeSettingsRTC.bthomeEncryptionOnWake));
    for (uint8_t i = 0; i < sizeof(bleWakeSettingsRTC.bthomeBindKeyOnWake); ++i) {
        mixByte(static_cast<uint8_t>(bleWakeSettingsRTC.bthomeBindKeyOnWake[i]));
    }
    for (uint8_t i = 0; i < sizeof(bleWakeSettingsRTC.bthomeCounterOnWake); ++i) {
        mixByte(static_cast<uint8_t>((bleWakeSettingsRTC.bthomeCounterOnWake >> (i * 8)) & 0xFF));
    }

    return checksum;
}

void saveBLEWakeSettingsToRTC() {
#ifdef SUPPORT_BLE
    bleWakeSettingsRTC.magic = BLE_WAKE_SETTINGS_MAGIC;
    bleWakeSettingsRTC.enableBLEOnWake = enableBLE;
    bleWakeSettingsRTC.sensirionBLEOnWake = activeBLE;
#ifdef SUPPORT_BTHOME_BLE
    bleWakeSettingsRTC.activeBTHomeOnWake = activeBTHome;
    bleWakeSettingsRTC.bthomeEncryptionOnWake = bthomeEncryption;
    bleWakeSettingsRTC.bthomeCounterOnWake = bthomeCounter;
    bthomeBindKey.toCharArray(bleWakeSettingsRTC.bthomeBindKeyOnWake, sizeof(bleWakeSettingsRTC.bthomeBindKeyOnWake));
#else
    bleWakeSettingsRTC.activeBTHomeOnWake = false;
    bleWakeSettingsRTC.bthomeEncryptionOnWake = false;
    bleWakeSettingsRTC.bthomeCounterOnWake = 0;
    bleWakeSettingsRTC.bthomeBindKeyOnWake[0] = '\0';
#endif
    bleWakeSettingsRTC.checksum = calculateBLEWakeSettingsChecksum();
    Serial.println("-->[DEEP] Saved BLE wake settings. magic: 0x" + String(bleWakeSettingsRTC.magic, HEX) + ", checksum: 0x" + String(bleWakeSettingsRTC.checksum, HEX) + ", actBLEOnWake: " + String(deepSleepData.activeBLEOnWake) + ", enableBLE: " + String(enableBLE) + ", activeBLE: " + String(activeBLE) + ", activeBTHome: " + String(bleWakeSettingsRTC.activeBTHomeOnWake));
#endif
}

void restoreBLEWakeSettingsFromRTC() {
#ifdef SUPPORT_BLE
    uint32_t currentBLEWakeSettingsChecksum = calculateBLEWakeSettingsChecksum();
    if ((bleWakeSettingsRTC.magic != BLE_WAKE_SETTINGS_MAGIC) || (bleWakeSettingsRTC.checksum != currentBLEWakeSettingsChecksum)) {
        uint32_t previousBLEWakeSettingsMagic = bleWakeSettingsRTC.magic;
        uint32_t previousBLEWakeSettingsChecksum = bleWakeSettingsRTC.checksum;
        preferences.begin("CO2-Gadget", false);
        enableBLE = preferences.getBool("enableBLE", true);
        activeBLE = preferences.getBool("activeBLE", true);
#ifdef SUPPORT_BTHOME_BLE
        activeBTHome = preferences.getBool("activeBTHome", false);
        bthomeEncryption = preferences.getBool("bthomeEncrypt", false);
        bthomeBindKey = preferences.getString("bthomeBindKey", "");
        bthomeCounter = preferences.getUInt("bthomeCounter", 0);
        bthomeCounterNeedsSeed = true;
        ensureBTHomeBindKey();
#endif
        preferences.end();
        saveBLEWakeSettingsToRTC();
        Serial.println("-->[DEEP] Restored BLE wake settings from preferences because RTC snapshot was not valid. previous magic: 0x" + String(previousBLEWakeSettingsMagic, HEX) + ", previous checksum: 0x" + String(previousBLEWakeSettingsChecksum, HEX) + ", calculated checksum: 0x" + String(currentBLEWakeSettingsChecksum, HEX));
        return;
    }
    enableBLE = bleWakeSettingsRTC.enableBLEOnWake;
    activeBLE = bleWakeSettingsRTC.sensirionBLEOnWake;
#ifdef SUPPORT_BTHOME_BLE
    activeBTHome = bleWakeSettingsRTC.activeBTHomeOnWake;
    bthomeEncryption = bleWakeSettingsRTC.bthomeEncryptionOnWake;
    bthomeCounter = bleWakeSettingsRTC.bthomeCounterOnWake;
    bthomeBindKey = String(bleWakeSettingsRTC.bthomeBindKeyOnWake);
#endif
    Serial.println("-->[DEEP] Restored BLE wake settings. magic: 0x" + String(bleWakeSettingsRTC.magic, HEX) + ", checksum: 0x" + String(bleWakeSettingsRTC.checksum, HEX) + ", actBLEOnWake: " + String(deepSleepData.activeBLEOnWake) + ", enableBLE: " + String(enableBLE) + ", activeBLE: " + String(activeBLE) + ", activeBTHome: " + String(activeBTHome));
#endif
}

uint16_t getValidatedDeepSleepSeconds() {
    if ((deepSleepData.timeSleeping < DEEP_SLEEP_SECONDS_MIN) || (deepSleepData.timeSleeping > DEEP_SLEEP_SECONDS_MAX)) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] Invalid timeSleeping in RTC memory: " + String(deepSleepData.timeSleeping) + ". Using " + String(DEEP_SLEEP_SECONDS_DEFAULT) + " seconds.");
#endif
        deepSleepData.timeSleeping = DEEP_SLEEP_SECONDS_DEFAULT;
    }

    return deepSleepData.timeSleeping;
}

void prepareServicesForDeepSleep() {
#ifdef SUPPORT_MQTT
    if (mqttClient.connected()) {
        mqttClient.loop();
        delay(10);
        mqttClient.disconnect();
        delay(20);
    }
#endif

#ifdef SUPPORT_ESPNOW
    if (EspNowInititialized) {
        disableESPNow();
        delay(20);
    }
#endif

#ifdef SUPPORT_BLE
    disableBLE();
#endif

    if (WiFi.getMode() != WIFI_OFF) {
        stopWiFiForDeepSleep();
    }
}

void toDeepSleep() {
#ifdef SUPPORT_EINK
// display.hibernate();
#endif

    deepSleepData.uptimeMillis += millis();
    const uint16_t sleepSeconds = getValidatedDeepSleepSeconds();

#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
                deepSleepData.displayReverseOnWake = displayReverse;
#endif
    saveBLEWakeSettingsToRTC();

    if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD30)) {
        // sensors.scd30.stopContinuousMeasurement();
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD41)) {
        // SCD41 supports powerDown() (idle→sleep: ~0.5 mA → <0.1 mA).
        // SCD40 does NOT support powerDown — use startLowPowerPeriodicMeasurement() instead.
        sensors.scd4x.stopPeriodicMeasurement();
        sensors.scd4x.powerDown();
    } else if ((deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD40))) {
        sensors.scd4x.stopPeriodicMeasurement();
        sensors.scd4x.startLowPowerPeriodicMeasurement();
    }

#ifdef DEEP_SLEEP_DEBUG
    Serial.println("");
    Serial.println("-->***********************************************************************************");
    Serial.println("-->[DEEP] Going into deep sleep for " + String(sleepSeconds) + " seconds with LowPowerMode: " + String(deepSleepData.lowPowerMode) + " (" + getLowPowerModeName(deepSleepData.lowPowerMode) + ")");
    Serial.println("-->***********************************************************************************");
    Serial.println("");
#endif
    printRTCMemoryEnter();

#ifdef BTN_WAKEUP_IS_TOUCHPAD
    // Setup interrupt on Touch Pad 0 (GPIO15)
    // touchAttachInterrupt(T3, callbackTouch, 40);
    touchAttachInterrupt(BTN_WAKEUP, callbackTouch, 40);
    esp_sleep_enable_touchpad_wakeup();
#endif

    // Cut display power rail before deep sleep (TDISPLAY_S3 and boards with TFT_POWER_ON_BATTERY)
    // Without this, gpio_deep_sleep_hold_en() keeps the pin HIGH and the display drains current during sleep.
#ifdef TFT_POWER_ON_BATTERY
    setDisplayBrightness(0);                  // Shutdown backlight IC first
    delay(5);
    digitalWrite(TFT_POWER_ON_BATTERY, LOW);  // Cut display power MOSFET
    delay(5);
#endif

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEW0213M21) || defined(EINKBOARDGDEM0213B74)
    // Pull up pin 13 to put flash memory into deep sleep
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
// gpio_hold_en(gpio_num_t(13));
#endif

#ifdef TIMEDEBUG
    Serial.println("-->[DEEP] Time awake: " + String(timerAwake.read()) + " ms (in light sleep for " + String(timerLightSleep.read()) + " ms)");
    Serial.println("");
    Serial.println("");
#endif
    prepareServicesForDeepSleep();
    Serial.flush();
    esp_deep_sleep_disable_rom_logging();
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    // #ifdef BTN_WAKEUP
    //     esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_WAKEUP), BTN_WAKEUP_ON);  // 1 = High, 0 = Low
    // #else

    // Configure GPIO wakeup source.
    // On ESP32-S3, ext0 wakeup uses esp_deep_sleep_enable_gpio_wakeup() (IDF 5.x LP GPIO API).
    // On classic ESP32, use esp_sleep_enable_ext0_wakeup() (RTC GPIO API).
#if CONFIG_IDF_TARGET_ESP32S3
    // ESP32-S3: use the GPIO wakeup API. On IDF 5.x this is esp_deep_sleep_enable_gpio_wakeup();
    // on older IDF the ext1 API with a single GPIO is used as a fallback.
    // Wakes on LOW level (button pressed = GND).
    if ((BTN_DWN != -1) && esp_sleep_is_valid_wakeup_gpio(static_cast<gpio_num_t>(BTN_DWN))) {
        gpio_pullup_en(static_cast<gpio_num_t>(BTN_DWN));    // ensure pull-up so pin is HIGH when idle
        gpio_pulldown_dis(static_cast<gpio_num_t>(BTN_DWN));
        // Use ext1 on ESP32-S3 (supports all GPIOs), wakeup when ALL listed GPIOs go LOW.
        esp_sleep_enable_ext1_wakeup(1ULL << BTN_DWN, ESP_EXT1_WAKEUP_ALL_LOW);
    } else if ((BTN_UP != -1) && esp_sleep_is_valid_wakeup_gpio(static_cast<gpio_num_t>(BTN_UP))) {
        gpio_pullup_en(static_cast<gpio_num_t>(BTN_UP));
        gpio_pulldown_dis(static_cast<gpio_num_t>(BTN_UP));
        esp_sleep_enable_ext1_wakeup(1ULL << BTN_UP, ESP_EXT1_WAKEUP_ALL_LOW);
    }
#else
    // Classic ESP32: only RTC-capable GPIOs can be used (0,2,4,12-15,25-27,32-39).
    if ((BTN_DWN != -1) && esp_sleep_is_valid_wakeup_gpio(static_cast<gpio_num_t>(BTN_DWN))) {
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_DWN), LOW);
    } else if ((BTN_UP != -1) && esp_sleep_is_valid_wakeup_gpio(static_cast<gpio_num_t>(BTN_UP))) {
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_UP), LOW);
    }
#endif
    // #endif
    esp_sleep_enable_timer_wakeup(static_cast<uint64_t>(sleepSeconds) * 1000000ULL);
    delay(5);

    prepareServicesForDeepSleep();

#if defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
    // On ESP32-S3 with USB CDC (HWCDC) enabled, the USB PHY can prevent deep sleep
    // or cause immediate wake-up unless fully torn down. Serial.end() stops the
    // HWCDC driver; the USB.end() call (if available) ensures the USB peripheral
    // is properly deregistered so the PHY can power down.
    Serial.end();
    delay(100);  // Allow USB PHY to finish disconnect sequence before entering deep sleep
#endif

    // Power down SPI flash/PSRAM LDO (VDDSDIO) during deep sleep.
    // ESP_PD_DOMAIN_VDDSDIO only exists on classic ESP32; on ESP32-S3 this call
    // is invalid and will trigger an abort. Guard it with the target check.
#if CONFIG_IDF_TARGET_ESP32
    esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
#endif

    gpio_deep_sleep_hold_en();
    // adc_oneshot_del_unit(adc_handle); // TO-DO: Check if this is needed measuring current consumption in deep sleep
    esp_deep_sleep_start();
}

bool isDataReadySCD4x() {
    bool dataReadyFlag = false;
    uint16_t error = 0;
    error = sensors.scd4x.getDataReadyFlag(dataReadyFlag);
    if (error != 0) {
        Serial.println("-->[DEEP] SCD4X isDataReadySCD4x() error: " + String(error));
    }
    return dataReadyFlag;
}

void doDeepSleepMQTTConnect() {
    if (WiFi.status() == WL_CONNECTED) {
        // Serial.printf("-->[DEEP] Initializing MQTT to broker IP: %s\n", mqttBroker.c_str());
        Serial.println("-->[DEEP] Initializing MQTT tobroker IP: " + mqttBroker);
        mqttClient.setServer(mqttBroker.c_str(), 1883);
        mqttClient.setCallback(callbackMQTT);
        mqttClient.setBufferSize(1024);
        if (mqttClient.connect((mqttClientId).c_str(), (mqttUser).c_str(), (mqttPass).c_str())) {
            // Serial.printf("connected\n");
            Serial.println("-->[DEEP] MQTT connected.");
            Serial.print("-->[DEEP] rootTopic: ");
            Serial.println(rootTopic);
        }
    }
}

void doDeepSleepWiFiConnect() {
    initPreferences();
    bool wifiConnected = WiFi.status() == WL_CONNECTED;
    if (deepSleepData.activeWifiOnWake) {
        wifiConnected = connectToWiFi();
    }

#ifdef SUPPORT_ESPNOW
    if (deepSleepData.sendESPNowOnWake && wifiConnected) {
        initESPNow();
    }
#endif
#ifdef SUPPORT_MQTT
    if (deepSleepData.sendMQTTOnWake && wifiConnected) {
        doDeepSleepMQTTConnect();
    }
#endif
    if (deepSleepData.activateWiFiEvery > 0) {
        if (wifiConnected) {
            deepSleepData.cyclesLeftToWiFiConnect = deepSleepData.activateWiFiEvery;
        } else {
            deepSleepData.cyclesLeftToWiFiConnect = 1;
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] WiFi connection failed. Will retry on next wake cycle.");
#endif
        }
    }
}

void displayFromDeepSleep(bool forceRedraw = false) {
#ifdef SUPPORT_EINK
    initDisplayFromDeepSleep(forceRedraw);
    einkDisplayUpdateFromDeepSleep = true;
    displayShowValues(forceRedraw);
    einkDisplayUpdateFromDeepSleep = false;
#endif
}

void reInitI2C() {
    // Re-Initialize I2C bus
    if (!Wire.available()) {
#if defined(CUSTOM_I2C_SDA) && defined(CUSTOM_I2C_SCL)
        Wire.begin(CUSTOM_I2C_SDA, CUSTOM_I2C_SCL);
#else
        Wire.begin();
#endif
        delay(10);
    }
    Wire.setTimeout(2000);
}

bool cm1106HandleFromDeepSleep() {
    pinMode(CM1106_ENABLE_PIN, OUTPUT);
    digitalWrite(CM1106_ENABLE_PIN, HIGH);
    pinMode(CM1106_READY_PIN, INPUT);
    Serial.print("-->[DEEP] Waking up from deep sleep. Handling CM1106 ");
#if defined(UART_RX_GPIO) && defined(UART_TX_GPIO)
    sensors.init(CM1106, UART_RX_GPIO, UART_TX_GPIO);
#else
    sensors.init(CM1106);
#endif

    while (digitalRead(CM1106_READY_PIN) == LOW) {
        Serial.print("+");
        delay(10);
    }

    Serial.println("");

    co2 = sensors.cm1106->get_co2();
    deepSleepData.lastCO2Value = co2;
    Serial.println("-->[DEEP] CO2 value: " + String(co2) + " ppm");
    return (true);
}

bool scd41HandleFromDeepSleep(bool blockingMode = true) {
    static bool initialized = false;
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    if (!initialized) {
        reInitI2C();
        sensors.scd4x.begin(Wire);
        // After powerDown() in toDeepSleep(), the SCD41 needs wakeUp() before any command.
        // wakeUp() is intentionally NACK'd by the sensor (it is in sleep) — that is expected.
        // The 20 ms delay is required per SCD41 datasheet before the next I2C command.
        sensors.scd4x.wakeUp();
        delay(20);
        initialized = true;
    }

    Serial.print("-->[DEEP] ");
    Serial.print(__func__);
    Serial.println("() Interactive mode: " + String(interactiveMode) + " Blocking mode: " + String(blockingMode) + " Data ready: " + String(isDataReadySCD4x()));

    if ((!blockingMode) && (!isDataReadySCD4x()) && (!interactiveMode)) {
        esp_sleep_enable_timer_wakeup(0.3 * 1000000);  // 0.3 seconds
                                                       // Serial.println("-->[DEEP] Light sleep for 0.3 seconds");
                                                       // Serial.flush();
#ifdef TIMEDEBUG
        timerLightSleep.resume();
#endif
        esp_light_sleep_start();
#ifdef TIMEDEBUG
        timerLightSleep.pause();
#endif
        return (false);
    }

    error = sensors.scd4x.measureSingleShot(true);
    if (error != 0) {
        Serial.println("-->[DEEP][ERROR] Waking up from deep sleep. measureSingleShot() error: " + String(error));
    }
    esp_sleep_enable_timer_wakeup(5 * 1000000);
    Serial.flush();
#ifdef TIMEDEBUG
    timerLightSleep.resume();
#endif
    if (esp_light_sleep_start() == ESP_OK) {
        Serial.println("-->[DEEP] Light sleep OK");
    } else {
        Serial.println("-->[DEEP] Light sleep failed");
    }
#ifdef TIMEDEBUG
    timerLightSleep.pause();
#endif

    while (!isDataReadySCD4x()) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            Serial.print("+");
        }
        delay(1);  // Feed the interrupt watchdog every iteration
    }
    error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
    if (error != 0) {
        Serial.println("-->[DEEP] Waking up from deep sleep. readMeasurement() error " + String(error));
        return (false);
    }
    co2 = co2value;
    temp = temperature;
    hum = humidity;
    deepSleepData.lastCO2Value = co2;
    deepSleepData.lastTemperatureValue = temp;
    deepSleepData.lastHumidityValue = hum;
    return (true);
}

bool scd40HandleFromDeepSleep(bool blockingMode = true) {
    static bool initialized = false;
    unsigned long previousMillis = 0;
    unsigned long startTimeoutMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    // Serial.println("-->[DEEP] " + __func__ + "() Interactive mode: " + String(interactiveMode) + " Blocking mode: " + String(blockingMode));

    if (!initialized) {
        reInitI2C();
        sensors.scd4x.begin(Wire);
        initialized = true;
    }

    if (!deepSleepData.measurementsStarted) {
        sensors.scd4x.stopPeriodicMeasurement();
        sensors.scd4x.startLowPowerPeriodicMeasurement();
        deepSleepData.measurementsStarted = true;
    }

    startTimeoutMillis = millis();
    if (!isDataReadySCD4x()) {
        if (!blockingMode) return (false);
        Serial.print("-->[DEEP] Waiting for data from sensor SCD40: ");
        while (!isDataReadySCD4x()) {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 1000) {
                previousMillis = currentMillis;
                Serial.print("+");
            }
            if (currentMillis - startTimeoutMillis >= 31000) {
                Serial.println("-->[DEEP] Timeout waiting for data from sensor SCD40");
                return (false);
            }
            Serial.flush();
            esp_sleep_enable_timer_wakeup(0.3 * 1000000);
#ifdef TIMEDEBUG
            timerLightSleep.resume();
#endif
            esp_light_sleep_start();
#ifdef TIMEDEBUG
            timerLightSleep.pause();
#endif
        }
        Serial.println("");
    }
    error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
    co2 = co2value;
    temp = temperature;
    hum = humidity;
    deepSleepData.lastCO2Value = co2;
    deepSleepData.lastTemperatureValue = temp;
    deepSleepData.lastHumidityValue = hum;
    if (error != 0) {
        Serial.println("-->[DEEP] Waking up from deep sleep. readMeasurement() error " + String(error));
        return (false);
    }
    return (true);
}

bool scd30HandleFromDeepSleep(bool blockingMode = true) {
    static bool initialized = false;
    unsigned long previousMillis = 0, startTimeoutMillis = millis();

    if (!initialized) {
        Serial.println("-->[DEEP] " + String(__func__) + "() Interactive mode: " + String(interactiveMode) + " Blocking mode: " + String(blockingMode));
        reInitI2C();
        sensors.setDebugMode(debugSensors);
        sensors.detectI2COnly(true);
        sensors.setSampleTime(measurementInterval);
        sensors.setOnDataCallBack(&onSensorDataOk);      // all data read callback
        sensors.setOnErrorCallBack(&onSensorDataError);  // [optional] error callback
        sensors.initCO2LowPowerMode(SENSORS::SSCD30, MEDIUM_LOWPOWER);
        initialized = true;
    }

#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP][SCD30] SCD30 is not fully supported in Low Power Mode (yet)");
#endif

    if (!sensors.isDataReady()) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD30] Waiting for data from sensor");
#endif
        while (!sensors.isDataReady()) {
            if (millis() - startTimeoutMillis >= (sensors.getSampleTime() + 1) * 1000) {  // If one second more than sample time then timeout
                Serial.println("-->[DEEP][SCD30][ERROR] Timeout waiting for data from sensor");
                return (false);
            }
            sensors.loop();
            if ((!sensors.isDataReady()) && (!interactiveMode)) {
                esp_sleep_enable_timer_wakeup(1 * 1000000);  // 1 second
                                                             // Serial.println("-->[DEEP] Light sleep for 1 second");
                                                             // Serial.flush();
#ifdef TIMEDEBUG
                timerLightSleep.resume();
#endif
                esp_light_sleep_start();
#ifdef TIMEDEBUG
                timerLightSleep.pause();
#endif
            }
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 1000) {
                previousMillis = currentMillis;
#ifdef DEEP_SLEEP_DEBUG
                Serial.print("+");
#endif
                delay(10);
            }
        }
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("");
#endif
    }

    return (true);
}

bool handleLowPowerSensors() {
    bool readOK = false;
    bool blockingMode = true;
    if ((deepSleepEnabled) && (interactiveMode)) blockingMode = false;
    if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD30)) {
#ifdef DEEP_SLEEP_DEBUG
        if (!interactiveMode) Serial.println("-->[DEEP][SCD30] Waking up from deep sleep. Handling SCD30");
#endif
        readOK = scd30HandleFromDeepSleep(true);

    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_CM1106SL_NS)) {
#ifdef DEEP_SLEEP_DEBUG
        if (!interactiveMode) Serial.println("-->[DEEP][CM1106SL-NS] Waking up from deep sleep. Handling CM1106SL_NS");
#endif
        readOK = cm1106HandleFromDeepSleep();
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD41)) {
#ifdef DEEP_SLEEP_DEBUG
        if (!interactiveMode) Serial.println("-->[DEEP][SCD41] Waking up from deep sleep. Handling SCD41");
#endif
        readOK = scd41HandleFromDeepSleep(blockingMode);  // NOTE: must be outside #ifdef DEEP_SLEEP_DEBUG
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD40)) {
#ifdef DEEP_SLEEP_DEBUG
        if (!interactiveMode) Serial.println("-->[DEEP][SCD40] Waking up from deep sleep. Handling SCD40");
#endif
        readOK = scd40HandleFromDeepSleep(blockingMode);  // NOTE: must be outside #ifdef DEEP_SLEEP_DEBUG
    } else {
#ifdef DEEP_SLEEP_DEBUG
        if (!interactiveMode) Serial.println("-->[DEEP][ERROR] deepSleepData.co2Sensor: Unknown");
#endif
        sensors.init();
    }
    return (readOK);
}

void handleCycleCountersOnWake() {
    if (deepSleepData.activateWiFiEvery > 0) {
        --deepSleepData.cyclesLeftToWiFiConnect;
        if (deepSleepData.cyclesLeftToWiFiConnect == 65535) deepSleepData.cyclesLeftToWiFiConnect = 0;
    }
    if (deepSleepData.redrawDisplayEveryCycles > 0) {
        --deepSleepData.cyclesLeftToRedrawDisplay;
        if (deepSleepData.cyclesLeftToRedrawDisplay == 65535) deepSleepData.cyclesLeftToRedrawDisplay = 0;
    }

#if defined(DEEP_SLEEP_DEBUG)
    Serial.println("-->[DEEP] Cycles left to connect to WiFi: " + String(deepSleepData.cyclesLeftToWiFiConnect));
    Serial.println("-->[DEEP] Display redraw cycles left: " + String(deepSleepData.cyclesLeftToRedrawDisplay));
#endif
}

void handleBLEOnWake() {
#ifdef SUPPORT_BLE
    if (deepSleepData.activeBLEOnWake && enableBLE && (activeBLE || activeBTHome)) {
        initBLE();
        Serial.println("-->[DEEP] BLE initialized. enableBLE: " + String(enableBLE) + ", activeBLE: " + String(activeBLE) + ", activeBTHome: " + String(activeBTHome));
        bool wakePayloadPublished = publishBLE(true);
        if (!wakePayloadPublished) {
            Serial.println("-->[DEEP] BLE wake advertisement skipped because no payload was published.");
        }
#ifdef SUPPORT_BTHOME_BLE
        else if (activeBLE && activeBTHome && sensirionBLEInitialized) {
            Serial.println("-->[DEEP] Sensirion BLE wake advertisement window: " + String(BLE_WAKE_ADVERTISEMENT_MS) + " ms");
            delay(BLE_WAKE_ADVERTISEMENT_MS);
            bool bthomePrimaryAdvertised = updateBTHomeAdvertisementData(false, true);
            if (bthomePrimaryAdvertised) {
                Serial.println("-->[DEEP] BTHome BLE wake advertisement window: " + String(BLE_WAKE_ADVERTISEMENT_MS) + " ms");
                delay(BLE_WAKE_ADVERTISEMENT_MS);
                restoreSensirionAdvertisementData();
                updateBTHomeAdvertisementData(false);
            } else {
                Serial.println("-->[DEEP] BTHome primary wake advertisement skipped.");
            }
        }
#endif
        else {
            Serial.println("-->[DEEP] BLE wake advertisement window: " + String(BLE_WAKE_ADVERTISEMENT_MS) + " ms");
            delay(BLE_WAKE_ADVERTISEMENT_MS);
        }
    } else {
        Serial.println("-->[DEEP] BLE wake skipped. actBLEOnWake: " + String(deepSleepData.activeBLEOnWake) + ", enableBLE: " + String(enableBLE) + ", activeBLE: " + String(activeBLE) + ", activeBTHome: " + String(activeBTHome));
    }
#endif
}

void handleDisplayReverseOnWake() {
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
    RTC_DATA_ATTR static bool lastDisplayReverseOnWake = false;
    RTC_DATA_ATTR static bool lastDisplayReverseOnWakeValid = false;
    bool displayReverseChanged = !lastDisplayReverseOnWakeValid || (lastDisplayReverseOnWake != deepSleepData.displayReverseOnWake);

    displayReverse = deepSleepData.displayReverseOnWake;
    if (displayReverseChanged) {
        setDisplayReverse(displayReverse);
        lastDisplayReverseOnWake = displayReverse;
        lastDisplayReverseOnWakeValid = true;
    }
    reverseButtons(displayReverse);
#endif
}

void handleDisplayRedrawOnWake() {
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
    if (deepSleepData.cyclesLeftToRedrawDisplay == 0) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] Updating display");
#endif
        initDisplay(true);
        displayShowValues(true);
        if (deepSleepData.redrawDisplayEveryCycles > 0) {
            deepSleepData.cyclesLeftToRedrawDisplay = deepSleepData.redrawDisplayEveryCycles;
        }
    }
#endif
}

void handleDisplayOnWake() {
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED)
    if (deepSleepData.displayOnWake && deepSleepData.cyclesLeftToRedrawDisplay == 0 && deepSleepData.redrawDisplayEveryCycles > 0) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] Displaying values momentarily for " + String(deepSleepData.timeToDisplayOnWake) + " seconds");
#endif
        initDisplay(true);
        displayShowValues(true);
        deepSleepData.cyclesLeftToRedrawDisplay = deepSleepData.redrawDisplayEveryCycles;
        esp_sleep_enable_timer_wakeup(deepSleepData.timeToDisplayOnWake * 1000000);
        Serial.flush();
        delay(deepSleepData.timeToDisplayOnWake * 1000);
    }
#endif
}

void handleWiFiConnectionOnWake() {
    if (deepSleepData.cyclesLeftToWiFiConnect == 0 && deepSleepData.activateWiFiEvery > 0) {
        doDeepSleepWiFiConnect();
    }
}

void handleMQTTPublishOnWake() {
#ifdef SUPPORT_MQTT
    if (deepSleepData.sendMQTTOnWake && WiFi.status() == WL_CONNECTED) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP] MQTT connected. Publishing measurements.");
#endif
        publishMQTT(true);
        delay(10);
    }
#endif
}

void handleMediumLowPowerModeOnWake() {
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP] Waking up from deep sleep. LowPowerMode: MEDIUM_LOWPOWER");
#endif
#ifdef SUPPORT_BLE
    restoreBLEWakeSettingsFromRTC();
#endif
    initBattery();
    batteryLoop();
    if (handleLowPowerSensors()) {
        displayFromDeepSleep(deepSleepData.cyclesLeftToRedrawDisplay == 0);
    }
    handleBLEOnWake();
    handleDisplayRedrawOnWake();
    handleDisplayOnWake();
    handleWiFiConnectionOnWake();
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP] CO2: " + String(co2) + " CO2temp: " + String(temp) + " CO2humi: " + String(hum));
#endif
    handleMQTTPublishOnWake();
}

void fromDeepSleepTimer() {
    handleCycleCountersOnWake();

    if (deepSleepData.lowPowerMode != HIGH_PERFORMANCE) {
        handleMediumLowPowerModeOnWake();
    }

    Serial.flush();
}

void handleWakeupCauseOnWake(esp_sleep_wakeup_cause_t wakeupCause) {
    switch (wakeupCause) {
        case ESP_SLEEP_WAKEUP_TIMER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by timer");
#endif
            fromDeepSleepTimer();
#if defined(SUPPORT_TFT)
            // For TFT displays, explicitly turn off the display before deep sleep.
            // turnOffDisplay() shuts down the backlight IC; TFT_POWER_ON_BATTERY is
            // then driven LOW inside toDeepSleep() to cut the display power rail.
            turnOffDisplay();
            delay(10);
#endif
#if defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
            Serial.println("-->[DEEP] Turn display off before going to deep sleep *");
            delay(10);
            displaySleep(true);
#endif
            toDeepSleep();
            break;
        case ESP_SLEEP_WAKEUP_EXT0:
        case ESP_SLEEP_WAKEUP_EXT1:
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by external signal");
#endif
            interactiveMode = true;
            break;
        case ESP_SLEEP_WAKEUP_ULP:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by ULP program");
#endif
            break;
        default:
            Serial.println("-->[DEEP] Wakeup was not caused by deep sleep. Reason: " + String(wakeupCause));
            break;
    }
}

void fromDeepSleep() {
    esp_sleep_wakeup_cause_t wakeupCause = esp_sleep_get_wakeup_cause();
#ifdef DEEP_SLEEP_DEBUG
    printRTCMemoryExit();
    Serial.println("-->[STUP] Initializing from deep sleep mode working with sensor (" + String(deepSleepData.co2Sensor) + "): " + getDeepSleepDataCo2SensorName());
    Serial.println();
#endif
    handleDisplayReverseOnWake();
    handleWakeupCauseOnWake(wakeupCause);
}

void deepSleepLoop() {
    // Variable to store the last time Serial.print was called
    static unsigned long lastSerialPrintTime = 0;
    static unsigned long lastDotPrintTime = 0;

    // if (deepSleepData.lowPowerMode == HIGH_PERFORMANCE) return;
    if (!deepSleepEnabled) return;

#ifdef DEEP_SLEEP_DEBUG
        // Serial.println("-->[DEEP] inMenu: " + String(inMenu));
#endif

#ifdef DEEP_SLEEP_DEBUG
    if (millis() - lastSerialPrintTime >= 5000) {  // Check if some time has passed since the last print
        if (!inMenu) {
            // Serial.println("-->[DEEP] deepSleepData.waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot * 1000));
            // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep));

            Serial.println("-->[DEEP] Waiting to go to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 1000) + " seconds");
            // Serial.println("-->[DEEP] startTimerToDeepSleep (ms): " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot (ms): " + String(deepSleepData.waitToGoDeepSleepOn1stBoot * 1000) + " Now: " + String(millis()));
            lastSerialPrintTime = millis();  // Update last print time
        }
    }

    // if ((millis() - lastDotPrintTime >= 1000)) {
    //     Serial.print(".");            // Print a dot from time to time to show that the device is alive
    //     lastDotPrintTime = millis();  // Update last print time
    // }
#endif

    if (inMenu) {
        restartTimerToDeepSleep();  // If user enters the menu, reset the timer to go to deep sleep while it's on menu
#ifdef DEEP_SLEEP_DEBUG
        // Check if enough time has passed since the last print
        if (millis() - lastSerialPrintTime >= 5000) {
            // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot (ms): " + String(deepSleepData.waitToGoDeepSleepOn1stBoot)*1000 + "Now: " + String(millis()));
            Serial.println("-->[DEEP] (inMenu=TRUE) Waiting to go to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 5000) + " seconds");
            lastSerialPrintTime = millis();  // Update last print time
        }
        // if ((millis() - lastDotPrintTime >= 1000)) {
        //     Serial.print(".");            // Print a dot every loop to show that the device is alive
        //     lastDotPrintTime = millis();  // Update last print time
#endif
    } else {
#ifdef DEEP_SLEEP_DEBUG
        // Check if enough time has passed since the last print
        if (millis() - lastSerialPrintTime >= 5000) {
            // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + "Now: " + String(millis()));
            Serial.println("-->[DEEP] (inMenu=FALSE) Waiting to go to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 5000) + " seconds");
            lastSerialPrintTime = millis();  // Update last print time
        }
#endif

        if (millis() - startTimerToDeepSleep >= deepSleepData.waitToGoDeepSleepOn1stBoot * 1000) {
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
#if defined(SUPPORT_TFT)
            turnOffDisplay();
            Serial.println("-->[DEEP] Display off");
#endif
            Serial.println("-->[DEEP] Display off before going to deep sleep");
            delay(20);
            displaySleep(true);
#endif
            // deepSleepData.lowPowerMode = MEDIUM_LOWPOWER;
            toDeepSleep();
        }
    }
    delay(20);  // Give time to Serial.print
}

#endif  // CO2_Gadget_DeepSleep_h
