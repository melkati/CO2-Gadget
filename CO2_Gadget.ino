// Functions and enum definitions
void reverseButtons(bool reversed);
void outputsLoop();
void publishMQTTLogData(String logData);
void putPreferences();
String getLowPowerModeName(uint16_t mode);
void restartTimerToDeepSleep();

// Define enum for toneBuzzerBeep
enum ToneBuzzerBeep {
    BUZZER_TONE_LOW = 300,
    BUZZER_TONE_MED = 1000,
    BUZZER_TONE_HIGH = 2000
};

// Define enum for durationBuzzerBeep
enum DurationBuzzerBeep {
    DURATION_BEEP_SHORT = 50,
    DURATION_BEEP_MEDIUM = 150,
    DURATION_BEEP_LONG = 300
};

// Next data always defined to be able to configure in menu
String hostName = UNITHOSTNAME;
String rootTopic = UNITHOSTNAME;
String discoveryTopic = MQTT_DISCOVERY_PREFIX;
String mqttClientId = UNITHOSTNAME;
String mqttBroker = MQTT_BROKER_SERVER;
String mqttUser = "";
String mqttPass = "";
String wifiSSID = WIFI_SSID_CREDENTIALS;
String wifiPass = WIFI_PW_CREDENTIALS;
String MACAddress = "Unset";
uint8_t peerESPNowAddress[] = ESPNOW_PEER_MAC_ADDRESS;

// Communication options
bool activeBLE = true;
bool activeWIFI = true;
bool activeMQTT = true;
bool activeESPNOW = false;
bool activeOTA = false;
bool troubledWIFI = false;               // There are problems connecting to WIFI. Temporary suspend WIFI
bool troubledMQTT = false;               // There are problems connecting to MQTT. Temporary suspend MQTT
bool troubledESPNOW = false;             // There are problems connecting to ESP-NOW. Temporary suspend ESP-NOW
uint64_t timeTroubledWIFI = 0;           // Time since WIFI is troubled
uint64_t timeTroubledMQTT = 0;           // Time since MQTT is troubled
uint64_t timeToRetryTroubledWIFI = 300;  // Time in seconds to retry WIFI connection after it is troubled
uint64_t timeToRetryTroubledMQTT = 900;  // Time in seconds to retry MQTT connection after it is troubled (no need to retry so often as it retries automatically after WiFi is connected)
uint16_t WiFiConnectionRetries = 0;
uint16_t maxWiFiConnectionRetries = 20;
bool mqttDiscoverySent = false;
bool wifiChanged = false;

// Display and menu options
uint16_t DisplayBrightness = 100;
bool displayReverse = false;
bool showFahrenheit = false;
bool displayShowTemperature = true;
bool displayShowHumidity = true;
bool displayShowBattery = true;
bool displayShowBatteryVoltage = false;
bool displayShowCO2 = true;
bool displayShowPM25 = true;
bool debugSensors = false;
bool inMenu = false;
bool shouldWakeUpDisplay = false;
bool shouldRedrawDisplay = false;
uint16_t measurementInterval = 10;
bool bleInitialized = false;
int8_t selectedCO2Sensor = -1;
bool outputsModeRelay = false;

// Variables for buzzer functionality
bool buzzerBeeping = false;
uint16_t toneBuzzerBeep = BUZZER_TONE_MED;
uint16_t durationBuzzerBeep = DURATION_BEEP_MEDIUM;
int16_t timeBetweenBuzzerBeeps = -1;

uint8_t channelESPNow = 1;
uint16_t boardIdESPNow = 0;
uint64_t timeInitializationCompleted = 0;

// Variables for Battery reading
float batteryVoltage = 0;
uint8_t batteryLevel = 100;
uint16_t vRef = 960;
uint16_t batteryDischargedMillivolts = 3200;    // Voltage of battery when we consider it discharged (0%).
uint16_t batteryFullyChargedMillivolts = 4200;  // Voltage of battery when it is considered fully charged (100%).

// Variables to control automatic display off to save power
bool workingOnExternalPower = true;    // True if working on external power (USB connected)
uint32_t actualDisplayBrightness = 0;  // To know if it's on or off
bool displayOffOnExternalPower = false;
uint16_t timeToDisplayOff = 0;                // Time in seconds to turn off the display to save power.
volatile uint64_t lastTimeButtonPressed = 0;  // Last time stamp button up was pressed

