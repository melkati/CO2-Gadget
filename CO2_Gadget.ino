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

#define SUPPORT_CAPTIVE_PORTAL  // Please, don't disable this.

// Functions and enum definitions
void reverseButtons(bool reversed);                 // Defined in CO2_Gadget_Buttons.h
void outputsLoop();                                 // Defined in CO2_Gadget_Outputs.h
void publishMQTTLogData(String logData);            // Defined in CO2_Gadget_MQTT.h
void putPreferences();                              // Defined in CO2_Gadget_Preferences.h
void menuLoop();                                    // Defined in CO2_Gadget_Menu.h
void setBLEHistoryInterval(uint64_t interval);      // Defined in CO2_Gadget_BLE.h
void refreshBLEOutputs(const char* reason, bool forcePublish = true);  // Defined in CO2_Gadget_BLE.h
void refreshBTHomeBLESettings(const char* reason, bool forcePublish = true);  // Defined in CO2_Gadget_BLE.h
#ifdef SUPPORT_BTHOME_BLE
uint32_t getBTHomeCounterNVSValue();                // Defined in CO2_Gadget_BTHome.h
#endif
String getLowPowerModeName(uint16_t mode);          // Defined in CO2_Gadget_DeepSleep.h
uint64_t getReliableUptimeSeconds();                // Accumulated uptime across deep sleep cycles
String getReliableUptimeFormatted();                // Accumulated uptime formatted as <dd>d <hh>h <mm>m
void restartTimerToDeepSleep();                     // Defined in CO2_Gadget_DeepSleep.h
void toDeepSleep();                                 // Defined in CO2_Gadget_DeepSleep.h
void processPendingCommands();                      // Defined below; called from CO2_Gadget_DeepSleep.h wake path
void beginCalibrationSequence(uint16_t ppm);        // Defined below; starts the warm-up sequence
void advanceCalibrationSequence();                  // Defined below; advances one warm-up reading (also called on wake)
void setDisplayReverse(bool reverse);               // Defined in CO2_Gadget_TFT.h or CO2_Gadget_OLED.h or CO2_Gadget_EINK.h
void setDisplayBrightness(uint16_t newBrightness);  // Defined in CO2_Gadget_TFT.h or CO2_Gadget_OLED.h

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
bool enableBLE = true;
bool activeBLE = true;
#ifdef SUPPORT_BTHOME_BLE
bool activeBTHome = false;
bool bthomeEncryption = false;
String bthomeBindKey = "";
// BTHome encryption uses a 32-bit packet counter in the nonce. A long-lived key
// must be replaced before this counter wraps.
uint32_t bthomeCounter = 0;
bool bthomeCounterNeedsSeed = false;
// [BTHOME-SENSEL] Selectable BTHome measurements: bitmask of which values to publish.
enum : uint32_t {
    BTHOME_SEL_BATTERY = 1u << 0,
    BTHOME_SEL_VOLTAGE = 1u << 1,
    BTHOME_SEL_TEMP    = 1u << 2,
    BTHOME_SEL_HUM     = 1u << 3,
    BTHOME_SEL_PRESS   = 1u << 4,
    BTHOME_SEL_CO2     = 1u << 5,
    BTHOME_SEL_PM25    = 1u << 6,
    BTHOME_SEL_PM10    = 1u << 7,
    BTHOME_SEL_PM1     = 1u << 8,  // no standard BTHome object (non-native)
    BTHOME_SEL_PM4     = 1u << 9,  // no standard BTHome object (non-native)
};
// Default selection = the "core" measurements only (CO2, temperature, humidity,
// battery). Optional sensors (pressure, PM2.5/PM10, battery voltage) are opt-in:
// pre-selecting them would show as "selected but not detected" on the many devices
// that lack that hardware. Only applies on first boot; a saved selection wins.
#define BTHOME_DEFAULT_SENSOR_MASK ((uint32_t)(BTHOME_SEL_BATTERY | BTHOME_SEL_TEMP | BTHOME_SEL_HUM | BTHOME_SEL_CO2))
uint32_t bthomeSensors = BTHOME_DEFAULT_SENSOR_MASK;
// Runtime-only mask of measurements populated during this boot/wake. The
// durable bthomeSensors mask remains the user's desired selection.
uint32_t bthomeFreshMeasurements = 0;
#else
constexpr bool activeBTHome = false;
constexpr bool bthomeEncryption = false;
const String bthomeBindKey = "";
constexpr uint32_t bthomeCounter = 0;
#endif
bool isDownloadingBLE = false;

