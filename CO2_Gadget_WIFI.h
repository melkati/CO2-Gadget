
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#ifdef SUPPORT_WIFI

#include "index.h" //Web page header file

#include "credentials.h"
WiFiClient espClient;
// WiFiMulti WiFiMulti;
AsyncWebServer server(80);

void onWifiSettingsChanged(std::string ssid, std::string password) {
  Serial.print("WifiSetup: SSID = ");
  Serial.print(ssid.c_str());
  Serial.print(", Password = ");
  Serial.println(password.c_str());

  // WiFiMulti.addAP(ssid.c_str(), password.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
}

void initWifi() {
  char hostName[12];
  uint8_t mac[6];
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.macAddress(mac);
  Serial.print("rootTopic: "); Serial.println(rootTopic);
  sprintf(hostName, "%s-%x%x", rootTopic.c_str(), mac[4], mac[5]);
  Serial.printf("Setting hostname %s: %d\n", hostName, WiFi.setHostname(hostName));
  WiFi.begin(WIFI_SSID_CREDENTIALS, WIFI_PW_CREDENTIALS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  delay(5000);

  /*use mdns for host name resolution*/
  if (!MDNS.begin(UNITHOSTNAME)) { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started. CO2 monitor web interface at: http://");
  Serial.print(UNITHOSTNAME);
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
}
////===============================================================
//// This function is called when you open its IP in browser
////===============================================================
// void handleRoot() {
// String s = MAIN_page; //Read HTML contents
// server.send(200, "text/html", s); //Send web page
//}
//
// void handleADC() {
// int a = analogRead(A0);
// String co2Value = String(co2);
//
// server.send(200, "text/plane", co2Value); //Send ADC value only to client
// ajax request
//}
#endif