// Variables for MQTT timming
uint16_t timeBetweenMQTTPublish = 60;  // Time in seconds between MQTT transmissions
uint16_t timeToKeepAliveMQTT = 3600;   // Maximum time in seconds between MQTT transmissions - Default: 1 Hour
uint64_t lastTimeMQTTPublished = 0;    // Time of last MQTT transmission

// Variables for ESP-NOW timming
uint16_t timeBetweenESPNowPublish = 60;  // Time in seconds between ESP-NOW transmissions
uint16_t timeToKeepAliveESPNow = 3600;   // Maximum time in seconds between ESP-NOW transmissions - Default: 1 Hour
uint64_t lastTimeESPNowPublished = 0;    // Time of last ESP-NOW transmission

// Variables for color and output ranges
uint16_t co2OrangeRange = 700;
uint16_t co2RedRange = 1000;

// Variables for Improv-Serial
uint16_t timeToWaitForImprov = 5;  // Time in seconds to wait for improv serial

// Variables for deep sleep
bool interactiveMode = false;
bool deepSleepEnabled = false;
uint64_t startTimerToDeepSleep = 0;
uint64_t lastTimeDeepSleep = 0;

// Define enum for sensors
typedef enum {
    CO2Sensor_NONE = -1,
    CO2Sensor_SCD30 = 1,
    CO2Sensor_SCD40 = 2,
    CO2Sensor_SCD41 = 3,
    CO2Sensor_MHZ19 = 4,
    CO2Sensor_CM1106 = 5,
    CO2Sensor_CM1106SL_NS = 6,
    CO2Sensor_SENSEAIRS8 = 7,
    CO2Sensor_DEMO = 127
} CO2SENSORS_t;

// LOW POWER MODES
// typedef enum LowPowerMode { HIGH_PERFORMANCE, BASIC_LOWPOWER, MEDIUM_LOWPOWER, MAXIMUM_LOWPOWER };

typedef struct {
    uint16_t lowPowerMode;  // 0 = No low power, 1 = Basic low power, 2 = Medium low power, 3 = Maximum low power
    CO2SENSORS_t co2Sensor;
    uint16_t waitToGoDeepSleepOn1stBoot;  // Give an opportunity to user to interact with the device before going to deep sleep
    uint16_t timeSleeping;
    uint32_t gpioConfig;
    bool waitingForDataReady;
    uint16_t activateWiFiEvery;         // Connect to WiFi every X deep sleep cycles (0 to disable)
    uint16_t redrawDisplayEveryCycles;  // Redraw display every X deep sleep cycles (0 to disable)
    uint16_t cyclesLeftToWiFiConnect;
    uint16_t cyclesLeftToRedrawDisplay;
    uint16_t lastCO2Value;
    float lastTemperatureValue;
    float lastHumidityValue;
    bool activeBLEOnWake = true;
    bool activeWifiOnWake;
    bool sendMQTTOnWake;
    bool sendESPNowOnWake;
    bool displayOnWake;
    uint16_t timeToDisplayOnWake = 3;
    bool measurementsStarted;
} deepSleepData_t;

RTC_DATA_ATTR deepSleepData_t deepSleepData;

#ifdef BUILD_GIT
#undef BUILD_GIT
#endif  // ifdef BUILD_GIT
#define BUILD_GIT __DATE__

#ifdef CUSTOM_I2C_SDA
#undef I2C_SDA
#define I2C_SDA CUSTOM_I2C_SDA
#endif
#ifdef CUSTOM_I2C_SCL
#undef I2C_SCL
#define I2C_SCL CUSTOM_I2C_SCL
#endif

#include <WiFi.h>
#include <Wire.h>

#include "driver/adc.h"
#include "soc/rtc_cntl_reg.h"  // disable brownout problems
#include "soc/soc.h"           // disable brownout problems

#ifdef SUPPORT_MDNS
#include <ESPmDNS.h>
#endif
// #include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "AsyncJson.h"
#ifdef SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif
#include <FS.h>
#include <SPIFFS.h>

#ifdef TIMEDEBUG
#include "Timer.h"
Timer timer;
Timer timerAwake;
Timer timerLightSleep;
#endif

// Stream& miSerialPort = Serial;

