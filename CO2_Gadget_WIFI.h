#ifndef CO2_Gadget_WIFI
#define CO2_Gadget_WIFI

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#if !defined WIFI_SSID_CREDENTIALS || !defined WIFI_PW_CREDENTIALS
// If not using enviroment variables, you must have a credentials.h file
#include "credentials.h"
#endif

WiFiClient espClient;
AsyncWebServer server(80);

void printSmallChar(char c, int row) {
    switch (c) {
        case '0':
            switch (row) {
                case 0:
                    Serial.print(" 000 ");
                    break;
                case 1:
                    Serial.print("0   0");
                    break;
                case 2:
                    Serial.print("0   0");
                    break;
                case 3:
                    Serial.print("0   0");
                    break;
                case 4:
                    Serial.print(" 000 ");
                    break;
            }
            break;
        case '1':
            switch (row) {
                case 0:
                    Serial.print("  1  ");
                    break;
                case 1:
                    Serial.print(" 11  ");
                    break;
                case 2:
                    Serial.print("  1  ");
                    break;
                case 3:
                    Serial.print("  1  ");
                    break;
                case 4:
                    Serial.print(" 111 ");
                    break;
            }
            break;
        case '2':
            switch (row) {
                case 0:
                    Serial.print(" 222 ");
                    break;
                case 1:
                    Serial.print("    2");
                    break;
                case 2:
                    Serial.print("  22 ");
                    break;
                case 3:
                    Serial.print(" 2   ");
                    break;
                case 4:
                    Serial.print(" 2222");
                    break;
            }
            break;
        case '3':
            switch (row) {
                case 0:
                    Serial.print(" 333 ");
                    break;
                case 1:
                    Serial.print("    3");
                    break;
                case 2:
                    Serial.print("  33 ");
                    break;
                case 3:
                    Serial.print("    3");
                    break;
                case 4:
                    Serial.print(" 333 ");
                    break;
            }
            break;
        case '4':
            switch (row) {
                case 0:
                    Serial.print("4   4");
                    break;
                case 1:
                    Serial.print("4   4");
                    break;
                case 2:
                    Serial.print("44444");
                    break;
                case 3:
                    Serial.print("    4");
                    break;
                case 4:
                    Serial.print("    4");
                    break;
            }
            break;
        case '5':
            switch (row) {
                case 0:
                    Serial.print("55555");
                    break;
                case 1:
                    Serial.print("5    ");
                    break;
                case 2:
                    Serial.print("555  ");
                    break;
                case 3:
                    Serial.print("    5");
                    break;
                case 4:
                    Serial.print("555  ");
                    break;
            }
            break;
        case '6':
            switch (row) {
                case 0:
                    Serial.print(" 666 ");
                    break;
                case 1:
                    Serial.print("6    ");
                    break;
                case 2:
                    Serial.print("6666 ");
                    break;
                case 3:
                    Serial.print("6   6");
                    break;
                case 4:
                    Serial.print(" 666 ");
                    break;
            }
            break;
        case '7':
            switch (row) {
                case 0:
                    Serial.print("77777");
                    break;
                case 1:
                    Serial.print("    7");
                    break;
                case 2:
                    Serial.print("   7 ");
                    break;
                case 3:
                    Serial.print("  7  ");
                    break;
                case 4:
                    Serial.print(" 7   ");
                    break;
            }
            break;
        case '8':
            switch (row) {
                case 0:
                    Serial.print(" 888 ");
                    break;
                case 1:
                    Serial.print("8   8");
                    break;
                case 2:
                    Serial.print(" 888 ");
                    break;
                case 3:
                    Serial.print("8   8");
                    break;
                case 4:
                    Serial.print(" 888 ");
                    break;
            }
            break;
        case '9':
            switch (row) {
                case 0:
                    Serial.print(" 999 ");
                    break;
                case 1:
                    Serial.print("9   9");
                    break;
                case 2:
                    Serial.print(" 9999");
                    break;
                case 3:
                    Serial.print("    9");
                    break;
                case 4:
                    Serial.print(" 999 ");
                    break;
            }
            break;
        case '.':
            switch (row) {
                case 0:
                    Serial.print("    ");
                    break;
                case 1:
                    Serial.print("    ");
                    break;
                case 2:
                    Serial.print("    ");
                    break;
                case 3:
                    Serial.print("    ");
                    break;
                case 4:
                    Serial.print("  o ");
                    break;
            }
            break;
        default:
            Serial.print("    ");  // Default character for unhandled
            break;
    }
}

