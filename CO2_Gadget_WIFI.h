#ifndef CO2_Gadget_WIFI
#define CO2_Gadget_WIFI

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP WIFI                                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#include <Arduino.h>

#ifdef SUPPORT_CAPTIVE_PORTAL
DNSServer dnsServer;
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
    Serial.println("-->[WiFi] WiFi disabled!");
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
#ifdef SUPPORT_CAPTIVE_PORTAL
    if (captivePortalActive) return;
#endif
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

String getCO2GadgetFeaturesAsJson() {
    JsonDocument doc;
#ifdef SUPPORT_BLE
    doc["BLE"] = true;
#else
    doc["BLE"] = false;
#endif
#ifdef SUPPORT_BUZZER
    doc["Buzzer"] = true;
#else
    doc["Buzzer"] = false;
#endif
#ifdef SUPPORT_ESPNOW
    doc["ESPNow"] = true;
#else
    doc["ESPNow"] = false;
#endif
#ifdef SUPPORT_MDNS
    doc["mDNS"] = true;
#else
    doc["mDNS"] = false;
#endif
#ifdef SUPPORT_MQTT
    doc["MQTT"] = true;
#else
    doc["MQTT"] = false;
#endif
#ifdef SUPPORT_MQTT_DISCOVERY
    doc["MQTTDiscovery"] = true;
#else
    doc["MQTTDiscovery"] = false;
#endif
#ifdef SUPPORT_OTA
    doc["OTA"] = true;
#else
    doc["OTA"] = false;
#endif
#ifdef SUPPORT_TFT
    doc["TFT"] = true;
#else
    doc["TFT"] = false;
#endif
#ifdef SUPPORT_EINK
    doc["EINK"] = true;
#else
    doc["EINK"] = false;
#endif
#ifdef SUPPORT_OLED
    doc["OLED"] = true;
#else
    doc["OLED"] = false;
#endif
#ifdef SUPPORT_CAPTIVE_PORTAL
    doc["CaptivePortal"] = true;
#else
    doc["CaptivePortal"] = false;
#endif

    String output;
    serializeJson(doc, output);
    return output;
}

String getCO2GadgetStatusAsJson() {
    StaticJsonDocument<512> doc;
    doc["mainDeviceSelected"] = mainDeviceSelected;
    doc["CO2"] = co2;
    doc["Temperature"] = String(temp, 2);
    doc["Humidity"] = String(hum, 0);
    doc["WiFiStatus"] = WiFi.status();
    doc["SSID"] = WiFi.SSID();
#ifndef WIFI_PRIVACY
    doc["wifiPass"] = wifiPass;
#endif
    doc["IP"] = WiFi.localIP().toString();
    doc["RSSI"] = WiFi.RSSI();
    doc["MACAddress"] = MACAddress;
    doc["hostName"] = hostName;
    doc["useStaticIP"] = useStaticIP;
    doc["staticIP"] = staticIP.toString();
    doc["gateway"] = gateway.toString();
    doc["subnet"] = subnet.toString();
    doc["dns1"] = dns1.toString();
    doc["dns2"] = dns2.toString();
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
    doc["sampleInterval"] = sampleInterval;
    doc["calibrationValue"] = calibrationValue;
    doc["pendingCalibration"] = pendingCalibration;
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["minFreeHeap"] = ESP.getMinFreeHeap();
    doc["uptime"] = millis();

    // Low power preferences
    doc["lowPowerMode"] = deepSleepData.lowPowerMode;
    doc["waitToDeep"] = deepSleepData.waitToGoDeepSleepOn1stBoot;
    doc["timeSleeping"] = deepSleepData.timeSleeping;
    doc["cyclsWifiConn"] = deepSleepData.cyclesLeftToWiFiConnect;
    doc["cycRedrawDis"] = deepSleepData.cyclesLeftToRedrawDisplay;
    doc["actBLEOnWake"] = deepSleepData.activeBLEOnWake;
    doc["actWifiOnWake"] = deepSleepData.activeWifiOnWake;
    doc["actMQTTOnWake"] = deepSleepData.sendMQTTOnWake;
    doc["actESPnowWake"] = deepSleepData.sendESPNowOnWake;
    doc["displayOnWake"] = deepSleepData.displayOnWake;

    String output;
    serializeJson(doc, output);
    return output;
}

