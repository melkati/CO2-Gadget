// clang-format off
/*****************************************************************************************************/
/*********                   GENERAL GLOBAL DEFINITIONS AND OPTIONS                          *********/
/*****************************************************************************************************/
/* If you are NOT using PlarformIO (You are using Arduino IDE) you must define your options bellow   */
/* If you ARE using PlarformIO (NOT Arduino IDE) you must define your options in platformio.ini file */
/**/ #ifndef PLATFORMIO
/**/ // #define SUPPORT_OTA            // Needs SUPPORT_WIFI - CURRENTLY NOT WORKING AWAITING FIX
/**/ #define SUPPORT_TFT
/**/ #define DEBUG_ARDUINOMENU
#define UNITHOSTNAME "CO2-Gadget"
/**/ // #define ALTERNATIVE_I2C_PINS   // For the compact build as shown at https://emariete.com/medidor-co2-display-tft-color-ttgo-t-display-sensirion-scd30/
/**/ #endif
/*****************************************************************************************************/
// clang-format on

// Next data always defined to be able to configure in menu
String rootTopic    = UNITHOSTNAME;
String mqttClientId = UNITHOSTNAME;
String mqttBroker   = MQTT_BROKER_SERVER;
String wifiSSID     = WIFI_SSID_CREDENTIALS;
String wifiPass     = WIFI_PW_CREDENTIALS;

bool activeBLE =  true;
bool activeWIFI = true;
bool activeMQTT = true;

uint16_t timeToDisplayOff = 0; // Time in seconds to turn off the display to save power.

#ifdef BUILD_GIT
#undef BUILD_GIT
#endif // ifdef BUILD_GIT
#define BUILD_GIT __DATE__

#include <Wire.h>
#include "soc/soc.h" // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems

#include <WiFi.h>
#include <ESPmDNS.h>
// #include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP SENSORS                                   *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#include <CO2_Gadget_Sensors.h>

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           INCLUDE WIFI FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <CO2_Gadget_WIFI.h>

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "CO2_Gadget_MQTT.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP BLE FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "CO2_Gadget_BLE.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP OTA FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           INCLUDE BATTERY FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#define ADC_PIN 34
uint16_t vRef = 1100;
uint16_t batteryDischargedMillivolts = 3500; // Voltage of battery when we consider it discharged (0%).
uint16_t batteryFullyChargedMillivolts = 4200; // Voltage of battery when it is considered fully charged (100%).
#include "CO2_Gadget_Battery.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********               INCLUDE OLED DISPLAY FUNCTIONALITY (UNFINISHED WIP)                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_OLED
#include <CO2_Gadget_OLED.h>
#endif

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                      INCLUDE TFT DISPLAY FUNCTIONALITY                            *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_TFT
#include "CO2_Gadget_TFT.h"
#endif

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********        FUNCTIONALITY TO STORE PREFERENCES IN NON VOLATILE MEMORY                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "CO2_Gadget_Preferences.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                         INCLUDE MENU FUNCIONALITY                                 *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "CO2_Gadget_Menu.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                      SETUP PUSH BUTTONS FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "CO2_Gadget_Buttons.h"

/*****************************************************************************************************/

static int64_t lastReadingsCommunicationTime = 0;
static int startCheckingAfterUs = 1900000;

void processPendingCommands() {
  if (pendingCalibration == true) {
    if (calibrationValue != 0) {
      printf("Calibrating CO2 sensor at %d PPM\n", calibrationValue);
      pendingCalibration = false;
      sensors.setCO2RecalibrationFactor(calibrationValue);
    } else {
      printf("Avoiding calibrating CO2 sensor with invalid value at %d PPM\n",
             calibrationValue);
      pendingCalibration = false;
    }
  }

  if (pendingAmbientPressure == true) {
    if (ambientPressureValue != 0) {
      printf("Setting AmbientPressure for CO2 sensor at %d mbar\n",
             ambientPressureValue);
      pendingAmbientPressure = false;
      sensors.scd30.setAmbientPressure(ambientPressureValue);
    } else {
      printf("Avoiding setting AmbientPressure for CO2 sensor with invalid "
             "value at %d mbar\n",
             ambientPressureValue);
      pendingAmbientPressure = false;
    }
  }
}

void readingsLoop() {
  if (esp_timer_get_time() - lastReadingsCommunicationTime >= startCheckingAfterUs) {
    if (newReadingsAvailable) {
      lastReadingsCommunicationTime = esp_timer_get_time();
      newReadingsAvailable = false;
      nav.idleChanged = true; // Must redraw display as there are new readings
      publishBLE();
      // Provide the sensor values for Tools -> Serial Monitor or Serial Plotter
      Serial.printf("CO2[ppm]:%d\tTemperature[\u00B0C]:%.2f\tHumidity[%%]:%.2f\n", co2, temp, hum);
      if ((activeWIFI) && (WiFi.status() != WL_CONNECTED)) {
        Serial.println("WiFi not connected");
      }
      publishMQTT();
    }    
  }
}

void setup() {
  uint32_t brown_reg_temp =
      READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG); // save WatchDog register
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector
  Serial.begin(115200);
  Serial.printf("\nCO2 Gadget Version: %s%s\nStarting up...\n", CO2_GADGET_VERSION, CO2_GADGET_REV);
  initPreferences();
  initBattery();
#if defined SUPPORT_OLED
  delay(100);
  initDisplayOLED();
  delay(1000);
#endif
#if defined SUPPORT_TFT
  initDisplayTFT();
  displaySplashScreenTFT(); // Display init and splash screen
  delay(2000);              // Enjoy the splash screen for 2 seconds
  tft.setTextSize(2);
#endif
  initBLE();
  initWifi();
  initSensors();  
  initMQTT();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,
                 brown_reg_temp); // enable brownout detector
  // readBatteryVoltage();
  buttonsInit();
  menu_init();
  Serial.println("Ready.");
}

void loop() {
  mqttClientLoop();
  sensorsLoop();
  processPendingCommands();
  readingsLoop();
  loopBLE();
#ifdef SUPPORT_OTA
  AsyncElegantOTA.loop();
#endif
  buttonsLoop();
  nav.poll(); // this device only draws when needed
}