void printLargeASCII(const char *text) {
    for (int row = 0; row < 5; row++) {  // 5 rows for each character
        for (int i = 0; i < strlen(text); i++) {
            printSmallChar(text[i], row);
            Serial.print(" ");  // Space between characters
        }
        Serial.println();  // New line after each row of characters
    }
}

void onWifiSettingsChanged(std::string ssid, std::string password) {
#ifdef WIFI_PRIVACY
    Serial.print("-->[WiFi] onWifiSettingsChanged() (SSID: ");
    Serial.print(ssid.c_str());
    Serial.println(")");
#else
    Serial.print("-->[WiFi] onWifiSettingsChanged() (SSID: ");
    Serial.print(ssid.c_str());
    Serial.print(", Password: ");
    Serial.print(password.c_str());
    Serial.println(")");
#endif
    WiFi.begin(ssid.c_str(), password.c_str());
}

String getMACAddressAsString() {
    byte mac[6];
    WiFi.macAddress(mac);

    String macAddress = String(mac[5], HEX) + ":" +
                        String(mac[4], HEX) + ":" +
                        String(mac[3], HEX) + ":" +
                        String(mac[2], HEX) + ":" +
                        String(mac[1], HEX) + ":" +
                        String(mac[0], HEX);

    return macAddress;
}

#include <WiFi.h>

void printDisconnectReason(int reasonCode) {
#ifdef DEBUG_WIFI_EVENTS
    switch (reasonCode) {
        case WIFI_REASON_UNSPECIFIED:
            Serial.println("-->[WiFi-event] Unspecified error");
            break;
        case WIFI_REASON_AUTH_EXPIRE:
            Serial.println("-->[WiFi-event] Authentication expired");
            break;
        case WIFI_REASON_AUTH_LEAVE:
            Serial.println("-->[WiFi-event] Authentication left");
            break;
        case WIFI_REASON_ASSOC_EXPIRE:
            Serial.println("-->[WiFi-event] Association expired");
            break;
        case WIFI_REASON_ASSOC_TOOMANY:
            Serial.println("-->[WiFi-event] Too many associations");
            break;
        case WIFI_REASON_NOT_AUTHED:
            Serial.println("-->[WiFi-event] Not authenticated");
            break;
        case WIFI_REASON_NOT_ASSOCED:
            Serial.println("-->[WiFi-event] Not associated");
            break;
        case WIFI_REASON_ASSOC_LEAVE:
            Serial.println("-->[WiFi-event] Association left");
            break;
        case WIFI_REASON_ASSOC_NOT_AUTHED:
            Serial.println("-->[WiFi-event] Association not authenticated");
            break;
        case WIFI_REASON_DISASSOC_PWRCAP_BAD:
            Serial.println("-->[WiFi-event] Disassociation power capability invalid");
            break;
        case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
            Serial.println("-->[WiFi-event] Disassociation supported channel invalid");
            break;
        case WIFI_REASON_IE_INVALID:
            Serial.println("-->[WiFi-event] Invalid IE");
            break;
        case WIFI_REASON_MIC_FAILURE:
            Serial.println("-->[WiFi-event] MIC failure");
            break;
        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
            Serial.println("-->[WiFi-event] 4-way handshake timeout");
            break;
        case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
            Serial.println("-->[WiFi-event] Group key update timeout");
            break;
        case WIFI_REASON_IE_IN_4WAY_DIFFERS:
            Serial.println("-->[WiFi-event] IE in 4-way differs");
            break;
        case WIFI_REASON_GROUP_CIPHER_INVALID:
            Serial.println("-->[WiFi-event] Invalid group cipher");
            break;
        case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
            Serial.println("-->[WiFi-event] Invalid pairwise cipher");
            break;
        case WIFI_REASON_AKMP_INVALID:
            Serial.println("-->[WiFi-event] Invalid AKMP");
            break;
        case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
            Serial.println("-->[WiFi-event] Unsupported RSN IE version");
            break;
        case WIFI_REASON_INVALID_RSN_IE_CAP:
            Serial.println("-->[WiFi-event] Invalid RSN IE capabilities");
            break;
        case WIFI_REASON_802_1X_AUTH_FAILED:
            Serial.println("-->[WiFi-event] 802.1X authentication failed");
            break;
        case WIFI_REASON_CIPHER_SUITE_REJECTED:
            Serial.println("-->[WiFi-event] Cipher suite rejected");
            break;
        case WIFI_REASON_BEACON_TIMEOUT:
            Serial.println("-->[WiFi-event] Beacon timeout");
            break;
        case WIFI_REASON_NO_AP_FOUND:
            Serial.println("-->[WiFi-event] No AP found");
            break;
        case WIFI_REASON_AUTH_FAIL:
            Serial.println("-->[WiFi-event] Authentication failed");
            break;
        case WIFI_REASON_ASSOC_FAIL:
            Serial.println("-->[WiFi-event] Association failed");
            break;
        case WIFI_REASON_HANDSHAKE_TIMEOUT:
            Serial.println("-->[WiFi-event] Handshake timeout");
            break;
        default:
            Serial.println("-->[WiFi-event] Unknown reason");
            break;
    }
#endif  // DEBUG_WIFI_EVENTS
}