String getCaptivePortalStatusAsJson() {
    StaticJsonDocument<256> doc;
    doc["captivePortalActive"] = captivePortalActive;
    doc["forceCaptivePortalActive"] = forceCaptivePortalActive;
    doc["captivePortalNoTimeout"] = captivePortalNoTimeout;
    doc["timeCaptivePortalStarted"] = timeCaptivePortalStarted;
    doc["timeToWaitForCaptivePortal"] = timeToWaitForCaptivePortal;
    if (relaxedSecurity) doc["relaxedSecurity"] = relaxedSecurity;
    // timeToWaitForCaptivePortal in seconds timeCaptivePortalStarted in milliseconds captivePortalTimeLeft in seconds
    if (captivePortalNoTimeout) {  // If Captive Portal is active
        doc["captivePortalTimeLeft"] = 0;
    } else {
        doc["captivePortalTimeLeft"] = (timeToWaitForCaptivePortal - (millis() - timeCaptivePortalStarted) / 1000);
    }
    if (captivePortalDebug) doc["captivePortalDebug"] = captivePortalDebug;

#ifdef DEBUG_CAPTIVE_PORTAL
        // doc["captivePortalDebug"] = true;
#endif

    String output;
    serializeJson(doc, output);
    return output;
}

String getWifiNetworksAsJson() {
    StaticJsonDocument<1024> doc;
    JsonArray networks = doc.createNestedArray("networks");
    int numNetworks = WiFi.scanNetworks();
    for (int i = 0; i < numNetworks; i++) {
        JsonObject network = networks.createNestedObject();
        network["SSID"] = WiFi.SSID(i);
        network["RSSI"] = WiFi.RSSI(i);
        network["Encryption"] = WiFi.encryptionType(i);
    }
    String output;
    serializeJson(doc, output);
    return output;
}

bool handleCaptivePortalSettings(String captivePortalPreferences) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, captivePortalPreferences);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }
#ifdef DEBUG_CAPTIVE_PORTAL
    Serial.println("-->[CAPT] Received Captive Portal Settings: " + captivePortalPreferences);
