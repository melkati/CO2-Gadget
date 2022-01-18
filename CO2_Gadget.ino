/*****************************************************************************************************/
/*********                   GENERAL GLOBAL DEFINITIONS AND OPTIONS                          *********/
/*****************************************************************************************************/
/* If you are NOT using PlatformIO (You are using Arduino IDE) you must define your options bellow   */
/* If you ARE using PlarformIO (NOT Arduino IDE) you must define your options in platformio.ini file */
/*  THIS SECTION IS OUTDATED, IF YOU WANT TO USE ARDUINO YOU WILL HAVE TO SHORT IT OUT BY YOURSELF   */
/*            I WILL PREPARE THE CODE AND WRITE NEW INSTRUCTIONS AS TIME PERMITS.                    */
/*                                                                                                   */
#ifndef PLATFORMIO
#define SUPPORT_OTA
#define SUPPORT_TFT
#define DEBUG_ARDUINOMENU
#define UNITHOSTNAME "CO2-Gadget"
// #define ALTERNATIVE_I2C_PINS   // For the compact build as shown at https://emariete.com/medidor-co2-display-tft-color-ttgo-t-display-sensirion-scd30/
#endif
/*****************************************************************************************************/

// Next data always defined to be able to configure in menu
String hostName = UNITHOSTNAME;
String rootTopic = UNITHOSTNAME;
String mqttClientId = UNITHOSTNAME;
String mqttBroker = MQTT_BROKER_SERVER;
String mqttUser = "";
String mqttPass = "";
String wifiSSID = WIFI_SSID_CREDENTIALS;
String wifiPass = WIFI_PW_CREDENTIALS;
String mDNSName = "Unset";

bool activeBLE = true;
bool activeWIFI = true;
bool activeMQTT = true;
bool debugSensors = false;
bool displayReverse = false;
bool showFahrenheit = false;
uint16_t measurementInterval = 10;
bool inMenu = false;
bool bleInitialized = false;
int8_t selectedCO2Sensor = -1;
uint32_t DisplayBrightness = 100;
bool outputsModeRelay = false;

// Variables to control automatic display off to save power
uint32_t actualDisplayBrightness = 100;  // To know if it's on or off
bool displayOffOnExternalPower = false;
uint16_t timeToDisplayOff = 0;                                         // Time in seconds to turn off the display to save power.
uint64_t nextTimeToDisplayOff = millis() + (timeToDisplayOff * 1000);  // Next time display should turn off
uint64_t lastButtonUpTimeStamp = millis();                             // Last time button UP was pressed

#ifdef BUILD_GIT
#undef BUILD_GIT
#endif  // ifdef BUILD_GIT
#define BUILD_GIT __DATE__

#ifdef ALTERNATIVE_I2C_PINS
#undef I2C_SDA
#undef I2C_SCL
#define I2C_SDA 22
#define I2C_SCL 21
#else
#undef I2C_SDA
#undef I2C_SCL
#define I2C_SDA 21
#define I2C_SCL 22
#endif

#include <WiFi.h>
#include <Wire.h>

#include "driver/adc.h"
#include "soc/rtc_cntl_reg.h"  // disable brownout problems
#include "soc/soc.h"  // disable brownout problems

#ifdef SUPPORT_MDNS
#include <ESPmDNS.h>
#endif
// #include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#ifdef SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif
#include <FS.h>
#include <SPIFFS.h>

// Functions and enum definitions
void reverseButtons(bool reversed);
void outputsLoop();

enum notificationTypes { notifyNothing,
                         notifyInfo,
                         notifyWarning,
                         notifyError };
bool displayNotification(String notificationText, notificationTypes notificationType);

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP SENSORS                                   *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <CO2_Gadget_Sensors.h>

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           INCLUDE WIFI FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include <CO2_Gadget_WIFI.h>

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
#ifdef SUPPORT_BLE
#include "CO2_Gadget_BLE.h"
#endif

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
/*********                           INCLUDE BATTERY FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#define ADC_PIN 34
uint16_t vRef = 1100;
uint16_t batteryDischargedMillivolts = 3500;    // Voltage of battery when we consider it discharged (0%).
uint16_t batteryFullyChargedMillivolts = 4200;  // Voltage of battery when it is considered fully charged (100%).
#include "CO2_Gadget_Battery.h"

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
/*********                         INCLUDE MENU FUNCIONALITY                                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Menu.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                      SETUP PUSH BUTTONS FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#include "CO2_Gadget_Buttons.h"

/*****************************************************************************************************/