void printWiFiStatus() {  // Print wifi status on serial monitor

    // Get current status
    //  WL_CONNECTED: assigned when connected to a WiFi network;
    //  WL_NO_SHIELD: assigned when no WiFi shield is present;
    //  WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
    //  WL_NO_SSID_AVAIL: assigned when no SSID are available;
    //  WL_SCAN_COMPLETED: assigned when the scan networks is completed;
    //  WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
    //  WL_CONNECTION_LOST: assigned when the connection is lost;
    //  WL_DISCONNECTED: assigned when disconnected from a network;

    Serial.print("-->[WiFi-Status] ");
    switch (WiFi.status()) {
        case WL_CONNECTED:
            Serial.println("WiFi connected");
            break;
        case WL_NO_SHIELD:
            Serial.println("No WiFi HW detected");
            break;
        case WL_IDLE_STATUS:
            Serial.println("Attempting...");
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("No SSID available");
            break;
        case WL_SCAN_COMPLETED:
            Serial.println("Networks scan completed");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Connect failed");
            break;
        case WL_CONNECTION_LOST:
            Serial.println("Connection lost");
            break;
        case WL_DISCONNECTED:
            Serial.println("Disconnected");
            break;
        default:
            Serial.println("Unknown status");
            break;
    }

    // Print the SSID of the network you're attached to:
    Serial.print("-->[WiFi] SSID: ");
    Serial.println(WiFi.SSID());

    // Print your WiFi shield's IP address:
    Serial.print("-->[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());

    // Print your WiFi shield's MAC address:
    Serial.print("-->[WiFi] MAC Address: ");
    MACAddress = getMACAddressAsString();
    Serial.println(MACAddress);

    // Print the received signal strength:
    Serial.print("-->[WiFi] Signal strength (RSSI):");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    /*
  // Print authentication used:
  Serial.print("Encryption type: ");
  switch (WiFi.encryptionType()) {
    case WIFI_AUTH_OPEN:
      Serial.println("Open WiFi");
      break;
    case WIFI_AUTH_WEP:
      Serial.println("WEP");
      break;
    case WIFI_AUTH_WPA_PSK:
      Serial.println("WPA-PSK");
      break;
    case WIFI_AUTH_WPA2_PSK:
      Serial.println("WPA2-PSK");
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      Serial.println("WPA-WPA2-PSK");
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      Serial.println("WPA2-Enterprise");
      break;
    default:
      Serial.println("Unknown encryption type");
      break;
  }
  */
}

String getWiFiDisconnectReason(uint8_t reason) {
    switch (reason) {
        case WIFI_REASON_AUTH_EXPIRE:
            return "Auth Expired";
        case WIFI_REASON_AUTH_LEAVE:
            return "Auth Leave";
        case WIFI_REASON_ASSOC_EXPIRE:
            return "Association Expired";
        case WIFI_REASON_ASSOC_TOOMANY:
            return "Too Many Associations";
        case WIFI_REASON_NOT_AUTHED:
            return "Not Authenticated";
        case WIFI_REASON_NOT_ASSOCED:
            return "Not Associated";
        case WIFI_REASON_ASSOC_LEAVE:
            return "Association Leave";
        case WIFI_REASON_ASSOC_NOT_AUTHED:
            return "Association not Authenticated";
        case WIFI_REASON_DISASSOC_PWRCAP_BAD:
            return "Disassociate Power Cap Bad";
        case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
            return "Disassociate Supported Channel Bad";
        case WIFI_REASON_IE_INVALID:
            return "IE Invalid";
        case WIFI_REASON_MIC_FAILURE:
            return "Mic Failure";
        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
            return "4-Way Handshake Timeout";
        case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
            return "Group Key Update Timeout";
        case WIFI_REASON_IE_IN_4WAY_DIFFERS:
            return "IE In 4-Way Handshake Differs";
        case WIFI_REASON_GROUP_CIPHER_INVALID:
            return "Group Cipher Invalid";
        case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
            return "Pairwise Cipher Invalid";
        case WIFI_REASON_AKMP_INVALID:
            return "AKMP Invalid";
        case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
            return "Unsupported RSN IE version";
        case WIFI_REASON_INVALID_RSN_IE_CAP:
            return "Invalid RSN IE Cap";
        case WIFI_REASON_802_1X_AUTH_FAILED:
            return "802.1x Authentication Failed";
        case WIFI_REASON_CIPHER_SUITE_REJECTED:
            return "Cipher Suite Rejected";
        case WIFI_REASON_BEACON_TIMEOUT:
            return "Beacon Timeout";
        case WIFI_REASON_NO_AP_FOUND:
            return "AP Not Found";
        case WIFI_REASON_AUTH_FAIL:
            return "Authentication Failed";
        case WIFI_REASON_ASSOC_FAIL:
            return "Association Failed";
        case WIFI_REASON_HANDSHAKE_TIMEOUT:
            return "Handshake Failed";
        case WIFI_REASON_CONNECTION_FAIL:
            return "Connection Failed";
        case WIFI_REASON_UNSPECIFIED:
        default:
            return "Unspecified";
    }
}

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
#ifdef DEBUG_WIFI_EVENTS
    Serial.printf("-->[WiFi-event] event: %d - ", event);

    switch (event) {
        case SYSTEM_EVENT_WIFI_READY:
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default:
            break;
    }
#endif  // DEBUG_WIFI_EVENTS
}

