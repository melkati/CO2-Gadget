/*****************************************************************************************************/

// ▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
// ▐       ____ ___ ____     ____           _            _        ▌
// ▐      / ___/ _ \___ \   / ___| __ _  __| | __ _  ___| |_      ▌
// ▐     | |  | | | |__) | | |  _ / _` |/ _` |/ _` |/ _ \ __|     ▌
// ▐     | |__| |_| / __/  | |_| | (_| | (_| | (_| |  __/ |_      ▌
// ▐      \____\___/_____|  \____|\__,_|\__,_|\__, |\___|\__|     ▌
// ▐                                          |___/               ▌
// ▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌

// CO2 Gadget Advanced Firmware
//
// Copyright (C) 2021-2024 Mariete & CO2 Gadget Contributors
// Contact: https://emariete.com
//
// This file is part of the CO2 Gadget firmware.
//
// The CO2 Gadget firmware is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// The emariete.com Website and Documentation is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with the emariete.com Website and Documentation. If not, see
// <http://www.gnu.org/licenses/>

// ▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
// ▐                                _      _                                 ▌
// ▐       ___ _ __ ___   __ _ _ __(_) ___| |_ ___   ___ ___  _ __ ___       ▌
// ▐      / _ \ '_ ` _ \ / _` | '__| |/ _ \ __/ _ \ / __/ _ \| '_ ` _ \      ▌
// ▐     |  __/ | | | | | (_| | |  | |  __/ ||  __/| (_| (_) | | | | | |     ▌
// ▐      \___|_| |_| |_|\__,_|_|  |_|\___|\__\___(_)___\___/|_| |_| |_|     ▌
// ▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌

/*****************************************************************************************************/

#define SUPPORT_CAPTIVE_PORTAL // Please, don't disable this.

// Functions and enum definitions
void reverseButtons(bool reversed);       // Defined in CO2_Gadget_Buttons.h
void outputsLoop();                       // Defined in CO2_Gadget_Main.h
void publishMQTTLogData(String logData);  // Defined in CO2_Gadget_MQTT.h
void putPreferences();                    // Defined in CO2_Gadget_Preferences.h
void menuLoop();                          // Defined in CO2_Gadget_Menu.h
void setDisplayReverse(bool reverse);     // Defined in CO2_Gadget_TFT.h or CO2_Gadget_OLED.h or CO2_Gadget_EINK.h
// void setBLEHistoryInterval(uint64_t interval);  // Defined in CO2_Gadget_BLE.h

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
String wifiSSID = "";
String wifiPass = "";
String MACAddress = "Unset";
uint8_t peerESPNowAddress[] = ESPNOW_PEER_MAC_ADDRESS;

// BLE options
bool activeBLE = true;
bool isDownloadingBLE = false;

// WIFI options
bool activeWIFI = true;
bool troubledWIFI = false;               // There are problems connecting to WIFI. Temporary suspend WIFI
uint64_t timeTroubledWIFI = 0;           // Time since WIFI is troubled
uint64_t timeToRetryTroubledWIFI = 300;  // Time in seconds to retry WIFI connection after it is troubled
uint16_t WiFiConnectionRetries = 0;
uint16_t maxWiFiConnectionRetries = 10;
bool wifiChanged = false;
bool useStaticIP = false;              // Set to true if you want to use a static IP
IPAddress staticIP(192, 168, 1, 199);  // Static IP address
IPAddress gateway(192, 168, 1, 1);     // Network gateway
IPAddress subnet(255, 255, 255, 0);    // Subnet mask
IPAddress dns1(8, 8, 8, 8);            // DNS server
IPAddress dns2(8, 8, 4, 4);            // DNS server

// MQTT options
bool activeMQTT = true;
bool troubledMQTT = false;               // There are problems connecting to MQTT. Temporary suspend MQTT
uint64_t timeTroubledMQTT = 0;           // Time since MQTT is troubled
uint64_t timeToRetryTroubledMQTT = 900;  // Time in seconds to retry MQTT connection after it is troubled (no need to retry so often as it retries automatically everytime WiFi is connected)
bool mqttDiscoverySent = false;
uint16_t timeBetweenMQTTPublish = 60;  // Time in seconds between MQTT transmissions
uint16_t timeToKeepAliveMQTT = 3600;   // Maximum time in seconds between MQTT transmissions - Default: 1 Hour
uint64_t lastTimeMQTTPublished = 0;    // Time of last MQTT transmission
bool mqttShowInConsole = false;