#ifdef SUPPORT_LOW_POWER
constexpr uint32_t BLE_WAKE_SETTINGS_MAGIC = 0xB1E20207;

typedef struct {
    uint32_t magic = 0;
    bool enableBLEOnWake = true;
    bool sensirionBLEOnWake = true;
    bool activeBTHomeOnWake = false;
    bool bthomeEncryptionOnWake = false;
    char bthomeBindKeyOnWake[33] = "";
    uint32_t bthomeSensorsOnWake = 0;
    uint32_t bthomeCounterOnWake = 0;
    uint32_t checksum = 0;
} bleWakeSettings_t;

RTC_DATA_ATTR bleWakeSettings_t bleWakeSettingsRTC;
#endif

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
bool displayReverse = false;
bool showFahrenheit = false;
volatile bool displayShowTemperature = true;
volatile bool displayShowHumidity = true;
volatile bool displayShowBattery = true;
volatile bool displayShowBatteryVoltage = false;
volatile bool displayShowCO2 = true;
volatile bool displayShowPM25 = true;
volatile bool displayShowStatusIcons = true;
bool debugSensors = false;
bool inMenu = false;
volatile bool shouldWakeUpDisplay = false;
volatile bool shouldRedrawDisplay = false;
volatile bool redrawDisplayOnNextLoop = false;
bool isMenuDirty = false;  // To know if we need to redraw the menu
RTC_DATA_ATTR uint16_t measurementInterval = 10;
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
#ifdef SUPPORT_LOW_POWER
RTC_DATA_ATTR uint16_t vRef = 960;
#else
uint16_t vRef = 960;
#endif
uint16_t batteryDischargedMillivolts = 3200;    // Voltage of battery when we consider it discharged (0%).
uint16_t batteryFullyChargedMillivolts = 4200;  // Voltage of battery when it is considered fully charged (100%).

// Variables to control automatic display off to save power
#ifdef SUPPORT_LOW_POWER
RTC_DATA_ATTR bool hasBattery = false;
RTC_DATA_ATTR bool workingOnExternalPower = true;    // True if working on external power (USB connected)
RTC_DATA_ATTR bool displayOffOnExternalPower = false;
#else
bool hasBattery = false;
bool workingOnExternalPower = true;    // True if working on external power (USB connected)
bool displayOffOnExternalPower = false;
#endif
uint32_t actualDisplayBrightness = 0;  // To know if it's on or off
bool wakeDisplayOnCO2Alert = true;            // Wake display when CO2 rises above the warning threshold (issue #80)
uint16_t timeToDisplayOff = 0;                // Time in seconds to turn off the display to save power.
volatile uint64_t lastTimeButtonPressed = 0;  // Last time stamp button up was pressed

// Variables for color and output ranges
uint16_t co2OrangeRange = 700;
uint16_t co2RedRange = 1000;

// Variables for Improv-Serial
bool waitingForImprov = true;
uint16_t timeToWaitForImprov = 0;  // Time in seconds to wait for improv serial

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

// Calibration warm-up sequence phase (stored in RTC, survives deep sleep).
// See: https://github.com/melkati/CO2-Gadget/issues/250
enum CalPhase : uint8_t { CAL_IDLE = 0, CAL_WARMUP = 1 };

// Per-sensor warm-up requirement before a field calibration may be applied.
struct CalWarmup {
    uint16_t minReadings;  // usable readings required (excludes the discarded first)
    uint16_t minSeconds;   // minimum elapsed warm-up time
};
CalWarmup getCalWarmup();  // Defined after the sensor includes; used by the web status endpoints too

// LOW POWER MODES
// typedef enum LowPowerMode { HIGH_PERFORMANCE, BASIC_LOWPOWER, MEDIUM_LOWPOWER, MAXIMUM_LOWPOWER };