enum notificationTypes { notifyNothing,
                         notifyInfo,
                         notifyWarning,
                         notifyError };
bool displayNotification(String notificationText, notificationTypes notificationType);
bool displayNotification(String notificationText, String notificationText2, notificationTypes notificationType);
#if (!SUPPORT_OLED && !SUPPORT_TFT && !SUPPORT_EINK)
bool displayNotification(String notificationText, String notificationText2, notificationTypes notificationType) { return true; }
bool displayNotification(String notificationText, notificationTypes notificationType) { return true; }
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP SENSORS                                   *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include <CO2_Gadget_Sensors.h>

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           INCLUDE BATTERY FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Battery.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********               SETUP NEOPIXEL (ES2812b AND OTHERS) LED FUNCTIONALITY               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Neopixel.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********        FUNCTIONALITY TO STORE PREFERENCES IN NON VOLATILE MEMORY                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Preferences.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           INCLUDE WIFI FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include <CO2_Gadget_WIFI.h>

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                         INCLUDE IMPROV FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Improv.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                         INCLUDE ESP-NOW FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include <CO2_Gadget_ESP-NOW.h>

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_MQTT.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP BLE FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_BLE.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP OTA FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#ifdef SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********               INCLUDE EINK DISPLAY FUNCTIONALITY (UNFINISHED WIP)                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_EINK
#include <CO2_Gadget_EINK.h>
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********               INCLUDE OLED DISPLAY FUNCTIONALITY (UNFINISHED WIP)                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_OLED
#include <CO2_Gadget_OLED.h>
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                      INCLUDE TFT DISPLAY FUNCTIONALITY                            *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_TFT
#include "CO2_Gadget_TFT.h"
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                       INCLUDE BUZZER FUNCIONALITY                                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Buzzer.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                         INCLUDE MENU FUNCIONALITY                                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Menu.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                      SETUP PUSH BUTTONS FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "Arduino.h"
#include "CO2_Gadget_Buttons.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                       INCLUDE LOW POWER FUNCIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include <CO2_Gadget_DeepSleep.h>

/*****************************************************************************************************/

static int64_t lastReadingsCommunicationTime = 0;
static int startCheckingAfterUs = 1900000;