#endif
    if (doc.containsKey("captivePortalActive")) {
#ifdef DEBUG_CAPTIVE_PORTAL
        // Serial.print("-->[CAPT] Received Captive Portal Settings: ");
        // Serial.println(captivePortalPreferences);
        Serial.println("-->[CAPT] Received Captive Portal Setting captivePortalActive:          " + String(doc["captivePortalActive"] ? "Enabled" : "Disabled") + " (Actual value: " + String(captivePortalActive ? "Enabled" : "Disabled") + ")");
        Serial.println("-->[CAPT] Received Captive Portal Setting forceCaptivePortalActive:     " + String(doc["forceCaptivePortalActive"] ? "Enabled" : "Disabled") + " (Actual value: " + String(forceCaptivePortalActive ? "Enabled" : "Disabled") + ")");
        Serial.println("-->[CAPT] Received Captive Portal Setting captivePortalNoTimeout:       " + String(doc["captivePortalNoTimeout"] ? "Enabled" : "Disabled") + " (Actual value: " + String(captivePortalNoTimeout ? "Enabled" : "Disabled") + ")");
        Serial.println("-->[CAPT] Received Captive Portal Setting timeCaptivePortalStarted:     " + String(doc["timeCaptivePortalStarted"].as<unsigned long>()) + " (Actual value: " + String(timeCaptivePortalStarted) + ")");
        Serial.println("-->[CAPT] Received Captive Portal Setting timeToWaitForCaptivePortal:   " + String(doc["timeToWaitForCaptivePortal"].as<unsigned long>()) + " (Actual value: " + String(timeToWaitForCaptivePortal) + ")");
        Serial.println("-->[CAPT] Received Captive Portal Setting captivePortalDebug:           " + String(doc["captivePortalDebug"] ? "Enabled" : "Disabled") + " (Actual value: " + String(captivePortalDebug ? "Enabled" : "Disabled") + ")");
#endif
        if ((doc["forceCaptivePortalActive"] == true) && (!forceCaptivePortalActive)) {  // If test captive portal is enabled and it was not enabled before
            captivePortalActive = true;
            forceCaptivePortalActive = true;
            timeCaptivePortalStarted = millis();
            Serial.println("-->[CAPT] *** Activating Captive Portal for testing");

        } else if ((doc["forceCaptivePortalActive"] == false) && (forceCaptivePortalActive)) {  // If test captive portal is disabled and it was enabled before
            captivePortalActive = false;
            forceCaptivePortalActive = false;
            Serial.println("-->[CAPT] *** Deactivating Captive Portal for testing");

        } else if ((doc["captivePortalActive"] == true) && (!captivePortalActive)) {  // If captive portal is enabled and it was not enabled before
            captivePortalActive = true;
            timeCaptivePortalStarted = millis();
            Serial.println("-->[CAPT] *** Activating Captive Portal");
        }
#ifdef DEBUG_CAPTIVE_PORTAL
        Serial.println("-->[CAPT] Captive Portal enabled for testing:                           " + String(forceCaptivePortalActive ? "Enabled" : "Disabled"));
        Serial.println("-->[CAPT] Captive Portal Active:                                        " + String(captivePortalActive ? "Enabled" : "Disabled"));
#endif
    }

    if (doc.containsKey("captivePortalNoTimeout")) {
        // if captivePortalNoTimeout is being disabled, reset the timeCaptivePortalStarted
        if (captivePortalNoTimeout && !doc["captivePortalNoTimeout"]) {
            timeCaptivePortalStarted = millis();
        }
        captivePortalNoTimeout = doc["captivePortalNoTimeout"];
        // Print if captive portal timeout is enabled or disabled
        Serial.println("-->[CAPT] Captive Portal captivePortalNoTimeout:                        " + String(captivePortalNoTimeout ? "Enabled" : "Disabled"));
    }

    if (doc.containsKey("captivePortalDebug")) {
        captivePortalDebug = doc["captivePortalDebug"];
        // Print if captive portal debug is enabled or disabled
        Serial.println("-->[CAPT] Captive Portal Debug:                                         " + String(captivePortalDebug ? "Enabled" : "Disabled"));
    }
    return true;
}

const char *PARAM_INPUT_1 = "MeasurementInterval";
const char *PARAM_INPUT_2 = "CalibrateCO2";
const char *PARAM_INPUT_3 = "SetVRef";