void initMDNS() {
#ifdef SUPPORT_MDNS
    /*use mdns for host name resolution*/
    if (!MDNS.begin(hostName.c_str())) {  // http://esp32.local
        Serial.println("-->[WiFi] Error setting up MDNS responder!");
        while (1) {
            delay(100);
        }
    }
    // Serial.print("-->[WiFi] mDNS responder started. CO2 Gadget web interface at: http://");
    // Serial.print(hostName);
    // Serial.println(".local");
    // Serial.printf("-->[WiFi] mDNS responder started. CO2 Gadget web interface at: http://%s.local\n", hostName.c_str());
    Serial.println("-->[WiFi] mDNS responder started. CO2 Gadget web interface at: http://" + hostName + ".local");
    MDNS.addService("http", "tcp", 80);
#endif
}

void disableWiFi() {
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    Serial.println("-->[WiFi] WiFi dissabled!");
}

// Replaces placeholder with actual values
String processor(const String &var) {
    // Serial.println(var);
    if (var == "CO2") {
        return String(co2);
    } else if (var == "TEMPERATURE") {
        return String(temp);
    } else if (var == "HUMIDITY") {
        return String(hum);
    }
    return String();
}

bool checkStringIsNumerical(String myString) {
    uint16_t Numbers = 0;

    for (uint16_t i = 0; i < myString.length(); i++) {
        if (myString[i] == '0' || myString[i] == '1' || myString[i] == '2' ||
            myString[i] == '3' || myString[i] == '4' || myString[i] == '5' ||
            myString[i] == '6' || myString[i] == '7' || myString[i] == '8' ||
            myString[i] == '9') {
            Numbers++;
        }
    }

    if (Numbers == myString.length()) {
        return true;
    } else {
        return false;
    }
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
#ifdef DEBUG_WIFI_EVENTS
    Serial.println("-->[WiFi-event] Connected to WiFi access point");
#endif
}