// ESP-NOW options
bool activeESPNOW = false;
bool troubledESPNOW = false;  // There are problems connecting to ESP-NOW. Temporary suspend ESP-NOW
uint8_t channelESPNow = 1;
uint16_t boardIdESPNow = 0;
uint16_t timeBetweenESPNowPublish = 60;  // Time in seconds between ESP-NOW transmissions
uint16_t timeToKeepAliveESPNow = 3600;   // Maximum time in seconds between ESP-NOW transmissions - Default: 1 Hour
uint64_t lastTimeESPNowPublished = 0;    // Time of last ESP-NOW transmission

// OTA options
#ifdef SUPPORT_OTA
bool activeOTA = true;
#else
bool activeOTA = false;
#endif

// Display and menu options
bool mustInitMenu = false;
bool menuInitialized = false;
uint16_t DisplayBrightness = 100;
bool displayOn = true;
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
bool isMenuDirty = false;  // To know if we need to redraw the menu
uint16_t measurementInterval = 10;
uint16_t sampleInterval = 60;
bool bleInitialized = false;
int8_t selectedCO2Sensor = -1;
bool outputsModeRelay = false;

// Variables for buzzer functionality
bool buzzerBeeping = false;
uint16_t toneBuzzerBeep = BUZZER_TONE_MED;
uint16_t durationBuzzerBeep = DURATION_BEEP_MEDIUM;
int16_t timeBetweenBuzzerBeeps = -1;

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

// Variables for color and output ranges
uint16_t co2OrangeRange = 700;
uint16_t co2RedRange = 1000;

// Variables for Improv-Serial
bool waitingForImprov = true;
uint16_t timeToWaitForImprov = 0;  // Time in seconds to wait for improv serial

// Variables for Captive Portal
#ifdef SUPPORT_CAPTIVE_PORTAL
bool captivePortalActive = false;
bool forceCaptivePortalActive = false;
bool captivePortalNoTimeout = false;
bool relaxedSecurity = false;
bool captivePortalDebug = false;
uint16_t timeToWaitForCaptivePortal = 60;  // Time in seconds to wait for captive portal
uint64_t timeCaptivePortalStarted = 0;
#endif

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
#ifdef SUPPORT_CAPTIVE_PORTAL
#include <DNSServer.h>
#endif
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

static int64_t lastReadingsCommunicationTime = 0;
static int startCheckingAfterUs = 1900000;

