// Library Button2 by Lennart Hennigs https://github.com/LennartHennigs/Button2

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                   GENERAL GLOBAL DEFINITIONS AND OPTIONS                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#define SUPPORT_BLE
// #define SUPPORT_WIFI           // HTTP SERVER NOT WORKING CURRENTLY. AWAITING FIX
// #define SUPPORT_MQTT           // Needs SUPPORT_WIFI
// #define SUPPORT_OTA            // Needs SUPPORT_WIFI - CURRENTLY NOT WORKING AWAITING FIX
#define SUPPORT_TFT
#define ALTERNATIVE_I2C_PINS   // For the compact build as shown at https://emariete.com/medidor-co2-display-tft-color-ttgo-t-display-sensirion-scd30/

#define UNITHOSTNAME "TEST"

#include <Wire.h>
#include "soc/soc.h"           // disable brownout problems
#include "soc/rtc_cntl_reg.h"  // disable brownout problems

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP CO2 SENSOR SCD30                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include "SparkFun_SCD30_Arduino_Library.h"
SCD30 airSensor;

#ifdef ALTERNATIVE_I2C_PINS
#define I2C_SDA 22
#define I2C_SCL 21
#else
#define I2C_SDA 21
#define I2C_SCL 22
#endif

bool pendingCalibration = false;
uint16_t calibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;

uint16_t co2 = 0;
float temp, hum  = 0;


/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP PUSH BUTTONS FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include "Button2.h"
Button2 button;
#define BUTTON_PIN  35 // Menu button (Press > 1500ms for calibration, press > 500ms to show battery voltage)
void longpress(Button2& btn);

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP BLE FUNCTIONALITY                              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#ifdef SUPPORT_BLE
#include "Sensirion_GadgetBle_Lib.h"
GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#ifdef SUPPORT_WIFI
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
// #include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index.h"  //Web page header file

#include "credentials.h"
WiFiClient espClient;
WiFiMulti WiFiMulti;
AsyncWebServer server(80);

void onWifiSettingsChanged(std::string ssid, std::string password) {
  Serial.print("WifiSetup: SSID = ");
  Serial.print(ssid.c_str());
  Serial.print(", Password = ");
  Serial.println(password.c_str());

  WiFiMulti.addAP(ssid.c_str(), password.c_str());
}

////===============================================================
//// This function is called when you open its IP in browser
////===============================================================
//void handleRoot() {
// String s = MAIN_page; //Read HTML contents
// server.send(200, "text/html", s); //Send web page
//}
//
//void handleADC() {
// int a = analogRead(A0);
// String co2Value = String(co2);
//
// server.send(200, "text/plane", co2Value); //Send ADC value only to client ajax request
//}
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_MQTT
#include <PubSubClient.h>

// ----------------------------------------------------------------------------
// MQTT handling
// ----------------------------------------------------------------------------
// const char *mqtt_server = "192.168.1.145";
String rootTopic;
String topic;
char charPublish[20];
PubSubClient mqttClient(espClient);
#endif

#ifdef SUPPORT_MQTT
void mqttReconnect()
{
  if (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    topic = rootTopic + "/#";
    if (mqttClient.connect((topic).c_str()))
    {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe((topic).c_str());
    }
    else
    {
      Serial.println(" not possible to connect");
    }
  }

  // Loop until we're reconnected
  // while (!mqttClient.connected())
  // {
  //   Serial.print("Attempting MQTT connection...");
  //   // Attempt to connect
  //   topic = rootTopic + "/#";
  //   if (mqttClient.connect((topic).c_str()))
  //   {
  //     Serial.println("connected");
  //     // Subscribe
  //     mqttClient.subscribe((topic).c_str());
  //   }
  //   else
  //   {
  //     Serial.print("failed, rc=");
  //     Serial.print(mqttClient.state());
  //     Serial.println(" try again in 5 seconds");
  //     // Wait 5 seconds before retrying
  //     delay(5000);
  //   }
  // }
}

// Function called when data is received via MQTT
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Payload: ");
  String messageTemp;
  String topicTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (strcmp(topic, "SCD30/calibration") == 0) {
    printf("Received calibration command with value %d\n", messageTemp.toInt());
    pendingCalibration = true;
    calibrationValue = messageTemp.toInt();
  }

  if (strcmp(topic, "SCD30/ambientpressure") == 0) {
    printf("Received ambient pressure with value %d\n", messageTemp.toInt());
    pendingAmbientPressure = true;
    ambientPressureValue = messageTemp.toInt();
  }
}