void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    WiFiConnectionRetries = 0;
    timeTroubledWIFI = 0;
    troubledMQTT = false;
    troubledWIFI = false;
#ifdef DEBUG_WIFI_EVENTS
    Serial.println("-->[WiFi-event] WiFi got IP)");
    Serial.print("-->[WiFi-event] IP address: ");
    Serial.println(WiFi.localIP());
#endif
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    ++WiFiConnectionRetries;
    Serial.println("-->[WiFi] Disconnected from WiFi access point. Reason: " + getWiFiDisconnectReason(info.wifi_sta_disconnected.reason) + " (" + String(info.wifi_sta_disconnected.reason) + ") Retries: " + String(WiFiConnectionRetries) + " of " + String(maxWiFiConnectionRetries));
#ifdef DEBUG_WIFI_EVENTS
    Serial.println("-->[WiFi-event] Disconnected from WiFi access point");
    Serial.print("-->[WiFi-event] WiFi lost connection. Reason: (");
    Serial.print(info.wifi_sta_disconnected.reason);
    Serial.print(") ");
    printDisconnectReason(info.wifi_sta_disconnected.reason);
    Serial.print("-->[WiFi-event] Retries: ");
    Serial.print(WiFiConnectionRetries);
    Serial.print(" of ");
    Serial.println(maxWiFiConnectionRetries);
#endif

    if (WiFiConnectionRetries >= maxWiFiConnectionRetries) {
        disableWiFi();
        troubledWIFI = true;
        Serial.println("-->[WiFi-event] WiFi troubled");

        timeTroubledWIFI = millis();
#ifdef DEBUG_WIFI_EVENTS
        Serial.println("-->[WiFi-event] Not possible to connect to WiFi after " + String(WiFiConnectionRetries) + " tries. Will try later.");
#endif
    }
}