void wakeUpDisplay() {
    if (!displayOn) {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
        turnOnDisplay();
//        setDisplayBrightness(DisplayBrightness);
// publishMQTTLogData("Display woken up. Setting display brightness to " + String(DisplayBrightness));
// Serial.println("-->[MAIN] Display woken up. Setting display brightness to " + String(DisplayBrightness));
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}

void processPendingCommands() {
    if (isDownloadingBLE) return;
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
    if (isDownloadingBLE) return;
    outputsRelays();
    outputsRGBLeds();
    neopixelLoop();
    buzzerLoop();
}

void readingsLoop() {
    if (isDownloadingBLE) return;
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
    if (isDownloadingBLE) return;

    if (shouldWakeUpDisplay) {
        wakeUpDisplay();
        shouldWakeUpDisplay = false;
    }

    // If battery pin not connected, assume it's working on external power
    if (batteryVoltage < 1) {
        workingOnExternalPower = true;
    }

    if (inMenu) {
        wakeUpDisplay();
        //        setDisplayBrightness(DisplayBrightness);
        return;
    }

    // Display backlight IS sleeping
    //    if ((actualDisplayBrightness == 0) && (actualDisplayBrightness != DisplayBrightness)) {
    if (!displayOn) {
        if ((!displayOffOnExternalPower) && (workingOnExternalPower)) {
            wakeUpDisplay();
            //            setDisplayBrightness(DisplayBrightness);
        }
        if (timeToDisplayOff == 0) {
            wakeUpDisplay();
            //            setDisplayBrightness(DisplayBrightness);
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
        wakeUpDisplay();
        //  if (actualDisplayBrightness == 0) {
        // setDisplayBrightness(DisplayBrightness);  // Exception: When USB connected (just connected) & TFT is OFF -> Turn Display ON
        // publishMQTTLogData("Turning on display on external power. Actual brightness: " + String(actualDisplayBrightness));
        // Serial.println("-->[MAIN] Turning on display on external power. Actual brightness: " + String(actualDisplayBrightness));
        // delay(10);
        //}
        return;
    }

    if (timeToDisplayOff == 0) return;  // If timeToDisplayOff is 0, don't turn off the display

    if (displayOn && (millis() - lastTimeButtonPressed >= timeToDisplayOff * 1000) && DisplayBrightness > 0) {
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
    if (isDownloadingBLE) return;
    int16_t actualCPUFrequency = getCpuFrequencyMhz();
    const int16_t highCpuFrequency = 240;
    const int16_t lowCpuFrequency = 80;

    if (workingOnExternalPower && actualCPUFrequency != highCpuFrequency) {
        setCpuFrequencyMhz(highCpuFrequency);
    } else if (!workingOnExternalPower && actualCPUFrequency != lowCpuFrequency) {
        setCpuFrequencyMhz(lowCpuFrequency);
    }

    // if (workingOnExternalPower && actualCPUFrequency != highCpuFrequency) {
    //     Serial.printf("-->[BATT] Battery voltage: %.2fV. Increasing CPU frequency to %dMHz\n", batteryVoltage, highCpuFrequency);
    //     setCpuFrequencyAndReinitSerial(highCpuFrequency);
    // } else if (!workingOnExternalPower && actualCPUFrequency != lowCpuFrequency) {
    //     Serial.printf("-->[BATT] Battery voltage: %.2fV. Decreasing CPU frequency to %dMHz\n", batteryVoltage, lowCpuFrequency);
    //     setCpuFrequencyAndReinitSerial(lowCpuFrequency);
    // }
}

// application entry point
void setup() {
    uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);  // save WatchDog register
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);                        // disable brownout detector
    Serial.setDebugOutput(true);
    Serial.setTxBufferSize(1024);
    Serial.setRxBufferSize(512);
    Serial.begin(115200);
    delay(50);
#ifdef AUTO_VERSION
    Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s (Git HEAD: %s)\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR, AUTO_VERSION);
#else
    Serial.printf("\n-->[STUP] CO2 Gadget Version: %s%s Flavour: %s\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
#endif

    Serial.println("-->[STUP] Version compiled: " __DATE__ " at " __TIME__);
    Serial.println("-->[STUP] Total heap: " + String(ESP.getHeapSize()));
    Serial.println("-->[STUP] Free heap: " + String(ESP.getFreeHeap()));
    Serial.println("-->[STUP] Min Free heap: " + String(ESP.getMinFreeHeap()));
    if (ESP.getPsramSize() > 0) {
        Serial.println("-->[STUP] Total PSRAM: " + String(ESP.getPsramSize()));
        Serial.println("-->[STUP] Free PSRAM: " + String(ESP.getFreePsram()));
    } else {
        Serial.println("-->[STUP] No PSRAM available");
    }

    // Get the size of the flash memory
    // Serial.printf("-->[STUP] Flash size: %d\n", ESP.getFlashChipSize());
    // Serial.printf("-->[STUP] Flash speed: %d\n", ESP.getFlashChipSpeed());
    // Serial.printf("-->[STUP] Flash mode: %d\n", ESP.getFlashChipMode());

    Serial.printf("-->[STUP] Starting up...\n\n");

    initPreferences();
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
    initButtons();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        printLargeASCII(WiFi.localIP().toString().c_str());
        Serial.println("");
    } else {
#ifdef SUPPORT_CAPTIVE_PORTAL
        initCaptivePortal();
#endif
    }
    initImprov();
    if (timeToWaitForImprov > 0) {
        Serial.println("-->[STUP] Waiting for Improv Serial " + String(timeToWaitForImprov) + " seconds...");
    } else {
        Serial.println("-->[STUP] Improv Serial enabled");
    }
    Serial.println("-->[STUP] Ready.");
    Serial.flush();
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);  // enable brownout detector
    timeInitializationCompleted = millis();
}

void loop() {
    batteryLoop();
    // utilityLoop();
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
}