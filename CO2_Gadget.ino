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

#include <Sensors.hpp>

#undef I2C_SDA
#undef I2C_SCL
#ifdef ALTERNATIVE_I2C_PINS
#define I2C_SDA 22
#define I2C_SCL 21
#else
#define I2C_SDA 21
#define I2C_SCL 22
#endif

String rootTopic    = UNITHOSTNAME; // Always defined to be able to configure in menu
String mqttClientId = UNITHOSTNAME; // Always defined to be able to configure in menu

bool activeBLE =  true;
bool activeWIFI = true;
bool activeMQTT = true;

bool pendingCalibration = false;
bool newReadingsAvailable = false;
uint16_t calibrationValue = 415;
uint16_t customCalibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;
uint16_t altidudeMeters = 600;
bool autoSelfCalibration = false;

uint16_t co2 = 0;
float temp, hum = 0;

uint16_t co2OrangeRange =
    700; // Default CO2 ppm concentration threshold to display values in orange
         // (user can change on menu and save on preferences)
uint16_t co2RedRange =
    1000; // Default CO2 ppm concentration threshold to display values in red
          // (user can change on menu and save on preferences)

void onSensorDataOk() {
  Serial.print("-->[MAIN] CO2: " + sensors.getStringCO2());
  Serial.print(" CO2humi: " + String(sensors.getCO2humi()));
  Serial.print(" CO2temp: " + String(sensors.getCO2temp()));

  Serial.print(" H: " + String(sensors.getHumidity()));
  Serial.println(" T: " + String(sensors.getTemperature()));

  co2 = sensors.getCO2();
  temp = sensors.getCO2temp();
  hum = sensors.getCO2humi();
  newReadingsAvailable = true;
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

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
const char *mqtt_server = MQTT_BROKER_SERVER; // Your MQTT broker IP address if any
#include "CO2_Gadget_MQTT.h"

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP BLE FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include "Sensirion_GadgetBle_Lib.h"
GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);

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
int vref = 1100;
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
// Button2 button;
// #define BUTTON_PIN  35 // Menu button (Press > 1500ms for calibration, press
// > 500ms to show battery voltage) void longpress(Button2& btn);
#define LONGCLICK_MS 300 // https://github.com/LennartHennigs/Button2/issues/10
#define BTN_UP 35 // Pinnumber for button for up/previous and select / enter actions
#define BTN_DWN 0 // Pinnumber for button for down/next and back / exit actions
#include "Button2.h"
Button2 btnUp(BTN_UP);   // Initialize the up button
Button2 btnDwn(BTN_DWN); // Initialize the down button

void button_init() {
  btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });

  btnUp.setClickHandler([](Button2 &b) {
    // Up
    nav.doNav(downCmd);
  });

  btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });

  btnDwn.setClickHandler([](Button2 &b) {
    // Down
    nav.doNav(upCmd);
  });
}

void buttonsLoop() {
  // Check for button presses
  btnUp.loop();
  btnDwn.loop();
}

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

void setup() {
  uint32_t brown_reg_temp =
      READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG); // save WatchDog register
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  Serial.begin(115200);
  Serial.printf("\nCO2 Gadget Version: %s%s\nStarting up...\n", CO2_GADGET_VERSION, CO2_GADGET_REV);

  initPreferences();

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

// #if defined SUPPORT_BLE && defined SUPPORT_WIFI
//   // Initialize the GadgetBle Library
//   gadgetBle.enableWifiSetupSettings(onWifiSettingsChanged);
//   gadgetBle.setCurrentWifiSsid(WIFI_SSID);
// #endif

  // Initialize the GadgetBle Library
  if (activeBLE) {
    gadgetBle.begin();
    Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
    Serial.println(gadgetBle.getDeviceIdString());
  }  

  initWifi();

  // Initialize sensors
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("-->[SETUP] Detecting sensors..");

  sensors.setSampleTime(5); // config sensors sample time interval
  sensors.setOnDataCallBack(&onSensorDataOk);     // all data read callback
  sensors.setOnErrorCallBack(&onSensorDataError); // [optional] error callback
  sensors.setDebugMode(true);                     // [optional] debug mode
  sensors.detectI2COnly(true);                    // force to only i2c sensors
  // sensors.scd30.setTemperatureOffset(2.0);         // example to set temp
  // offset

  sensors.init();
  if (sensors.isPmSensorConfigured())
    Serial.println("-->[SETUP] Sensor configured: " +
                   sensors.getPmDeviceSelected());

  delay(500);

  // sensors.setAutoSelfCalibration(false);

  initMQTT();

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,
                 brown_reg_temp); // enable brownout detector

  readBatteryVoltage();
  button_init();
  menu_init();

  Serial.println("Ready.");
}

void loop() {
  mqttClientLoop();
  sensors.loop();
  processPendingCommands();

  if (esp_timer_get_time() - lastReadingsCommunicationTime >= startCheckingAfterUs) {
    if (newReadingsAvailable) {
      lastReadingsCommunicationTime = esp_timer_get_time();
      newReadingsAvailable = false;
      nav.idleChanged = true; // Must redraw display as there are new readings

      if (activeBLE) {
        gadgetBle.writeCO2(co2);
        gadgetBle.writeTemperature(temp);
        gadgetBle.writeHumidity(hum);
        gadgetBle.commit();
      }
      // Provide the sensor values for Tools -> Serial Monitor or Serial Plotter
      Serial.printf("CO2[ppm]:%d\tTemperature[\u00B0C]:%.2f\tHumidity[%%]:%.2f\n", co2, temp, hum);
      if ((activeWIFI) && (WiFi.status() != WL_CONNECTED)) {
        Serial.println("WiFi not connected");
      }
      publishMQTT();
    }    
  }

  if (activeBLE) {
    gadgetBle.handleEvents();
    delay(3);
  }

#ifdef SUPPORT_OTA
  AsyncElegantOTA.loop();
#endif

  buttonsLoop();
  nav.poll(); // this device only draws when needed
}