static int64_t lastReadingsCommunicationTime = 0;
static int startCheckingAfterUs = 1900000;

void processPendingCommands() {
    if (pendingCalibration == true) {
        if (calibrationValue != 0) {
            printf("-->[MAIN] Calibrating CO2 sensor at %d PPM\n", calibrationValue);
            pendingCalibration = false;
            sensors.setCO2RecalibrationFactor(calibrationValue);
        } else {
            printf("-->[MAIN] Avoiding calibrating CO2 sensor with invalid value at %d PPM\n",
                   calibrationValue);
            pendingCalibration = false;
        }
    }

    if (pendingAmbientPressure == true) {
        if (ambientPressureValue != 0) {
            printf("-->[MAIN] Setting AmbientPressure for CO2 sensor at %d mbar\n",
                   ambientPressureValue);
            pendingAmbientPressure = false;
            sensors.scd30.setAmbientPressure(ambientPressureValue);
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
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(GREEN_PIN, LOW);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(BLUE_PIN, LOW);
    pinMode(RED_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
}

void outputsRelays() {
    if ((!outputsModeRelay) || (co2 == 0)) return;  // Don't turn on relays until there is CO2 Data
    if (co2 >= co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
    }
    if (co2 < co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
    }
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
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        return;
    }
    if (co2 >= co2OrangeRange) {
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        return;
    }
    digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
    digitalWrite(BLUE_PIN, GREEN_PIN_LOW);
    digitalWrite(RED_PIN, GREEN_PIN_LOW);
}

void outputsLoop() {
    outputsRelays();
    outputsRGBLeds();
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
            if ((activeWIFI) && (WiFi.status() != WL_CONNECTED)) {
                Serial.println("-->[MAIN] WiFi not connected");
            }
#ifdef SUPPORT_MQTT
            publishMQTT();
#endif
        }
    }
}

void displayLoop() {
    if (timeToDisplayOff == 0)  // TFT Always ON
        return;

    // If configured not to turn off the display on external power
    // and actual voltage is more than those of a maximum loaded batery + 5%, do
    // nothing and return
    if ((!displayOffOnExternalPower) &&
        (battery_voltage * 1000 > batteryFullyChargedMillivolts +
                                      (batteryFullyChargedMillivolts * 5 / 100))) {
        if (actualDisplayBrightness == 0)  // When connect USB & TFT is OFF -> TURN IT ON
        {
            setDisplayBrightness(DisplayBrightness);  // Turn on the display at DisplayBrightness brightness
            actualDisplayBrightness = DisplayBrightness;
        }
        return;
    }

    if (millis() > nextTimeToDisplayOff) {
        Serial.println("-->[MAIN] Turning off display to save power");
        turnOffDisplay();
        actualDisplayBrightness = 0;
        nextTimeToDisplayOff = nextTimeToDisplayOff + (timeToDisplayOff * 1000);
    }
}

void setup() {
    uint32_t brown_reg_temp =
        READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);  // save WatchDog register
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // disable brownout detector
    Serial.begin(115200);
    delay(100);
    // Serial.printf("Total heap: %d", ESP.getHeapSize());
    // Serial.printf("Free heap: %d", ESP.getFreeHeap());
    // Serial.printf("Total PSRAM: %d", ESP.getPsramSize());
    // Serial.printf("Free PSRAM: %d", ESP.getFreePsram());
    Serial.printf("\n-->[MAIN] CO2 Gadget Version: %s%s Flavour: %s\nStarting up...\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
    Serial.printf("\n-->[MAIN] Version compiled: %s at %s\n", __DATE__, __TIME__);

    setCpuFrequencyMhz(80);  // Lower CPU frecuency to reduce power consumption
    initPreferences();
    initBattery();
    initGPIO();
    initNeopixel();
    initDisplay();
#ifdef SUPPORT_BLE
    initBLE();
#endif
    initWifi();
    initSensors();
#ifdef SUPPORT_MQTT
    initMQTT();
#endif
    menu_init();
    buttonsInit();
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,
                   brown_reg_temp);  // enable brownout detector
    Serial.println("-->[STUP] Ready.");
}

void loop() {
    mqttClientLoop();
    sensorsLoop();
    outputsLoop();
    processPendingCommands();
    readingsLoop();
    OTALoop();
    displayLoop();
    buttonsLoop();
    menuLoop();
    neopixelLoop();
}