typedef struct {
    uint16_t lowPowerMode;  // 0 = HIGH_PERFORMANCE (no deep sleep), 1 = LOW_POWER (deep sleep enabled)
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
    bool hasPressureOnWake;  // A pressure sensor (BME280) was present at sleep; gates the SUPPORT_LOW_POWER_PRESSURE wake read
    bool activeWifiOnWake;
    bool sendMQTTOnWake;
    bool sendESPNowOnWake;
    bool displayOnWake;
    bool displayReverseOnWake;  // Display reverse on wake. Here to avoid having to read preferences on wake
    uint16_t timeToDisplayOnWake = 3;
    bool measurementsStarted;
    uint64_t bootTimes;
    uint64_t uptimeMillis;
    bool lastWifiRSSIValid;
    int16_t lastWifiRSSI;
    // Calibration warm-up sequence + pending ambient pressure, carried across deep
    // sleep so a calibration runs the datasheet-required warm-up (collect N readings
    // over a minimum time, discarding the first) even when that warm-up spans
    // several low-power wake cycles. See: https://github.com/melkati/CO2-Gadget/issues/250
    uint8_t calPhase;            // CalPhase: CAL_IDLE / CAL_WARMUP (set by beginCalibrationSequence)
    uint16_t calTargetPpm;       // reference ppm to calibrate to
    uint16_t calReadingsSeen;    // readings since the sequence began (the first is discarded)
    uint32_t calStartUptimeSec;  // reliable uptime when the sequence began (min-time gate)
    bool calForceContinuous;     // pause deep sleep for sensors needing continuous operation (CM1106)
    bool setAmbientPressureOnNextWake;
    uint16_t pendingAmbientPressureValue;
} deepSleepData_t;

RTC_DATA_ATTR deepSleepData_t deepSleepData;

uint64_t getReliableUptimeSeconds() {
    return (deepSleepData.uptimeMillis + millis()) / 1000;
}

String getReliableUptimeFormatted() {
    uint64_t totalMinutes = (deepSleepData.uptimeMillis + millis()) / 60000;
    uint64_t days = totalMinutes / 1440;
    uint8_t hours = (totalMinutes % 1440) / 60;
    uint8_t minutes = totalMinutes % 60;
    char uptime[32];

    snprintf(uptime, sizeof(uptime), "%02llu%c %02u%c %02u%c",
             static_cast<unsigned long long>(days),
             'd',
             static_cast<unsigned int>(hours),
             'h',
             static_cast<unsigned int>(minutes),
             'm');
    return String(uptime);
}

