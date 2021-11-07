
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#include "index.h" //Web page header file

#if !defined WIFI_SSID_CREDENTIALS || !defined WIFI_PW_CREDENTIALS
#include "credentials.h"
#endif

WiFiClient espClient;
AsyncWebServer server(80);

char hostName[12];

void onWifiSettingsChanged(std::string ssid, std::string password) {
  Serial.print("WifiSetup: SSID = ");
  Serial.print(ssid.c_str());
  Serial.print(", Password = ");
  Serial.println(password.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
}

void initMDNS() {
  /*use mdns for host name resolution*/
  if (!MDNS.begin(hostName)) { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started. CO2 Gadget web interface at: http://");
  Serial.print(hostName);
  Serial.println(".local");
}

void initWifi() {
  uint8_t mac[6];
  int connectionTries = 0;
  int maxConnectionTries = 30;
  if (activeWIFI) {
    WiFi.mode(WIFI_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.macAddress(mac);
    Serial.print("rootTopic: ");
    Serial.println(rootTopic);
    sprintf(hostName, "%s-%x%x", rootTopic.c_str(), mac[4], mac[5]);
    Serial.printf("Setting hostname %s: %d\n", hostName,
                  WiFi.setHostname(hostName));
    WiFi.begin(WIFI_SSID_CREDENTIALS, WIFI_PW_CREDENTIALS);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      ++connectionTries;
      if (connectionTries==maxConnectionTries) {
        activeWIFI = false;
        Serial.printf("\nNot possible to connect to WiFi after %d tries.\nDisabling WiFi.\n", connectionTries);
        return;
      }
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.print("WiFi connected - IP = ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", MAIN_page);
    });
    server.on("/simple", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", SIMPLE_page);
    });
    server.on("/readCO2", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", String(co2));
    });
    server.on("/readTemperature", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", String(temp));
    });
    server.on("/readHumidity", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", String(hum));
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
      request->send(400, "text/plain", "Not found");
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
