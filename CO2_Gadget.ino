// Functions and enum definitions
void reverseButtons(bool reversed);
void outputsLoop();
void publishMQTTLogData(String logData);
void putPreferences();

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
bool workingOnExternalPower = true;      // True if working on external power (USB connected)
uint32_t actualDisplayBrightness = 100;  // To know if it's on or off
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
uint64_t waitToGoDeepSleepOnFirstBoot = 60000;  // Give an opportunity to user to interact with the device before going to deep sleep
bool deepSleepEnabled = true;
uint64_t startTimerToDeepSleep = 0;
uint64_t lastTimeDeepSleep = 0;
uint64_t timeBetweenDeepSleep = 60;
uint16_t deepSleepWiFiConnectEach = 5;  // Connect to WiFi each X deep sleep cycles (0 to disable)
uint16_t cyclesToRedrawDisplay = 5;     // Redraw display each X deep sleep cycles (0 to disable)

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

typedef struct {
    CO2SENSORS_t co2Sensor;
    uint16_t lowPowerMode;
    uint32_t gpioConfig;
    bool waitingForDataReady;
    uint16_t cyclesToWiFiConnect;
    uint16_t cyclesToRedrawDisplay;
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
/*********                      INCLUDE DEEP SLEEP FUNCIONALITY                              *********/
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
        publishMQTTLogData("Display woken up. Setting display brightness to " + String(DisplayBrightness));
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}

void processPendingCommands() {
    if (pendingCalibration == true) {
        if (calibrationValue != 0) {
            printf("-->[MAIN] Calibrating CO2 sensor at %d PPM\n", calibrationValue);
            pendingCalibration = false;
            sensors.setCO2RecalibrationFactor(calibrationValue);
        } else {
            printf("-->[MAIN] Avoiding calibrating CO2 sensor with invalid value at %d PPM\n", calibrationValue);
            pendingCalibration = false;
        }
    }

    if (pendingAmbientPressure == true) {
        if (ambientPressureValue != 0) {
            printf("-->[MAIN] Setting AmbientPressure for CO2 sensor at %d mbar\n", ambientPressureValue);
            pendingAmbientPressure = false;
            // sensors.scd30.setAmbientPressure(ambientPressureValue); To-Do: Implement after migration to sensorlib 0.7.3
        } else {
            printf(
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
#ifdef BTN_WAKEUP
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
            // Provide the sensor values for Tools -> Serial Monitor or Serial Plotter
            // Serial.printf("CO2[ppm]:%d\tTemperature[\u00B0C]:%.2f\tHumidity[%%]:%.2f\n", co2, temp, hum);
            // if ((!troubledWIFI) && (activeWIFI) && (WiFi.status() != WL_CONNECTED)) {
            //     Serial.println("-->[MAIN] WiFi not connected");
            // }
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
            publishMQTTLogData("Turning on display on external power. Actual brightness: " + String(actualDisplayBrightness));
        }
        return;
    }

    if (timeToDisplayOff == 0) return;  // If timeToDisplayOff is 0, don't turn off the display

    if ((actualDisplayBrightness != 0) && (millis() - lastTimeButtonPressed >= timeToDisplayOff * 1000) && DisplayBrightness > 0) {
        if ((workingOnExternalPower) && (displayOffOnExternalPower)) {
            Serial.println("-->[MAIN] Turning off display on external power to save power. Actual brightness: " + String(actualDisplayBrightness));
            turnOffDisplay();
            publishMQTTLogData("[MAIN] Turning off display on external power to save power. Actual brightness: " + String(actualDisplayBrightness));
        }
        if (!workingOnExternalPower) {
            Serial.println("-->[MAIN] Turning off display on battery to save power. Actual brightness: " + String(actualDisplayBrightness));
            turnOffDisplay();
            publishMQTTLogData("[MAIN] Turning off display on battery to save power. Actual brightness: " + String(actualDisplayBrightness));
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
        Serial.printf("-->[BATT] Battery voltage: %.2fV. Increasing CPU frequency to %dMHz\n", batteryVoltage, highCpuFrequency);
        setCpuFrequencyAndReinitSerial(highCpuFrequency);
    } else if (!workingOnExternalPower && actualCPUFrequency != lowCpuFrequency) {
        Serial.printf("-->[BATT] Battery voltage: %.2fV. Decreasing CPU frequency to %dMHz\n", batteryVoltage, lowCpuFrequency);
        setCpuFrequencyAndReinitSerial(lowCpuFrequency);
    }
}

// application entry point
void setup() {
#ifdef TIMEDEBUG
    timerAwake.start();
#endif
    // Manually set los power mode until implementation in preferences
    // Choose one of enum LowPowerMode NO_LOWPOWER, BASIC_LOWPOWER, MEDIUM_LOWPOWER, MAXIMUM_LOWPOWER
    sensors.setLowPowerMode(MEDIUM_LOWPOWER);
    deepSleepData.lowPowerMode = sensors.getLowPowerMode();

    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);  // save WatchDog register
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);                        // disable brownout detector
    Serial.begin(115200);
    // if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (sensors.getLowPowerMode() != NO_LOWPOWER)) {
        if (esp_reset_reason() == ESP_RST_DEEPSLEEP) {
        fromDeepSleep();
    } else {
        delay(50);
        resetReason();
#ifdef AUTO_VERSION
        Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s (Git HEAD: %s)\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR, AUTO_VERSION);
#else
        Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
#endif
        Serial.printf("-->[STUP] Version compiled: %s at %s\n", __DATE__, __TIME__);
        Serial.printf("-->[STUP] Total heap: %d\n", ESP.getHeapSize());
        Serial.printf("-->[STUP] Free heap: %d\n", ESP.getFreeHeap());
        Serial.printf("-->[STUP] Total PSRAM: %d\n", ESP.getPsramSize());
        Serial.printf("-->[STUP] Free PSRAM: %d\n", ESP.getFreePsram());

        if (ESP.getFlashChipSize() > 0) {
            Serial.printf("-->[STUP] Flash size: %d\n", ESP.getFlashChipSize());
            Serial.printf("-->[STUP] Flash speed: %d\n", ESP.getFlashChipSpeed());
            Serial.printf("-->[STUP] Flash mode: %d\n", ESP.getFlashChipMode());
        }

        Serial.printf("-->[STUP] Starting up...\n\n");

        initImprov();
        initPreferences();
        initBattery();
        initGPIO();
        initNeopixel();
        initBuzzer();
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
        initDisplay();
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
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);  // enable brownout detector
        Serial.println("-->[STUP] Ready.");
        timeInitializationCompleted = millis();
        startTimerToDeepSleep = millis();
        initDeepSleep();
        if (sensors.getLowPowerMode() == MEDIUM_LOWPOWER || sensors.getLowPowerMode() == MAXIMUM_LOWPOWER) {
            Serial.printf("-->[STUP] Going to deep sleep in: %d seconds\n", (waitToGoDeepSleepOnFirstBoot - (millis() - startTimerToDeepSleep)) / 1000);
        }
    }
}

void loop() {
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