void publishIntMQTT(String topic, int16_t payload)
{
  dtostrf(payload, 0, 0, charPublish);
  topic = rootTopic + topic;
  Serial.printf("Publishing %d to ", payload);
  Serial.println("topic: " + topic);
  mqttClient.publish((topic).c_str(), charPublish);
}

void publishFloatMQTT(String topic, float payload)
{
  dtostrf(payload, 0, 2, charPublish);
  topic = rootTopic + topic;
  Serial.printf("Publishing %.0f to ", payload);
  Serial.println("topic: " + topic);
  mqttClient.publish((topic).c_str(), charPublish);
}
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP OTA FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP TFT DISPLAY FUNCTIONALITY                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_TFT
// Go to TTGO T-Display's Github Repository
// Download the code as zip, extract it and copy the Folder TFT_eSPI
//  => https://github.com/Xinyuan-LilyGO/TTGO-T-Display/archive/master.zip
// to your Arduino library path
#include <TFT_eSPI.h>
#include <SPI.h>

#define SENSIRION_GREEN 0x6E66
#define sw_version "v2.0"

#define ADC_PIN 34
int vref = 1100;

#define GFXFF 1
#define FF99  &SensirionSimple25pt7b
#define FF90  &ArchivoNarrow_Regular10pt7b
#define FF95  &ArchivoNarrow_Regular50pt7b

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke library, pins defined in User_Setup.h
#endif

void initDisplayTFT() {
#if defined SUPPORT_TFT
  tft.init();
  tft.setRotation(1);
#endif
}

void displaySplashScreenTFT() {
#if defined SUPPORT_TFT
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(SENSIRION_GREEN, TFT_WHITE);

  uint8_t defaultDatum = tft.getTextDatum();
  tft.setTextDatum(1); // Top centre

  tft.setTextSize(1);
  tft.setFreeFont(FF99);
  tft.drawString("B", 120, 40);

  tft.setTextSize(1);
  tft.drawString(sw_version, 120, 90, 2);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);
#endif
}

void showValuesTFT(uint16_t co2) {
#if defined SUPPORT_TFT
  if (co2 > 9999) {
    co2 = 9999;
  }

  tft.fillScreen(TFT_BLACK);

  uint8_t defaultDatum = tft.getTextDatum();

  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setTextDatum(6); // bottom left
  tft.drawString("CO2", 10, 125);

  tft.setTextDatum(8); // bottom right
  tft.drawString(gadgetBle.getDeviceIdString(), 230, 125);

  // Draw CO2 number
  if (co2 >= 1000 ) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (co2 >= 700 ) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }

  tft.setTextDatum(8); // bottom right
  tft.setTextSize(1);
  tft.setFreeFont(FF95);
  tft.drawString(String(co2), 195, 105);

  // Draw CO2 unit
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.drawString("ppm", 230, 90);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);
#endif
}

void showVoltageTFT()
{
#if defined SUPPORT_TFT
  static uint64_t timeStamp = 0;
  if (millis() - timeStamp > 1000) {
    timeStamp = millis();
    uint16_t v = analogRead(ADC_PIN);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    String voltage = "Voltage :" + String(battery_voltage) + "V";
    Serial.println(voltage);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(voltage,  tft.width() / 2, tft.height() / 2 );
  }
#endif
}

/*****************************************************************************************************/

static int64_t lastMmntTime = 0;
static int startCheckingAfterUs = 1900000;

void initMQTT()
{
#ifdef SUPPORT_MQTT
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  rootTopic = UNITHOSTNAME;
#endif
}

void processPendingCommands()
{
  if (pendingCalibration == true) {
    if (calibrationValue != 0) {
      printf("Calibrating SCD30 sensor at %d PPM\n", calibrationValue);
      pendingCalibration = false;
      airSensor.setForcedRecalibrationFactor(calibrationValue);
    }
    else
    {
      printf("Avoiding calibrating SCD30 sensor with invalid value at %d PPM\n", calibrationValue);
      pendingCalibration = false;
    }
  }

  if (pendingAmbientPressure == true) {
    if (ambientPressureValue != 0) {
      printf("Setting AmbientPressure for SCD30 sensor at %d mbar\n", ambientPressureValue);
      pendingAmbientPressure = false;
      airSensor.setAmbientPressure(ambientPressureValue);
    }
    else
    {
      printf("Avoiding setting AmbientPressure for SCD30 sensor with invalid value at %d mbar\n", ambientPressureValue);
      pendingAmbientPressure = false;
    }
  }
}

void setup()
{
  uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG); //save WatchDog register
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