String getCO2GadgetStatusAsJson() {
    StaticJsonDocument<512> doc;
    doc["CO2"] = co2;
    doc["Temperature"] = temp;
    doc["Humidity"] = hum;
    doc["WiFiStatus"] = WiFi.status();
    doc["SSID"] = WiFi.SSID();
#ifndef WIFI_PRIVACY
    doc["wifiPass"] = wifiPass;
#endif
    doc["IP"] = WiFi.localIP().toString();
    doc["RSSI"] = WiFi.RSSI();
    doc["MACAddress"] = MACAddress;
    doc["hostName"] = hostName;
#ifdef SUPPORT_MQTT
    doc["rootTopic"] = rootTopic;
    doc["discoveryTopic"] = discoveryTopic;
    doc["mqttClientId"] = mqttClientId;
    doc["mqttBroker"] = mqttBroker;
    doc["mqttUser"] = mqttUser;
#ifndef WIFI_PRIVACY
    doc["mqttPass"] = mqttPass;
#endif
#ifdef SUPPORT_ESPNOW
    char peerAddressString[18];
    snprintf(peerAddressString, sizeof(peerAddressString),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
             peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
    doc["peerESPNowAddress"] = peerAddressString;
#endif
#endif
    doc["activeWIFI"] = activeWIFI;
#ifdef SUPPORT_MQTT
    doc["activeMQTT"] = activeMQTT;
#endif
#ifdef SUPPORT_BLE
    doc["activeBLE"] = activeBLE;
#endif
#ifdef SUPPORT_OTA
    doc["activeOTA"] = activeOTA;
#endif
    doc["troubledWiFi"] = troubledWIFI;
#ifdef SUPPORT_MQTT
    doc["troubledMQTT"] = troubledMQTT;
#endif
#ifdef SUPPORT_ESPNOW
    doc["troubledESPNOW"] = troubledESPNOW;
#endif
    doc["measurementInterval"] = measurementInterval;
    doc["calibrationValue"] = calibrationValue;
    doc["pendingCalibration"] = pendingCalibration;
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["uptime"] = millis();
    String output;
    serializeJson(doc, output);
    return output;
}

const char *PARAM_INPUT_1 = "MeasurementInterval";
const char *PARAM_INPUT_2 = "CalibrateCO2";
const char *PARAM_INPUT_3 = "SetVRef";

void initWebServer() {
    SPIFFS.begin();

    // server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server.serveStatic("/", SPIFFS, "/index.html");
    // server.serveStatic("/index.html", SPIFFS, "/index.html");
    // server.serveStatic("/preferences.html", SPIFFS, "/preferences.html");
    // server.serveStatic("/status.html", SPIFFS, "/status.html");
    // server.serveStatic("favicon.ico", SPIFFS, "/favicon.png");
    // server.serveStatic("main.js", SPIFFS, "/main.js");
    // server.serveStatic("style.css", SPIFFS, "/style.css");
    // server.serveStatic("preferences.js", SPIFFS, "/preferences.js");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/preferences.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/preferences.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/status.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/status.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/low_power.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/low_power.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/main.js.gz", "application/javascript");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/preferences.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/preferences.js.gz", "application/javascript");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/style.css.gz", "text/css");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/favicon.png.gz", "image/png");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
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

    server.on("/readBatteryVoltage", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(batteryVoltage));
    });

    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        String inputString;
        restartTimerToDeepSleep();
        // <CO2-GADGET_IP>/settings?MeasurementInterval=100
        if (request->hasParam("MeasurementInterval")) {
            inputString = request->getParam("MeasurementInterval")->value();
            if (checkStringIsNumerical(inputString)) {
                // Serial.printf("-->[WiFi] Received /settings command MeasurementInterval with parameter %s\n", inputString);
                Serial.println("-->[WiFi] Received /settings command MeasurementInterval with parameter " + inputString);
                measurementInterval = inputString.toInt();
                request->send(200, "text/plain", "OK. Setting MeasurementInterval to " + inputString + ", please re-calibrate your sensor.");
            } else {
                request->send(400, "text/plain", "Error. MeasurementInterval must have a number as parameter.");
            }
        };
        // <CO2-GADGET_IP>/settings?CalibrateCO2=400
        if (request->hasParam("CalibrateCO2")) {
            inputString = request->getParam("CalibrateCO2")->value();
            if (checkStringIsNumerical(inputString)) {
                // Serial.printf("-->[WiFi] Received /settings command CalibrateCO2 with parameter %s\n", inputString);
                Serial.println("-->[WiFi] Received /settings command CalibrateCO2 with parameter: " + inputString);
                if ((inputString.toInt() >= 400) && (inputString.toInt() <= 2000)) {
                    calibrationValue = inputString.toInt();
                    pendingCalibration = true;
                    request->send(200, "text/plain", "OK. Recalibrating CO2 sensor to " + inputString);
                } else {
                    request->send(400, "text/plain", "Error. CO2 calibration value must be between 400 and 2000");
                }
            }
        };
        // <CO2-GADGET_IP>/settings?displayShowBatteryVoltage=true
        if (request->hasParam("displayShowBatteryVoltage")) {
            String inputString = request->getParam("displayShowBatteryVoltage")->value();
            if (inputString.equalsIgnoreCase("true") || inputString.equalsIgnoreCase("false")) {
                bool newValue = inputString.equalsIgnoreCase("true");
                displayShowBatteryVoltage = newValue;
                // Serial.printf("-->[WiFi] Received /settings command displayShowBatteryVoltage with parameter: %s\n", newValue ? "true" : "false");
                Serial.println("-->[WiFi] Received /settings command displayShowBatteryVoltage with parameter: " + inputString);
                request->send(200, "text/plain", "OK. Setting displayShowBatteryVoltage to " + inputString);
            } else {
                request->send(400, "text/plain", "Error. Invalid parameter. Use 'true' or 'false'");
            }
        }
        // <CO2-GADGET_IP>/settings?SetVRef=930
        if (request->hasParam("SetVRef")) {
            inputString = request->getParam("SetVRef")->value();
            if (checkStringIsNumerical(inputString)) {
                // Serial.printf("-->[WiFi] Received /settings command SetVRef with parameter %s\n", inputString);
                Serial.println("-->[WiFi] Received /settings command SetVRef with parameter: " + inputString);
                battery.begin(vRef, voltageDividerRatio, &asigmoidal);
                vRef = inputString.toFloat();
                request->send(200, "text/plain", "OK. Setting VRef to " + inputString);
            } else {
                request->send(400, "text/plain", "Error. VRef must have a number as parameter.");
            }
        }
    });

    server.on("/getPreferences", HTTP_GET, [](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        String preferencesJson = getPreferencesAsJson();
        request->send(200, "application/json", preferencesJson);
    });

    server.on("/getActualSettingsAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        String preferencesJson = getActualSettingsAsJson();
        request->send(200, "application/json", preferencesJson);
    });

    server.on("/getVersion", HTTP_GET, [](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        String versionJson = getCO2GadgetVersionAsJson();
        request->send(200, "application/json", versionJson);
    });

    server.on("/getMeasurementInterval", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(measurementInterval));
    });

    server.on("/getFreeHeap", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        String statusJson = getCO2GadgetStatusAsJson();
        request->send(200, "application/json", statusJson);
    });

    // Trigger a software reset
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        request->send(200, "text/plain", "ESP32 restart initiated");
        delay(100);
        ESP.restart();
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        request->send(400, "text/plain", "Not found");
    });

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/savepreferences", [](AsyncWebServerRequest *request, JsonVariant &json) {
        restartTimerToDeepSleep();
        StaticJsonDocument<2048> data;
        if (json.is<JsonArray>()) {
            data = json.as<JsonArray>();
        } else if (json.is<JsonObject>()) {
            data = json.as<JsonObject>();
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
        // Serial.print("-->[WiFi] Received /savepreferences command with parameter: ");
        // Serial.println(response);
        handleSavePreferencesfromJSON(response);
    });

    server.addHandler(handler);
}

void customWiFiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info) {
    // Check if the event is one of the events you are already handling
    if (event != WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED &&
        event != WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP &&
        event != WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        // Call the WiFiEvent function only if the event is not one of the above
        WiFiEvent(event, info);
    }
}

// easy to use helper-function for non-blocking timing
boolean TimePeriodIsOver(unsigned long &startOfPeriod, unsigned long TimePeriod) {
    unsigned long currentMillis = millis();
    if (currentMillis - startOfPeriod >= TimePeriod) {
        // more time than TimePeriod has elapsed since last time if-condition was true
        startOfPeriod = currentMillis;  // a new period starts right here so set new starttime
        return true;
    } else
        return false;  // actual TimePeriod is NOT yet over
}

bool connectToWiFi() {
    displayNotification("Init WiFi", notifyInfo);
    Serial.print("\n-->[WiFi] Initializing WiFi...\n");
    WiFi.disconnect(true);  // disconnect form wifi to set new wifi connection
    delay(100);
    WiFi.mode(WIFI_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(hostName.c_str());
    Serial.println("-->[WiFi] Setting hostname: " + hostName);
#ifdef WIFI_PRIVACY
    Serial.println("-->[WiFi] Connecting to WiFi (SSID: " + wifiSSID + ")");
#else
    Serial.println("-->[WiFi] Connecting to WiFi (SSID: " + wifiSSID + " with password: " + wifiPass + ")");
#endif

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiStationGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(customWiFiEventHandler);

    unsigned long checkTimer = 0;  // Timer-variables MUST be of type unsigned long
    troubledWIFI = false;
    WiFiConnectionRetries = 0;

    WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());

    // Wait for connection until maxWiFiConnectionRetries or WiFi is connected
    while (WiFi.status() != WL_CONNECTED && WiFiConnectionRetries < maxWiFiConnectionRetries) {
        if (TimePeriodIsOver(checkTimer, 500)) {  // Once every 500 miliseconds
            Serial.print(".");
            WiFiConnectionRetries++;
            if (WiFiConnectionRetries > maxWiFiConnectionRetries) {
                Serial.println();
                Serial.print("not connected ");
            }
        }
        yield();
    }

    if ((WiFiConnectionRetries > maxWiFiConnectionRetries) && (WiFi.status() != WL_CONNECTED)) {
        disableWiFi();
        troubledWIFI = true;
        timeTroubledWIFI = millis();
        // Serial.printf("-->[WiFi] Not possible to connect to WiFi after %d tries. Will try later.\n", WiFiConnectionRetries);
        Serial.println("-->[WiFi] Troubled. Not possible to connect to WiFi after " + String(WiFiConnectionRetries) + " tries. Will try later.");
    }

    if ((troubledWIFI) || (WiFi.status() != WL_CONNECTED)) {
        Serial.println("");
        return false;
    } else {
        Serial.println("");
        saveWifiCredentials();
        Serial.print("-->[WiFi] MAC: ");
        Serial.println(MACAddress);
        Serial.print("-->[WiFi] WiFi connected - IP = ");
        Serial.println(WiFi.localIP());
        return true;
    }
}