void wakeUpDisplay() {
    if (actualDisplayBrightness == 0) {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
        setDisplayBrightness(DisplayBrightness);
        // publishMQTTLogData("Display woken up. Setting display brightness to " + String(DisplayBrightness));
        // Serial.println("-->[MAIN] Display woken up. Setting display brightness to " + String(DisplayBrightness));
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}

void processPendingCommands() {
    if (pendingCalibration == true) {
        if ((calibrationValue >= 0) && (calibrationValue <= 2000)) {
            Serial.println("-->[MAIN] Calibrating CO2 sensor at " + String(calibrationValue) + " PPM");
            // Serial.printf("-->[MAIN] Calibrating CO2 sensor at %d PPM\n", calibrationValue);
            pendingCalibration = false;
            sensors.setCO2RecalibrationFactor(calibrationValue);
        } else {
            Serial.println("-->[MAIN] Avoiding calibrating CO2 sensor with invalid value at " + String(calibrationValue) + " PPM");
            // Serial.printf("-->[MAIN] Avoiding calibrating CO2 sensor with invalid value at %d PPM\n", calibrationValue);
            pendingCalibration = false;
        }
    }

    if (pendingAmbientPressure == true) {
        if (ambientPressureValue != 0) {
            Serial.printf("-->[MAIN] Setting AmbientPressure for CO2 sensor at %d mbar\n", ambientPressureValue);
            pendingAmbientPressure = false;
            // sensors.scd30.setAmbientPressure(ambientPressureValue); To-Do: Implement after migration to sensorlib 0.7.3
        } else {
            Serial.printf(
                "-->[MAIN] Avoiding setting AmbientPressure for CO2 sensor with invalid "
                "value at %d mbar\n",
                ambientPressureValue);
            pendingAmbientPressure = false;
        }
    }
}

void initGPIO() {
#ifdef GREEN_PIN
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(GREEN_PIN, LOW);
#endif
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(BLUE_PIN, LOW);
    pinMode(RED_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
    // If BTN_WAKEUP is defined and BTN_WAKEUP_IS_TOUCHPAD is not defined or set to 0, set it as input
#if defined(BTN_WAKEUP) && (!defined(BTN_WAKEUP_IS_TOUCHPAD) || BTN_WAKEUP_IS_TOUCHPAD == 0)
    pinMode(BTN_WAKEUP, INPUT_PULLUP);
#endif
}

void outputsRelays() {
    if ((!outputsModeRelay) || (co2 == 0)) return;  // Don't turn on relays until there is CO2 Data
#ifdef GREEN_PIN
    if (co2 >= co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
    }
    if (co2 < co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
    }
#endif
    if (co2 >= co2OrangeRange) {
        digitalWrite(BLUE_PIN, BLUE_PIN_HIGH);
    }
    if (co2 < co2OrangeRange - PIN_HYSTERESIS) {
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
    }
    if (co2 > co2RedRange) {
        digitalWrite(RED_PIN, RED_PIN_HIGH);
    }
    if (co2 <= co2RedRange - PIN_HYSTERESIS) {
        digitalWrite(RED_PIN, RED_PIN_LOW);
    }
}

void outputsRGBLeds() {
    if ((outputsModeRelay) || (co2 == 0)) return;  // Don't turn on led until there is CO2 Data
    if (co2 > co2RedRange) {
#ifdef GREEN_PIN
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
#endif
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        return;
    }
    if (co2 >= co2OrangeRange) {
#ifdef GREEN_PIN
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
#endif
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        return;
    }
#ifdef GREEN_PIN
    digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
#endif
    digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
    digitalWrite(RED_PIN, RED_PIN_LOW);
}

void outputsLoop() {
    outputsRelays();
    outputsRGBLeds();
    neopixelLoop();
    buzzerLoop();
}

void readingsLoop() {
    if (esp_timer_get_time() - lastReadingsCommunicationTime >= startCheckingAfterUs) {
        if (newReadingsAvailable) {
            lastReadingsCommunicationTime = esp_timer_get_time();
            newReadingsAvailable = false;
            nav.idleChanged = true;  // Must redraw display as there are new readings
#ifdef SUPPORT_BLE
            publishBLE();
#endif
#ifdef SUPPORT_MQTT
            publishMQTT();
#endif
#ifdef SUPPORT_ESPNOW
            publishESPNow();
#endif
        }
    }
}

void adjustBrightnessLoop() {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)

    if (shouldWakeUpDisplay) {
        wakeUpDisplay();
        shouldWakeUpDisplay = false;
    }

    // If battery pin not connected, assume it's working on external power
    if (batteryVoltage < 1) {
        workingOnExternalPower = true;
    }

    if (inMenu) {
        setDisplayBrightness(DisplayBrightness);
        return;
    }

    // Display backlight IS sleeping
    if ((actualDisplayBrightness == 0) && (actualDisplayBrightness != DisplayBrightness)) {
        if ((!displayOffOnExternalPower) && (workingOnExternalPower)) {
            setDisplayBrightness(DisplayBrightness);
        }
        if (timeToDisplayOff == 0) {
            setDisplayBrightness(DisplayBrightness);
        }
        return;
    }

    // Display backlight is NOT sleeping and brightness change detected
    if ((actualDisplayBrightness > 0) && (actualDisplayBrightness != DisplayBrightness)) {
        setDisplayBrightness(DisplayBrightness);
        publishMQTTLogData("Setting display brightness to " + String(DisplayBrightness));
    }

    // If configured not to turn off the display on external power and it's working on external power, do nothing and return (except if DisplayBrightness is 0, then turn on display))
    if ((workingOnExternalPower) && (!displayOffOnExternalPower)) {
        if (actualDisplayBrightness == 0) {
            setDisplayBrightness(DisplayBrightness);  // Exception: When USB connected (just connected) & TFT is OFF -> Turn Display ON
            // publishMQTTLogData("Turning on display on external power. Actual brightness: " + String(actualDisplayBrightness));
            // Serial.println("-->[MAIN] Turning on display on external power. Actual brightness: " + String(actualDisplayBrightness));
            // delay(10);
        }
        return;
    }

    if (timeToDisplayOff == 0) return;  // If timeToDisplayOff is 0, don't turn off the display

    if ((actualDisplayBrightness != 0) && (millis() - lastTimeButtonPressed >= timeToDisplayOff * 1000) && DisplayBrightness > 0) {
        if ((workingOnExternalPower) && (displayOffOnExternalPower)) {
            Serial.println("-->[MAIN] Turning off display on external power to save power. Actual brightness: " + String(actualDisplayBrightness));
            turnOffDisplay();
            // publishMQTTLogData("[MAIN] Turning off display on external power to save power. Actual brightness: " + String(actualDisplayBrightness));
        }
        if (!workingOnExternalPower) {
            // Serial.println("-->[MAIN] Turning off display on battery to save power. Actual brightness: " + String(actualDisplayBrightness));
            turnOffDisplay();
            // publishMQTTLogData("[MAIN] Turning off display on battery to save power. Actual brightness: " + String(actualDisplayBrightness));
            delay(10);
        }
    }
#endif
}

void setCpuFrequencyAndReinitSerial(int16_t newCpuFrequency) {
    while (Serial.available()) {
        Serial.read();
    }
    delay(100);  // time to write all data to serial
#if defined(CONFIG_IDF_TARGET_ESP32)
    Serial.end();
    setCpuFrequencyMhz(newCpuFrequency);
    Serial.begin(115200);
#endif
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    setCpuFrequencyMhz(newCpuFrequency);
#endif
}

void utilityLoop() {
    int16_t actualCPUFrequency = getCpuFrequencyMhz();
    const int16_t highCpuFrequency = 240;
    const int16_t lowCpuFrequency = 80;

    if (workingOnExternalPower && actualCPUFrequency != highCpuFrequency) {
        Serial.println("-->[BATT] Battery voltage: " + String(batteryVoltage) + "V. Increasing CPU frequency to " + String(highCpuFrequency) + "MHz");
        // Serial.printf("-->[BATT] Battery voltage: %.2fV. Increasing CPU frequency to %dMHz\n", batteryVoltage, highCpuFrequency);
        setCpuFrequencyAndReinitSerial(highCpuFrequency);
    } else if (!workingOnExternalPower && actualCPUFrequency != lowCpuFrequency) {
        Serial.println("-->[BATT] Battery voltage: " + String(batteryVoltage) + "V. Decreasing CPU frequency to " + String(lowCpuFrequency) + "MHz");
        // Serial.printf("-->[BATT] Battery voltage: %.2fV. Decreasing CPU frequency to %dMHz\n", batteryVoltage, lowCpuFrequency);
        setCpuFrequencyAndReinitSerial(lowCpuFrequency);
    }
}

void initHighPerformanceMode() {
    Serial.println("");
    Serial.println("-->**********************************************");
    Serial.println("-->[STUP]--> INITIALIZING HIGH PERFORMANCE MODE *");
    Serial.println("-->**********************************************");
    Serial.println("");
#ifdef AUTO_VERSION
    Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s (Git HEAD: %s)\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR, AUTO_VERSION);
#else
    Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
#endif
    Serial.println("-->[STUP] Version compiled: " __DATE__ " at " __TIME__);
    Serial.println("-->[STUP] Total heap: " + String(ESP.getHeapSize()));
    Serial.println("-->[STUP] Free heap: " + String(ESP.getFreeHeap()));
    if (ESP.getPsramSize() > 0) {
        Serial.println("-->[STUP] Total PSRAM: " + String(ESP.getPsramSize()));
        Serial.println("-->[STUP] Free PSRAM: " + String(ESP.getFreePsram()));
    } else {
        Serial.println("-->[STUP] No PSRAM available");
    }

    if (ESP.getFlashChipSize() > 0) {
        Serial.println("-->[STUP] Flash size: " + String(ESP.getFlashChipSize()));
        // Serial.println("-->[STUP] Flash speed: " + String(ESP.getFlashChipSpeed()));
        // Serial.println("-->[STUP] Flash mode: " + String(ESP.getFlashChipMode()));
    }
    delay(50);

    // printResetReason();
    initImprov();
    initBattery();
    initGPIO();
    initNeopixel();
    initBuzzer();
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
    initDisplay(false);
#endif
#ifdef SUPPORT_BLE
    initBLE();
#endif
    initSensors();
    initWifi();
    wifiChanged = false;
#ifdef SUPPORT_ESPNOW
    initESPNow();
#endif
#ifdef SUPPORT_MQTT
    initMQTT();
#endif
    menu_init();
    initButtons();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        printLargeASCII(WiFi.localIP().toString().c_str());
        Serial.println("");
    }
    timeInitializationCompleted = millis();
    startTimerToDeepSleep = timeInitializationCompleted;
    Serial.println("-->[STUP] Initialization in HIGH PERFORMANCE MODE Ready.");
    Serial.println("");
    Serial.flush();
}