#if defined SUPPORT_TFT
  // Display init and splash screen
  initDisplayTFT();
  displaySplashScreenTFT();
  // Enjoy the splash screen for 2 seconds
  delay(2000);
#endif

#if defined SUPPORT_BLE && defined SUPPORT_WIFI
  // Initialize the GadgetBle Library
  gadgetBle.enableWifiSetupSettings(onWifiSettingsChanged);
  gadgetBle.setCurrentWifiSsid(WIFI_SSID_CREDENTIALS);
#endif

#ifdef SUPPORT_WIFI
  WiFiMulti.addAP(WIFI_SSID_CREDENTIALS, WIFI_PW_CREDENTIALS);
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  /*use mdns for host name resolution*/
  if (!MDNS.begin(UNITHOSTNAME)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started. CO2 monitor web interface at: http://");
  Serial.print(UNITHOSTNAME);
  Serial.println(".local");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "CO2: " + String(co2) + " PPM");
    //  server.on("/", handleRoot);      //This is display page
    //  server.on("/readADC", handleADC);//To get update of ADC Value only
  });

#ifdef SUPPORT_OTA
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  Serial.println("OTA ready");
#endif

  server.begin();
  Serial.println("HTTP server started");
#endif

#ifdef SUPPORT_BLE
  // Initialize the GadgetBle Library
  gadgetBle.begin();
  Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
  Serial.println(gadgetBle.getDeviceIdString());
#endif

  // Initialize the SCD30 driver
  Wire.begin(I2C_SDA, I2C_SCL);
  if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    //    while (1)
    //      ;
  }
  else
  {
    airSensor.setAutoSelfCalibration(false);
  }

  initMQTT();

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp); //enable brownout detector

  button.begin(BUTTON_PIN);
  button.setLongClickHandler(longpress);

  Serial.println("Ready.");
}

void loop()
{
#ifdef SUPPORT_MQTT
  mqttClient.loop();
#endif

  processPendingCommands();

  if (esp_timer_get_time() - lastMmntTime >= startCheckingAfterUs) {
    if (airSensor.dataAvailable()) {
      co2  = airSensor.getCO2();
      temp = airSensor.getTemperature();
      hum  = airSensor.getHumidity();

#if defined SUPPORT_TFT
      showValuesTFT(co2);
#endif

#ifdef SUPPORT_BLE
      gadgetBle.writeCO2(co2);
      gadgetBle.writeTemperature(temp);
      gadgetBle.writeHumidity(hum);
      gadgetBle.commit();
#endif
      lastMmntTime = esp_timer_get_time();

      // Provide the sensor values for Tools -> Serial Monitor or Serial Plotter
      Serial.print("CO2[ppm]:");
      Serial.print(co2);
      Serial.print("\t");
      Serial.print("Temperature[℃]:");
      Serial.print(temp);
      Serial.print("\t");
      Serial.print("Humidity[%]:");
      Serial.println(hum);

//      Serial.print("Free heap: ");
//      Serial.println(ESP.getFreeHeap());

#ifdef SUPPORT_WIFI
      if (WiFiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected");
      } else {
        Serial.print("WiFi connected - IP = ");
        Serial.println(WiFi.localIP());
        if (!mqttClient.connected())
        {
          mqttReconnect();
        }
      }
#endif

#if defined SUPPORT_MQTT && defined SUPPORT_WIFI
      if (WiFiMulti.run() == WL_CONNECTED) {
        publishIntMQTT("/co2", co2);
        publishFloatMQTT("/temp", temp);
        publishFloatMQTT("/humi", hum);
      }
#endif
    }
  }

#ifdef SUPPORT_BLE
  gadgetBle.handleEvents();
  delay(3);
#endif

#ifdef SUPPORT_OTA
  AsyncElegantOTA.loop();
#endif

  button.loop();
}

void longpress(Button2& btn) {
  unsigned int time = btn.wasPressedFor();
  Serial.print("You clicked ");
  if (time > 3000) {
    Serial.print("a really really long time.");
    calibrationValue = 400;
    printf("Manually calibrating SCD30 sensor at %d PPM\n", calibrationValue);
    airSensor.setForcedRecalibrationFactor(calibrationValue);
    pendingCalibration = false;
  } else if (time > 1000) {
    Serial.print("a really long time.");
  } else if (time > 500) {
    Serial.print("a long time.");
    showVoltageTFT();
    delay(2000);
  } else {
    Serial.print("long.");
  }
  Serial.print(" (");
  Serial.print(time);
  Serial.println(" ms)");
}