void initWebServer() {
    SPIFFS.begin();

#ifdef DEBUG_WIFI_EVENTS
    // Print to serial the free space in the SPIFFS
    Serial.print("-->[WEBS] SPIFFS total bytes: ");
    Serial.println(SPIFFS.totalBytes());
    Serial.print("-->[WEBS] SPIFFS used bytes: ");
    Serial.println(SPIFFS.usedBytes());
    Serial.print("-->[WEBS] SPIFFS free bytes: ");
    Serial.println(SPIFFS.totalBytes() - SPIFFS.usedBytes());
#endif

    // server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server.serveStatic("/", SPIFFS, "/index.html");
    // server.serveStatic("/index.html", SPIFFS, "/index.html");
    // server.serveStatic("/preferences.html", SPIFFS, "/preferences.html");
    // server.serveStatic("/status.html", SPIFFS, "/status.html");
    // server.serveStatic("favicon.ico", SPIFFS, "/favicon.png");
    // server.serveStatic("main.js", SPIFFS, "/main.js");
    // server.serveStatic("style.css", SPIFFS, "/style.css");
    // server.serveStatic("preferences.js", SPIFFS, "/preferences.js");
    // server.serveStatic("combined.js", SPIFFS, "/combined.js");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html.gz", "text/html");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html.gz", "text/html");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/preferences.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
#ifdef DEBUG_CAPTIVE_PORTAL
            String debugText = "-->[WEBS] Loading preferences.html with parameters: ";
            if (request->hasParam("captivePortalActive")) debugText += "captivePortalActive: " + String(captivePortalActive ? "Enabled" : "Disabled") + ", ";
            if (request->hasParam("forcedCaptivePortal")) debugText += "forceCaptivePortalActive: " + String(forceCaptivePortalActive ? "Enabled" : "Disabled") + ", ";
            if (request->hasParam("forcedDebugWindow")) debugText += "forcedDebugWindow: " + String(request->hasParam("forcedDebugWindow") ? "Enabled" : "Disabled") + ", ";
            if (request->hasParam("captivePortalNoTimeout")) debugText += "captivePortalNoTimeout: " + String(captivePortalNoTimeout ? "Enabled" : "Disabled") + ", ";
            if (request->hasParam("relaxedSecurity")) debugText += "relaxedSecurity: " + String(relaxedSecurity ? "Enabled" : "Disabled") + ", ";
            if (request->hasParam("captivePortalActive") || request->hasParam("forceCaptivePortalActive") || request->hasParam("captivePortalNoTimeout") ||
                request->hasParam("relaxedSecurity") || request->hasParam("forcedDebugWindow")) Serial.println(debugText);
#endif
            if (request->hasParam("relaxedSecurity")) relaxedSecurity = true;
            if (request->hasParam("forcedCaptivePortal")) forceCaptivePortalActive = true;
            if (request->hasParam("captivePortalNoTimeout")) captivePortalNoTimeout = true;

            if (WiFi.softAPgetStationNum() > 0) {
                captivePortalActive = true;
                Serial.println("-->[WEBS] Captive Portal active because there are connected stations");
            }

            if (forceCaptivePortalActive) captivePortalActive = true;
            if (captivePortalActive) relaxedSecurity = true;

            timeCaptivePortalStarted = millis();

            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/preferences.html.gz", "text/html");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
#ifdef DEBUG_CAPTIVE_PORTAL
        Serial.println("-->[WEBS] Page preferences.html loaded");
#endif
    });

    server.on("/status.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/status.html.gz", "text/html");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/low_power.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/low_power.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/ota.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/ota.html.gz", "text/html");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.js.gz", "application/javascript");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/preferences.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/preferences.js.gz", "application/javascript");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/low_power.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        /** GZIPPED CONTENT ***/
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/low_power.js.gz", "application/javascript");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/status.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/status.js.gz", "application/javascript");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/ota.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/ota.js.gz", "application/javascript");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/style.css.gz", "text/css");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            /** GZIPPED CONTENT ***/
            AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/favicon.png.gz", "image/png");
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/readCO2", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(co2));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/readTemperature", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(temp));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/readHumidity", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(hum));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/readBatteryVoltage", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(batteryVoltage));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/pingServer", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Server is running");
    });

    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String inputString;
            // <CO2-GADGET_IP>/settings?MeasurementInterval=100
            if (request->hasParam("MeasurementInterval")) {
                inputString = request->getParam("MeasurementInterval")->value();
                if (checkStringIsNumerical(inputString)) {
                    // Serial.printf("-->[WEBS] Received /settings command MeasurementInterval with parameter %s\n", inputString);
                    Serial.println("-->[WEBS] Received /settings command MeasurementInterval with parameter " + inputString);
                    measurementInterval = inputString.toInt();
                    request->send(200, "text/plain", "OK. Setting MeasurementInterval to " + inputString + ", please re-calibrate your sensor.");
                } else {
                    request->send(400, "text/plain", "Error. MeasurementInterval must have a number as parameter.");
                }
            }
            // <CO2-GADGET_IP>/settings?CalibrateCO2=400
            if (request->hasParam("CalibrateCO2")) {
                inputString = request->getParam("CalibrateCO2")->value();
                if (checkStringIsNumerical(inputString)) {
                    // Serial.printf("-->[WEBS] Received /settings command CalibrateCO2 with parameter %s\n", inputString);
                    Serial.println("-->[WEBS] Received /settings command CalibrateCO2 with parameter: " + inputString);
                    if ((inputString.toInt() >= 400) && (inputString.toInt() <= 2000)) {
                        calibrationValue = inputString.toInt();
                        pendingCalibration = true;
                        request->send(200, "text/plain", "OK. Recalibrating CO2 sensor to " + inputString);
                    } else {
                        request->send(400, "text/plain", "Error. CO2 calibration value must be between 400 and 2000");
                    }
                }
            };
            // <CO2-GADGET_IP>/settings?ToggleDisplayReverse
            if (request->hasParam("ToggleDisplayReverse")) {
                Serial.println("-->[WEBS] Toggle display reverse");
                displayReverse = !displayReverse;
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
                setDisplayReverse(displayReverse);
                reverseButtons(displayReverse);
                if (inMenu) isMenuDirty = true;
#else
                reverseButtons(displayReverse);
#endif
                request->send(200, "text/plain", "OK. Display reversed");
            };
            // <CO2-GADGET_IP>/settings?setDisplayBrightness
            if (request->hasParam("setDisplayBrightness")) {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
                inputString = request->getParam("setDisplayBrightness")->value();
                Serial.println("-->[WEBS] Set display brightness");
                DisplayBrightness = inputString.toInt();
                setDisplayBrightness(DisplayBrightness);
                if (inMenu) isMenuDirty = true;
#else
                Serial.println("-->[WEBS] Display brightness not supported");
#endif
                request->send(200, "text/plain", "OK. Set Display brightness");
            };
            // <CO2-GADGET_IP>/settings?showTemp=${inputShowTemp}&showHumidity=${inputShowHumidity}&showBattery=${inputShowBattery}
            if (request->hasParam("showTemp") || request->hasParam("showHumidity") || request->hasParam("showBattery")) {
                displayShowTemperature = (request->getParam("showTemp")->value() == "true" || request->getParam("showTemp")->value() == "1");
                displayShowHumidity = (request->getParam("showHumidity")->value() == "true" || request->getParam("showHumidity")->value() == "1");
                displayShowBattery = (request->getParam("showBattery")->value() == "true" || request->getParam("showBattery")->value() == "1");
                Serial.println("-->[WEBS] showTemp(" + request->getParam("showTemp")->value() + ") - showHumidity(" + request->getParam("showHumidity")->value() + ") - showBAttery(" + request->getParam("showBattery")->value() + ") in display");
                if (!inMenu) {
#ifdef DEBUG_CAPTIVE_PORTAL
                    Serial.println("-->[WEBS] Set shouldRedrawDisplay to true");
#endif
                    redrawDisplayOnNextLoop = true;
                }
                request->send(200, "text/plain", "OK. Showing/hidding Temp/Humidity/Battery in display");
            };
            // <CO2-GADGET_IP>/settings?displayShowBatteryVoltage=true
            if (request->hasParam("displayShowBatteryVoltage")) {
                String inputString = request->getParam("displayShowBatteryVoltage")->value();
                if (inputString.equalsIgnoreCase("true") || inputString.equalsIgnoreCase("false")) {
                    bool newValue = inputString.equalsIgnoreCase("true");
                    displayShowBatteryVoltage = newValue;
                    // Serial.printf("-->[WEBS] Received /settings command displayShowBatteryVoltage with parameter: %s\n", newValue ? "true" : "false");
                    Serial.println("-->[WEBS] Received /settings command displayShowBatteryVoltage with parameter: " + inputString);
                    request->send(200, "text/plain", "OK. Setting displayShowBatteryVoltage to " + inputString);
                } else {
                    request->send(400, "text/plain", "Error. Invalid parameter. Use 'true' or 'false'");
                }
            }
            // <CO2-GADGET_IP>/settings?SetVRef=930
            if (request->hasParam("SetVRef")) {
                inputString = request->getParam("SetVRef")->value();
                if (checkStringIsNumerical(inputString)) {
                    // Serial.printf("-->[WEBS] Received /settings command SetVRef with parameter %s\n", inputString);
                    Serial.println("-->[WEBS] Received /settings command SetVRef with parameter: " + inputString);
                    battery.begin(vRef, voltageDividerRatio, &asigmoidal);
                    vRef = inputString.toFloat();
                    request->send(200, "text/plain", "OK. Setting VRef to " + inputString);
                } else {
                    request->send(400, "text/plain", "Error. VRef must have a number as parameter.");
                }
            }
        } else {
            request->send(400, "text/plain", "Error. No parameters received.");
        }
    });

    server.on("/getActualSettingsAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            //
            if (request->hasParam("relaxedSecurity")) relaxedSecurity = true;
            String preferencesJson = getActualSettingsAsJson(relaxedSecurity);
            request->send(200, "application/json", preferencesJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getWifiNetworksAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String wifiNetworksJson = getWifiNetworksAsJson();
            request->send(200, "application/json", wifiNetworksJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getCaptivePortalStatusAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String captivePortalStatusJson = getCaptivePortalStatusAsJson();
            request->send(200, "application/json", captivePortalStatusJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getThresholdsAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        restartTimerToDeepSleep();
        String thresholdsAsJson = thresholdsManager.getAllThresholdsAsJson();
        request->send(200, "application/json", thresholdsAsJson);
    });

    server.on("/getVersion", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String versionJson = getCO2GadgetVersionAsJson();
            request->send(200, "application/json", versionJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getMeasurementInterval", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(measurementInterval));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getFreeHeap", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(ESP.getFreeHeap()));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getMinFreeHeap", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", String(ESP.getMinFreeHeap()));
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/getFeaturesAsJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String featuresJson = getCO2GadgetFeaturesAsJson();
            request->send(200, "application/json", featuresJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            String statusJson = getCO2GadgetStatusAsJson();
            request->send(200, "application/json", statusJson);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            request->send(200, "text/plain", "ESP32 restart initiated");
            delay(100);
            ESP.restart();
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request != nullptr) {
            Serial.println("-->[WEBS] Page not found: " + request->url());
            request->send(400, "text/plain", "Not found");
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    AsyncCallbackJsonWebHandler *saveThresholdsHandler = new AsyncCallbackJsonWebHandler("/saveThresholds", [](AsyncWebServerRequest *request, JsonVariant &json) {
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
        Serial.print("-->[WiFi] Received /saveThresholds command with parameter: ");
        Serial.println(response);
        thresholdsManager.setThresholdsFromJSON(response);
        Serial.print("-->[WiFi] Thresholds saved: ");
        printThresholdsFromNVR();
    });

    server.on("/setPreferencesValue", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("key") && request->hasParam("value")) {
            String key = request->getParam("key")->value();
            String value = request->getParam("value")->value();
            Serial.println("-->[WEBS] Received /setPreferencesValue command with key: " + key + " and value: " + value);
            if (setPreferenceValue(key, value)) {
                request->send(200, "text/plain", "OK. Setting " + key + " to " + value);
            } else {
                request->send(400, "text/plain", "Error. Invalid parameter or unsupported data type");
            }
        } else {
            request->send(400, "text/plain", "Error. Missing parameters. Use 'key' and 'value'");
        }
    });

    AsyncCallbackJsonWebHandler *savePreferencesHandlerHandler = new AsyncCallbackJsonWebHandler("/savePreferences", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request != nullptr) {
            StaticJsonDocument<2048> data;
            if (json.is<JsonArray>()) {
                data = json.as<JsonArray>();
            } else if (json.is<JsonObject>()) {
                data = json.as<JsonObject>();
            }
            String response;
            serializeJson(data, response);
            request->send(200, "application/json", response);
#ifdef DEBUG_CAPTIVE_PORTAL
            Serial.print("-->[WEBS] Received /savePreferences command with content: ");
            Serial.println(response);
#endif
            handleSavePreferencesFromJSON(response);
            timeCaptivePortalStarted = millis();
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    AsyncCallbackJsonWebHandler *setCaptivePortalSettingsHandler = new AsyncCallbackJsonWebHandler("/setCaptivePortalSettings", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request != nullptr) {
            JsonDocument data;
            if (json.is<JsonArray>()) {
                data = json.as<JsonArray>();
            } else if (json.is<JsonObject>()) {
                data = json.as<JsonObject>();
            }
            String response;
            serializeJson(data, response);
            request->send(200, "application/json", response);
            Serial.print("-->[WEBS] Received /setCaptivePortalSettings command with parameter: ");
            Serial.println(response);
            handleCaptivePortalSettings(response);
        } else {
            Serial.println("---> [WiFi] Error: request is null");
        }
    });

    server.addHandler(savePreferencesHandlerHandler);
    server.addHandler(setCaptivePortalSettingsHandler);
    server.addHandler(saveThresholdsHandler);
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

    if (useStaticIP) {
        if (!WiFi.config(staticIP, gateway, subnet, dns1, dns2)) {
            Serial.println("-->[WiFi] Failed to configure static IP and DNS");
            return false;
        }
        Serial.print("-->[WiFi] Configuring static IP: ");
        Serial.println(staticIP);
    } else {
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);  // Use DHCP
    }

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
#ifdef SUPPORT_CAPTIVE_PORTAL
    if (captivePortalActive) return;
