
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

// #include "index.h" //Web page header file

#if !defined WIFI_SSID_CREDENTIALS || !defined WIFI_PW_CREDENTIALS
#include "credentials.h"
#endif

WiFiClient espClient;
AsyncWebServer server(80);

void onWifiSettingsChanged(std::string ssid, std::string password) {
  Serial.print("WifiSetup: SSID = ");
  Serial.print(ssid.c_str());
  Serial.print(", Password = ");
  Serial.println(password.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
}

void initWifi() {
  char hostName[12];
  uint8_t mac[6];
  uint16_t connectionRetries = 0;
  uint16_t maxConnectionRetries = 30;
  if (activeWIFI) {
    WiFi.mode(WIFI_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.macAddress(mac);
    Serial.print("rootTopic: ");
    Serial.println(rootTopic);
    sprintf(hostName, "%s-%x%x", rootTopic.c_str(), mac[4], mac[5]);
    Serial.printf("Setting hostname %s: %d\n", hostName,
                  WiFi.setHostname(hostName));
    WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      ++connectionRetries;
      if (connectionRetries==maxConnectionRetries) {
        activeWIFI = false;
        Serial.printf("\nNot possible to connect to WiFi after %d tries.\nDisabling WiFi.\n", connectionRetries);
        return;
      }
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.print("WiFi connected - IP = ");
    Serial.println(WiFi.localIP());

    /*use mdns for host name resolution*/
    if (!MDNS.begin(hostName)) { // http://esp32.local
      Serial.println("Error setting up MDNS responder!");
      while (1) {
        delay(1000);
      }
    }
    // Serial.printf("mDNS responder started. CO2 Gadget web interface at: http:\\\\%s\n",hostName);
    Serial.print(
        "mDNS responder started. CO2 Gadget web interface at: http://");
    Serial.print(hostName);
    Serial.println(".local");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "CO2: " + String(co2) + " PPM");
      //  server.on("/", handleRoot);      //This is display page
      //  server.on("/readADC", handleADC);//To get update of ADC Value only
    });

#ifdef SUPPORT_OTA
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    Serial.println("OTA ready");
#endif

    server.begin();
    Serial.println("HTTP server started");

#ifdef SUPPORT_MQTT
    mqttClientId = hostName;
#endif
  }
}