void initGPIOLowPower() {
    Serial.println("");
    Serial.println("-->****************************************************");
    Serial.println("-->[STUP]--> INITIALIZING LOW POWER MODE GPIO WAKE UP *");
    Serial.println("-->****************************************************");
    Serial.println("");
    co2 = deepSleepData.lastCO2Value;
    temp = deepSleepData.lastTemperatureValue;
    hum = deepSleepData.lastHumidityValue;
    interactiveMode = true;
    deepSleepEnabled = true;
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
    initDisplay(true);
#endif
    initBattery();
    initGPIO();
#ifdef SUPPORT_BLE
    initBLE();
#endif
    // initSensors();
    if (deepSleepData.activeWifiOnWake) {
        doDeepSleepWiFiConnect();
    }
    wifiChanged = false;
#ifdef SUPPORT_ESPNOW
    initESPNow();
#endif
#ifdef SUPPORT_MQTT
    initMQTT();
#endif
    initButtons();
    menu_init();
    timeInitializationCompleted = millis();
    restartTimerToDeepSleep();
    Serial.println("-->[STUP] Going to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 1000) + " seconds");
    Serial.println("-->[STUP] deepSleepData.waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot * 1000) + " startTimerToDeepSleep: " + String(startTimerToDeepSleep) + " millis: " + String(millis()));
    Serial.println("-->**********************************************************");
    Serial.println("-->[STUP]--> INITIALIZING LOW POWER MODE GPIO WAKE UP READY *");
    Serial.println("-->**********************************************************");
    Serial.println("");
}

void deepSleepDirectly() {
    esp_sleep_enable_timer_wakeup(10 * 1000000);
    delay(4000);
    // gpio_deep_sleep_hold_en();
    // adc_oneshot_del_unit(adc_handle); // TO-DO: Check if this is needed measuring current consumption in deep sleep
    // esp_wifi_stop();
    // esp_wifi_deinit();
    // btStop();

//   adc_power_off();
//   esp_wifi_stop();
#if defined(EINKBOARDGDEM029T94) || defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEW0213M21)
    // displaySleep(false);

    // Pull up pin 13 to put flash memory into deep sleep
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    gpio_hold_en(gpio_num_t(13));
#endif
    gpio_deep_sleep_hold_en();

    // esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);

    delay(1000);
    esp_deep_sleep_start();
}

void setup() {
    // deepSleepDirectly();
#ifdef TIMEDEBUG
    timerAwake.start();
    timerLightSleep.start();
    timerLightSleep.pause();
#endif
    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);  // save WatchDog register
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);                        // disable brownout detector
    Serial.setDebugOutput(true);
    Serial.setTxBufferSize(2048);
    Serial.setRxBufferSize(1024);
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.println("-->[STUP] millis(): " + String(millis()));
    Serial.println("-->[STUP] Reset reason: (" + String(esp_reset_reason()) + ") " + getResetReason());
    Serial.println("-->[STUP] Wakeup cause: (" + String(esp_sleep_get_wakeup_cause()) + ") " + getWakeupCause());
    Serial.println("-->[STUP] lowPowerMode mode (from RTC memory): (" + String(deepSleepData.lowPowerMode) + ") " + getLowPowerModeName(deepSleepData.lowPowerMode));

    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (deepSleepData.lowPowerMode != HIGH_PERFORMANCE)) {
        timeToWaitForImprov = 0;
        switch (esp_sleep_get_wakeup_cause()) {
            case ESP_SLEEP_WAKEUP_TIMER:
                Serial.println("-->[STUP] Initializing from deep sleep timer");
                fromDeepSleep();
                break;
            case ESP_SLEEP_WAKEUP_EXT0:
                Serial.println("-->[STUP] Initializing from deep sleep GPIO (WAKEUP_EXT0)");
                initGPIOLowPower();
                fromDeepSleep();
                Serial.println("-->[STUP] Initialization from deep sleep GPIO (WAKEUP_EXT0) completed");
                break;
            case ESP_SLEEP_WAKEUP_EXT1:
                Serial.println("-->[STUP] Initializing from deep sleep GPIO (WAKEUP_EXT1)");
                Serial.print("-->[STUP] Wake up caused because of GPIO: ");
                Serial.println((log(esp_sleep_get_ext1_wakeup_status())) / log(2), 0);
                initGPIOLowPower();
                fromDeepSleep();
                Serial.println("-->[STUP] Initialization from deep sleep GPIO (WAKEUP_EXT1) completed");
                break;
            case ESP_SLEEP_WAKEUP_TOUCHPAD:
                Serial.println("-->[STUP] Initializing from deep sleep touchpad");
                printWakeupTouchpad();
                initGPIOLowPower();
                fromDeepSleep();
                Serial.println("-->[STUP] Initialization from deep sleep touchpad completed");
                break;
            default:
                Serial.print("-->[STUP][ERROR] Initializing from unknown deep sleep cause: ");
                Serial.println(esp_sleep_get_wakeup_cause());
                delay(5000);
                initHighPerformanceMode();
                break;
        }
    } else {
        // Normal boot from any reason
        if ((esp_reset_reason() == ESP_RST_POWERON) || (esp_reset_reason() == ESP_RST_BROWNOUT) || (esp_reset_reason() == ESP_RST_SW) || (esp_reset_reason() == ESP_RST_PANIC) || (esp_reset_reason() == ESP_RST_INT_WDT) || (esp_reset_reason() == ESP_RST_TASK_WDT) || (esp_reset_reason() == ESP_RST_WDT)) {
            Serial.println("-->[STUP] Initializing from: " + getResetReason());
            initPreferences();
            if (deepSleepData.lowPowerMode == HIGH_PERFORMANCE) {
                Serial.println("-->[STUP] Will go into high performance mode after initialization");
                delay(10);
                deepSleepEnabled = false;
                // initHighPerformanceMode();
            } else {
                Serial.println("-->[STUP] Will go into low power mode " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + " secs after initialization");
                delay(10);
                interactiveMode = true;
                deepSleepEnabled = true;
                restartTimerToDeepSleep();
            }
            initHighPerformanceMode();
        } else {
            Serial.println("-->[STUP][ERROR] No mode defined. Reset reason: " + String(esp_reset_reason()));
            printResetReason();
            delay(5000);
            ESP.restart();
            while (1) {
                delay(10);
            }
        }
    }
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);  // enable brownout detector
}

