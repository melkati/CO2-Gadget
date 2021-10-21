# 1 "C:\\Users\\Mario\\AppData\\Local\\Temp\\tmpvqkwfhwk"
#include <Arduino.h>
# 1 "D:/Documentos/Arduino/CO2_Gadget/CO2_Gadget.ino"






#define SUPPORT_BLE 



#define SUPPORT_TFT 
#define SUPPORT_ARDUINOMENU 


#define UNITHOSTNAME "TEST"

#ifdef BUILD_GIT
#undef BUILD_GIT
#endif
#define BUILD_GIT __DATE__

#include <Wire.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"







#include <Sensors.hpp>

#ifdef ALTERNATIVE_I2C_PINS
#define I2C_SDA 22
#define I2C_SCL 21
#else
#define I2C_SDA 21
#define I2C_SCL 22
#endif

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
void onSensorDataOk();
void onSensorDataError(const char* msg);
void button_init();
void button_loop();
void initMQTT();
void processPendingCommands();
void setup();
void loop();
#line 54 "D:/Documentos/Arduino/CO2_Gadget/CO2_Gadget.ino"
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

void onSensorDataError(const char* msg) {
    Serial.println(msg);
}







uint16_t co2OrangeRange = 700;
uint16_t co2RedRange = 1000;






#ifdef SUPPORT_BLE
#include "Sensirion_GadgetBle_Lib.h"
GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2);
#endif






#ifdef SUPPORT_WIFI
#include <CO2_Gadget_WIFI>
#endif






#if defined SUPPORT_MQTT
#include "CO2_Gadget_MQTT.h"
#endif






#if defined SUPPORT_OTA
#include <AsyncElegantOTA.h>
#endif






#define ADC_PIN 34
int vref = 1100;
#include "CO2_Gadget_Battery.h"






#if defined SUPPORT_OLED
#include <CO2_Gadget_OLED.h>
#endif






#if defined SUPPORT_TFT
#include "CO2_Gadget_TFT.h"
#endif






#include "CO2_Gadget_Preferences.h"






#if defined SUPPORT_ARDUINOMENU
#define DEBUG_ARDUINOMENU 
#include "CO2_Gadget_Menu.h"
#endif






#include "Button2.h"



#define LONGCLICK_MS 300
#define BTN_UP 35
#define BTN_DWN 0
Button2 btnUp(BTN_UP);
Button2 btnDwn(BTN_DWN);

void button_init()
{
#if defined SUPPORT_ARDUINOMENU
    btnUp.setLongClickHandler([](Button2 & b) {
        nav.doNav(enterCmd);
    });

    btnUp.setClickHandler([](Button2 & b) {

       nav.doNav(downCmd);
    });

    btnDwn.setLongClickHandler([](Button2 & b) {
        nav.doNav(escCmd);
    });

    btnDwn.setClickHandler([](Button2 & b) {

        nav.doNav(upCmd);
    });
#endif
}

void button_loop()
{

    btnUp.loop();
    btnDwn.loop();
}



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
      printf("Calibrating CO2 sensor at %d PPM\n", calibrationValue);
      pendingCalibration = false;
      sensors.setCO2RecalibrationFactor(calibrationValue);
    }
    else
    {
      printf("Avoiding calibrating CO2 sensor with invalid value at %d PPM\n", calibrationValue);
      pendingCalibration = false;
    }
  }

  if (pendingAmbientPressure == true) {
    if (ambientPressureValue != 0) {
      printf("Setting AmbientPressure for CO2 sensor at %d mbar\n", ambientPressureValue);
      pendingAmbientPressure = false;
      sensors.scd30.setAmbientPressure(ambientPressureValue);
    }
    else
    {
      printf("Avoiding setting AmbientPressure for CO2 sensor with invalid value at %d mbar\n", ambientPressureValue);
      pendingAmbientPressure = false;
    }
  }
}

void setup()
{
  uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  initPreferences();

#if defined SUPPORT_OLED
  delay(100);
  initDisplayOLED();
  delay(1000);
#endif

#if defined SUPPORT_TFT
  initDisplayTFT();
  displaySplashScreenTFT();
  delay(2000);

  tft.setTextSize(2);
#endif

#if defined SUPPORT_BLE && defined SUPPORT_WIFI

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


  if (!MDNS.begin(UNITHOSTNAME)) {
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


  });

#ifdef SUPPORT_OTA
  AsyncElegantOTA.begin(&server);
  Serial.println("OTA ready");
#endif

  server.begin();
  Serial.println("HTTP server started");
#endif

#ifdef SUPPORT_BLE

  gadgetBle.begin();
  Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
  Serial.println(gadgetBle.getDeviceIdString());
#endif


  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("-->[SETUP] Detecting sensors..");

  sensors.setSampleTime(5);
  sensors.setOnDataCallBack(&onSensorDataOk);
  sensors.setOnErrorCallBack(&onSensorDataError);
  sensors.setDebugMode(true);
  sensors.detectI2COnly(true);



  sensors.init();
  if (sensors.isPmSensorConfigured())
        Serial.println("-->[SETUP] Sensor configured: " + sensors.getPmDeviceSelected());

  delay(500);



  initMQTT();

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);

  readBatteryVoltage();
  button_init();
  menu_init();

  Serial.println("Ready.");
}

void loop()
{
#ifdef SUPPORT_MQTT
  mqttClient.loop();
#endif

  sensors.loop();
  processPendingCommands();

  if (esp_timer_get_time() - lastMmntTime >= startCheckingAfterUs) {
    if (newReadingsAvailable) {
    newReadingsAvailable = false;

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


      Serial.print("CO2[ppm]:");
      Serial.print(co2);
      Serial.print("\t");
      Serial.print("Temperature[℃]:");
      Serial.print(temp);
      Serial.print("\t");
      Serial.print("Humidity[%]:");
      Serial.println(hum);






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

  button_loop();
#ifdef SUPPORT_ARDUINOMENU
  nav.poll();
#endif
}