void initOTA() {
#ifdef SUPPORT_OTA
    AsyncElegantOTA.begin(&server);
    Serial.println("-->[WiFi] OTA ready");
#endif
}

void initWifi() {
    if (wifiSSID == "") {
        activeWIFI = false;
    }
    if (activeWIFI) {
        if (!connectToWiFi()) {
            wifiChanged = false;
            return;
        }

        initWebServer();
        initMDNS();
        initOTA();

        server.begin();
        Serial.println("-->[WiFi] HTTP server started");
        printWiFiStatus();

        troubledMQTT = false;  // Try to connect to MQTT broker on next loop if needed
    }
}

void wifiClientLoop() {
    if (isDownloadingBLE) return;
    if (activeWIFI && troubledWIFI && (millis() - timeTroubledWIFI >= timeToRetryTroubledWIFI * 1000)) {
        initWifi();
    }

    // // This is a workaround until I can directly determine whether the Wi-Fi data has been changed via BLE
    // // Only checks for SSID changed (not password)
    // if ((WiFi.SSID() != wifiSSID) && (!inMenu) && (WiFi.SSID() != "")) {
    //     Serial.println("-->[WiFi] Wi-Fi SSID changed. Old SSID: " + wifiSSID + ", new SSID: " + WiFi.SSID());
    //     Serial.println("-->[WiFi] IP address: " + WiFi.localIP().toString());
    //     Serial.println("-->[WiFi] RSSI: " + String(WiFi.RSSI()) + " dBm");
    //     wifiSSID = WiFi.SSID();
    //     activeWIFI = true;
    //     if (wifiSSID != "") {
    //         putPreferences();
    //         wifiChanged = true;
    //     }
    // }

    if ((wifiChanged) && (!inMenu)) {
        wifiChanged = false;
        initWifi();
    }
}

void OTALoop() {
#ifdef SUPPORT_OTA
    if (isDownloadingBLE) return;
    if ((activeWIFI) && (activeOTA) && (!troubledWIFI) && (WiFi.status() == WL_CONNECTED)) {
        AsyncElegantOTA.loop();
    }
#endif
}

#endif  // CO2_Gadget_WIFI