#ifdef BUILD_GIT
#undef BUILD_GIT
#endif  // ifdef BUILD_GIT
#define BUILD_GIT __DATE__
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
#include <ElegantOTA.h>
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
/*********         UNIFIED OUTPUTS: GPIO relays, RGB LEDs, NeoPixel, Buzzer                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Outputs.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********        FUNCTIONALITY TO STORE PREFERENCES IN NON VOLATILE MEMORY                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Preferences.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP CIRCULAR BUFFER FUNCTIONALITY                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#ifdef SUPPORT_CIRCULAR_BUFFER
#include "CO2_Gadget_CircularBufferManager.h"
#endif

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
#include <ElegantOTA.h>
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

// CO2_Gadget_Buzzer.h and CO2_Gadget_Neopixel.h are included via CO2_Gadget_Outputs.h above

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

// ---- Datasheet-compliant calibration warm-up sequence ----------------------
// Field calibration on these sensors is only valid after the sensor has been
// measuring in a stable, homogeneous CO2 environment for a sensor-specific
// warm-up, and the first reading after start-up must be discarded. In low-power
// mode the warm-up spans several deep-sleep wakes, so progress lives in RTC
// (deepSleepData) and advances one reading at a time.
// See: https://github.com/melkati/CO2-Gadget/issues/250
CalWarmup getCalWarmup() {
    switch (deepSleepData.co2Sensor) {
        case CO2Sensor_SCD40:
        case CO2Sensor_SCD41:        return {5, 180};   // >3 min / 5 single shots (SCD4x Low Power Operation)
        case CO2Sensor_SCD30:        return {4, 120};   // >2 min continuous (SCD30 interface description)
        case CO2Sensor_MHZ19:        return {3, 1200};  // ~20 min at 400 ppm before zero-point calibration
        case CO2Sensor_CM1106:
        case CO2Sensor_CM1106SL_NS:  return {3, 120};   // warm-up before start_calibration()
        case CO2Sensor_SENSEAIRS8:   return {3, 120};   // fresh-air exposure, then background manual_calibration()
        default:                     return {1, 0};     // DEMO / NONE: immediate (for testing)
    }
}

void beginCalibrationSequence(uint16_t ppm) {
    // Reject implausible targets: fresh air is ~400-430 ppm, so a value below 400 (or
    // 0) is not a valid calibration reference. Matches the menu's 400-2000 ppm range.
    if ((ppm < 400) || (ppm > 2000)) {
        Serial.println("-->[CAL] Ignoring calibration request: invalid value " + String(ppm) + " ppm (valid 400-2000)");
        return;
    }
    // CM1106 re-inits its driver and toggles CM1106_ENABLE_PIN on each deep-sleep
    // wake, so it is not guaranteed to stay continuously powered/measuring while a
    // warm-up spans sleeps — and its field calibration needs continuous operation.
    // Instead of refusing, pause deep sleep for the duration of the calibration so
    // the sensor runs continuously, then resume deep sleep when it completes (flag
    // cleared in advanceCalibrationSequence()). SCD30/SCD4x stay powered across
    // sleep (and SCD4x's one-shot-per-wake warm-up matches the datasheet), so they
    // keep calibrating across wakes. See: https://github.com/melkati/CO2-Gadget/issues/250
    bool isCm1106 = (deepSleepData.co2Sensor == CO2Sensor_CM1106) ||
                    (deepSleepData.co2Sensor == CO2Sensor_CM1106SL_NS);
    deepSleepData.calForceContinuous = (isCm1106 && (deepSleepData.lowPowerMode != HIGH_PERFORMANCE));
    if (deepSleepData.calForceContinuous) {
        Serial.println("-->[CAL] CM1106 needs continuous operation: pausing deep sleep until calibration completes.");
        // Ensure the sensor is in continuous measurement mode so sensors.loop() yields readings.
        if (sensors.isSensorRegistered(SENSORS::SCM1106)) {
            sensors.cm1106->set_working_status(CM1106_CONTINUOUS_MEASUREMENT);
        }
    }
    deepSleepData.calPhase = CAL_WARMUP;
    deepSleepData.calTargetPpm = ppm;
    deepSleepData.calReadingsSeen = 0;
    deepSleepData.calStartUptimeSec = (uint32_t)getReliableUptimeSeconds();
    CalWarmup w = getCalWarmup();
    Serial.println("-->[CAL] Starting calibration to " + String(ppm) + " ppm. Warm-up needs " +
                   String(w.minReadings) + " readings over >=" + String(w.minSeconds) + " s (first reading discarded).");
}

// Advance the warm-up by one fresh sensor reading. Called once per reading:
// from readingsLoop() in high-performance mode and from the deep-sleep wake
// handler in low-power mode (one reading per wake).
void advanceCalibrationSequence() {
    if (deepSleepData.calPhase != CAL_WARMUP) return;
    deepSleepData.calReadingsSeen++;
    if (deepSleepData.calReadingsSeen == 1) {
        Serial.println("-->[CAL] Warm-up: discarding first reading after start.");
        return;
    }
    CalWarmup w = getCalWarmup();
    uint16_t usable = deepSleepData.calReadingsSeen - 1;  // first reading is discarded
    uint32_t elapsed = (uint32_t)getReliableUptimeSeconds() - deepSleepData.calStartUptimeSec;
    Serial.println("-->[CAL] Warm-up: reading " + String(usable) + "/" + String(w.minReadings) +
                   ", elapsed " + String(elapsed) + "/" + String(w.minSeconds) + " s (CO2 " + String(co2) + " ppm)");
    if (usable < w.minReadings || elapsed < w.minSeconds) return;

    // Warm-up satisfied: issue the sensor-specific recalibration now.
    Serial.println("-->[CAL] Warm-up complete. Calibrating sensor to " + String(deepSleepData.calTargetPpm) + " ppm.");
    calibrationValue = deepSleepData.calTargetPpm;
    sensors.setCO2RecalibrationFactor(calibrationValue);
    saveCalibrationValue();  // persist so the value survives reboot. See issue #250
    deepSleepData.calPhase = CAL_IDLE;
    deepSleepData.calForceContinuous = false;  // resume deep sleep if it was paused for this calibration
    Serial.println("-->[CAL] Calibration command sent and value persisted.");
}

void processPendingCommands() {
    if (isDownloadingBLE) return;
    if (pendingCalibration == true) {
        pendingCalibration = false;
        // Don't calibrate immediately — start the datasheet warm-up sequence, which
        // collects/discards readings (across wakes in low power) before recalibrating.
        if ((calibrationValue >= 400) && (calibrationValue <= 2000)) {
            if (deepSleepData.calPhase == CAL_IDLE) {
                beginCalibrationSequence(calibrationValue);
            } else {
                Serial.println("-->[MAIN] Calibration already in progress (warming up to " + String(deepSleepData.calTargetPpm) + " ppm); ignoring new request for " + String(calibrationValue) + " PPM");
            }
        } else {
            Serial.println("-->[MAIN] Avoiding calibrating CO2 sensor with invalid value at " + String(calibrationValue) + " PPM");
        }
    }

    if (pendingAmbientPressure == true) {
        pendingAmbientPressure = false;
        if (ambientPressureValue != 0) {
            // Ambient pressure (mbar == hPa) enables continuous pressure compensation
            // and overrides altitude-based compensation. Only the Sensirion CO2 sensors
            // support it. See: https://github.com/melkati/CO2-Gadget/issues/250
            Serial.println("-->[MAIN] Setting ambient pressure for CO2 sensor to " + String(ambientPressureValue) + " mbar");
            bool isScd4x = (deepSleepData.co2Sensor == CO2Sensor_SCD40) ||
                           (deepSleepData.co2Sensor == CO2Sensor_SCD41);
            bool isScd30 = (deepSleepData.co2Sensor == CO2Sensor_SCD30);
            if (isScd4x && sensors.isSensorRegistered(SENSORS::SSCD4X)) {
                // SCD4x takes hPa (== mbar) and accepts it during periodic measurement.
                uint16_t err = sensors.scd4x.setAmbientPressure(ambientPressureValue);
                if (err) Serial.println("-->[MAIN][ERROR] SCD4x setAmbientPressure error: " + String(err));
            } else if (isScd30 && sensors.isSensorRegistered(SENSORS::SSCD30)) {
                // SCD30 sets pressure by (re)starting continuous measurement; valid 700-1400 mbar.
                if ((ambientPressureValue >= 700) && (ambientPressureValue <= 1400)) {
                    sensors.scd30.startContinuousMeasurement(ambientPressureValue);
                } else {
                    Serial.println("-->[MAIN] SCD30 ambient pressure out of range (700-1400 mbar); ignoring " + String(ambientPressureValue));
                }
            } else {
                Serial.println("-->[MAIN] Ambient pressure compensation not supported for the active sensor; ignoring.");
            }
        } else {
            Serial.println("-->[MAIN] Avoiding setting ambient pressure with invalid value (0 mbar)");
        }
    }
}

void initThresholds() {
    thresholdsManager.loadThresholdsFromNVR();
#ifdef DEBUG_THRESHOLDS
    Serial.print("-->[THRE] Thresholds loaded from NVRAM\t: ");
    printThresholdsFromNVR();
#endif
}

void readingsLoop() {
    if (isDownloadingBLE) return;
    if (esp_timer_get_time() - lastReadingsCommunicationTime >= startCheckingAfterUs) {
        if (newReadingsAvailable) {
            lastReadingsCommunicationTime = esp_timer_get_time();
            newReadingsAvailable = false;
            // Advance any calibration warm-up on each fresh reading. Fires in
            // high-performance mode and while a calibration has paused deep sleep
            // (CM1106 forced-continuous); in normal low power the deep-sleep wake
            // handler advances it once per wake instead. See issue #250.
            if ((deepSleepData.lowPowerMode == HIGH_PERFORMANCE) || deepSleepData.calForceContinuous) advanceCalibrationSequence();
            nav.idleChanged = true;  // Must redraw display as there are new readings
#ifdef SUPPORT_CIRCULAR_BUFFER
            addCO2Value(co2);
#endif
#ifdef SUPPORT_BLE
            publishBLE();
#endif
#ifdef SUPPORT_MQTT
            publishMQTT();
#endif
#ifdef SUPPORT_ESPNOW
            publishESPNow();
#endif

            // Issue #80: Wake display when CO2 rises above the warning threshold
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
            if (wakeDisplayOnCO2Alert && actualDisplayBrightness == 0) {
                static bool co2WasBelowAlertThreshold = true;
                if (co2 >= co2OrangeRange && co2WasBelowAlertThreshold) {
                    shouldWakeUpDisplay = true;
                    co2WasBelowAlertThreshold = false;
                } else if (co2 < (co2OrangeRange - PIN_HYSTERESIS)) {
                    co2WasBelowAlertThreshold = true;
                }
            }
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
    return;  // Temporary disable utilityLoop
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
    Serial.println("-->[STUP] Min Free heap: " + String(ESP.getMinFreeHeap()));
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

    initPreferences();
    initThresholds();
    initBattery();
    initOutputs();
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
#ifdef SUPPORT_CIRCULAR_BUFFER
    initCircularBuffer();
    setSensorSampleInterval(10);       // Set the default sample interval to 10 seconds
    setMovingAverageInterval(60);      // Set the default moving average interval to 60 seconds
    setLongTermBufferCapacity(1440);   // Store one day of data at 1 minute intervals
#endif
    Serial.println("-->[STUP] Ready.");
    Serial.flush();
    // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);  // enable brownout detector
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
    handleDisplayReverseOnWake();
    initDisplay(true);
#endif
    initBattery();
    initOutputsGPIO();
#ifdef SUPPORT_BLE
    restoreBLEWakeSettingsFromRTC();
    initBLE();
#endif
    // initSensors();
    if (deepSleepData.activeWifiOnWake) {
        Serial.println("-->[STUP]--> Reconnecting to WiFi...");
        doDeepSleepWiFiConnect();
        Serial.println("-->[STUP]--> Initializing Web Server...");
        initWebServer();
        server.begin();
        Serial.println("-->[WiFi] HTTP server started");
        printWiFiStatus();
    }
    wifiChanged = false;
#ifdef SUPPORT_ESPNOW
    initESPNow();
#endif
#ifdef SUPPORT_MQTT
    initMQTT();
#endif
    initPreferences();
    initButtons();
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
    Serial.setTxBufferSize(1024);
    Serial.setRxBufferSize(512);
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.println("-->[STUP] millis(): " + String(millis()));
    Serial.println("-->[STUP] Reset reason: (" + String(esp_reset_reason()) + ") " + getResetReason());
    Serial.println("-->[STUP] Wakeup cause: (" + String(esp_sleep_get_wakeup_cause()) + ") " + getWakeupCause());
    Serial.println("-->[STUP] lowPowerMode mode (from RTC memory): (" + String(deepSleepData.lowPowerMode) + ") " + getLowPowerModeName(deepSleepData.lowPowerMode));

    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (deepSleepData.lowPowerMode != HIGH_PERFORMANCE)) {
        if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
            deepSleepData.uptimeMillis += static_cast<uint64_t>(deepSleepData.timeSleeping) * 1000ULL;
        }
        ++deepSleepData.bootTimes;
        Serial.println("-->[STUP] Boot times from Deep Sleep: " + String(deepSleepData.bootTimes));
        timeToWaitForImprov = 0;

        // Reload wake flags from NVS before any wake-path handler runs.
        // This is critical for GPIO wakes (EXT0/EXT1/Touchpad) where setup()
        // calls initGPIOLowPower() before fromDeepSleep(), and initGPIOLowPower()
        // reads deepSleepData.activeWifiOnWake to decide whether to reconnect WiFi.
        // Without this early reload, corrupted RTC flags can trigger an unwanted
        // WiFi reconnect on GPIO wake.
        reloadWakeFlagsFromNVS();

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
            deepSleepData.uptimeMillis = 0;
            deepSleepData.lastWifiRSSIValid = false;
            deepSleepData.lastWifiRSSI = 0;
            Serial.println("-->[STUP] Initializing from: " + getResetReason());
            initPreferences();
            initThresholds();
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

    if (interactiveMode) {
        Serial.println("-->[STUP] Entering interactive mode");
    } else {
        Serial.println("-->[STUP] Entering high performance mode");
    }

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);  // enable brownout detector
}

void loopOLD() {  // Only reached in HIGH PERFORMANCE MODE
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
    deepSleepLoop();
}

void loop() {  // Old loop function. Not used anymore. Just for reference
    bool showDebug = false;
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
        // if ((showDebug) && (!inMenu)) Serial.println("-->[MAIN] Reading sensors with CO2 Gadget in high performance mode. ");
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