#endif

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

#ifdef SUPPORT_CAPTIVE_PORTAL
class CaptiveRequestHandler : public AsyncWebHandler {
   public:
    CaptiveRequestHandler() {
        Serial.println("-->[WiFi] CAPTIVE PORTAL STARTED");
    }

    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
        // request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        request->redirect("/preferences.html");
#ifdef DEBUG_WIFI_EVENTS
        Serial.println("-->[WiFi] Captive portal request");
#endif
        captivePortalActive = true;  // Set captive portal active to true
        timeCaptivePortalStarted = millis();
    }
};

static const void initCaptivePortal() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("-->[WiFi] Already connected to Wi-Fi");
        return;
    } else {
        Serial.println("-->[WiFi] NOT CONNECTED TO WI-FI. STARTING CAPTIVE PORTAL FOR " + String(timeToWaitForCaptivePortal) + " SECONDS");
    }
    WiFi.disconnect(true);
    delay(20);
    WiFi.softAP("CO2-Gadget", "emariete");  // SSID, password
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.end();
    delay(20);
    initWebServer();
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  // only when requested from AP

    server.begin();
    delay(100);
    Serial.print("-->[WiFi] AP IP address: ");
    Serial.println(WiFi.softAPIP());
    captivePortalActive = true;
    timeCaptivePortalStarted = millis();
}
#endif  // SUPPORT_CAPTIVE_PORTAL

