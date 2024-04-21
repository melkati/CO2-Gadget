#ifndef CO2_Gadget_Improv_h
#define CO2_Gadget_Improv_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP IMPROV FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
/*****************************************************************************************************/
// clang-format on

#include "ImprovWiFiLibrary.h"
ImprovWiFi improvSerial(&Serial);

void onImprovWiFiErrorCb(ImprovTypes::Error err) {
    Serial.println("-->[IMPR] Error: " + String(err));
}

void onImprovWiFiConnectedCb(const char *ssid, const char *password) {
    #ifdef WIFI_PRIVACY
    Serial.println("-->[IMPR] Connected to: " + String(ssid));
    #else
    Serial.println("-->[IMPR] Connected to: " + String(ssid) + " with password: " + String(password));
    #endif
    wifiChanged = true;
    activeWIFI = true;
    wifiSSID = ssid;
    wifiPass = password;
    saveWifiCredentials();
}

void initImprov() {
    char version[120];  // Ajusta el tamaño según tus necesidades
    sprintf(version, "CO2 Gadget Version: %s%s Flavour: %s\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
    Serial.println("-->[IMPR] Init Improv");
    #if defined(CONFIG_IDF_TARGET_ESP32)
    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32, "CO2-Gadget-Beta-Desarrollo", version, "CO2-Gadget", "http://{LOCAL_IPV4}/preferences.html");
    #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32_S3, "CO2-Gadget-Beta-Desarrollo", version, "CO2-Gadget", "http://{LOCAL_IPV4}/preferences.html");
    #endif

    improvSerial.onImprovError(onImprovWiFiErrorCb);
    improvSerial.onImprovConnected(onImprovWiFiConnectedCb);
    //   improvSerial.setCustomConnectWiFi(initWifi);  // Optional
}

void improvLoop() {
    if (!inMenu) {
        if (Serial.available() && Serial.peek() != 0x2A) { // 0x2A = '*'
            improvSerial.handleSerial();
        }
    }
}

#endif  // CO2_Gadget_Improv_h