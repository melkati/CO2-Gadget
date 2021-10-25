
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#ifdef SUPPORT_WIFI
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
// #include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index.h" //Web page header file

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