void loopOLD() {  // Only reached in HIGH PERFORMANCE MODE
    batteryLoop();
    utilityLoop();
    improvLoop();
    wifiClientLoop();
    mqttClientLoop();
    sensorsLoop();
    outputsLoop();
    processPendingCommands();
    readingsLoop();
    OTALoop();
    adjustBrightnessLoop();
    buttonsLoop();
    menuLoop();
    BLELoop();
    deepSleepLoop();
}

void loop() {  // Old loop function. Not used anymore. Just for reference
    bool showDebug = true;
    static unsigned long lastDotPrintTime = 0;
    if (!inMenu && (showDebug) && (millis() - lastDotPrintTime > 3000)) {
        lastDotPrintTime = millis();
        if (!inMenu) {
            Serial.print("-->[MAIN] Looping (interactive mode: " + String(interactiveMode) + "). ");
            Serial.print("Low power mode: " + getLowPowerModeName(deepSleepData.lowPowerMode) + ". ");
            Serial.print("Deep sleep enabled: " + String(deepSleepEnabled) + ". ");
            Serial.print("Time to go into low power mode: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 1000) + " secs. ");
            Serial.println("...");
        }
    }

    batteryLoop();
    utilityLoop();
    improvLoop();
    wifiClientLoop();
    mqttClientLoop();
    if (deepSleepEnabled) {
        // if ((showDebug) && (!inMenu)) Serial.println("-->[MAIN] Reading sensors in interactive mode (will go into low power mode)");
        sensorsLoop();
        deepSleepLoop();
    } else {
        if ((showDebug) && (!inMenu)) Serial.println("-->[MAIN] Reading sensors with CO2 Gadget in high performance mode. ");
        sensorsLoop();
    }
    outputsLoop();
    processPendingCommands();
    readingsLoop();
    OTALoop();
    adjustBrightnessLoop();
    buttonsLoop();
    menuLoop();
    BLELoop();
}