void wifiCaptivePortalLoop() {
#ifdef SUPPORT_CAPTIVE_PORTAL
    if (captivePortalActive) {
        dnsServer.processNextRequest();

        if (captivePortalNoTimeout) return;

#ifdef DEBUG_CAPTIVE_PORTAL
        static uint64_t lastPrintTime = 0;
        if (millis() - lastPrintTime >= 5000) {
            lastPrintTime = millis();
            int connectedStations = WiFi.softAPgetStationNum();
            if (connectedStations > 1) Serial.println("-->[CAPT] Captive portal active. Connected stations: " + String(connectedStations));
            // Serial.println("-->[CAPT] Time to wait for Captive Portal: " + String(timeToWaitForCaptivePortal) + " seconds. ");
            // Serial.println("-->[CAPT] Time Captive Portal started: " + String(timeCaptivePortalStarted) + " milliseconds. ");
            Serial.println("-->[CAPT] Time left to disable captive portal: " + String((timeToWaitForCaptivePortal - (millis() - timeCaptivePortalStarted) / 1000)) + " seconds. ");
        }
#endif
        if ((millis() > timeCaptivePortalStarted + timeToWaitForCaptivePortal * 1000)) {
            captivePortalActive = false;
            forceCaptivePortalActive = false;
            Serial.println("-->[CAPT] CAPTIVE PORTAL TIMEOUT. DISABLE CAPTIVE PORTAL");
        }
    }
#endif
}

void wifiClientLoop() {
#ifdef SUPPORT_CAPTIVE_PORTAL
    if (captivePortalActive) {
        wifiCaptivePortalLoop();
        return;
    }
#endif
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
#ifdef SUPPORT_CAPTIVE_PORTAL
    if (captivePortalActive) return;
#endif
    if (isDownloadingBLE) return;
    if ((activeWIFI) && (activeOTA) && (!troubledWIFI) && (WiFi.status() == WL_CONNECTED)) {
        AsyncElegantOTA.loop();
    }
#endif
}

#endif  // CO2_Gadget_WIFI