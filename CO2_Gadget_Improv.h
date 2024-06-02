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
    char version[120];  // Adjust the size of the buffer to your needs
    sprintf(version, "CO2 Gadget Version: %s%s Flavour: %s\n", CO2_GADGET_VERSION, CO2_GADGET_REV, FLAVOUR);
    Serial.println("-->[IMPR] Init Improv");
#if defined(CONFIG_IDF_TARGET_ESP32)
    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32, "CO2-Gadget", version, "CO2-Gadget", "http://{LOCAL_IPV4}/");
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32_S3, "CO2-Gadget", version, "CO2-Gadget", "http://{LOCAL_IPV4}/");
#endif

    improvSerial.onImprovError(onImprovWiFiErrorCb);
    improvSerial.onImprovConnected(onImprovWiFiConnectedCb);
    // improvSerial.setCustomConnectWiFi(initWifi);  // Optional
}

void improvLoop() {
    if (isDownloadingBLE) return;
    if (inMenu) {  // If in menu ignore Improv functionality
#ifdef DEBUG_IMPROV_WIFI
        static unsigned long lastPrintTime = 0;
        if (waitingForImprov && (millis() - lastPrintTime >= 1000)) {
            Serial.println("-->[IMPR] In menu, Improv functionality disabled");
            lastPrintTime = millis();
        }
#endif
        return;
    }

    // If not '*' = 0x2A or '/' = 0x2F
    if (Serial.available() && (Serial.peek() != 0x2A) && (Serial.peek() != 0x2F)) {
        improvSerial.handleSerial();
#ifdef DEBUG_IMPROV_WIFI
        static unsigned long lastPrintTime = 0;
        if (millis() - lastPrintTime >= 1000) {
            Serial.println("-->[IMPR] Handling Improv serial");
            lastPrintTime = millis();
        }
#endif
    }

    if (timeToWaitForImprov == 0) {  // If time to wait for Improv is 0, Improv functionality will be enabled forever
#ifdef DEBUG_IMPROV_WIFI
        static unsigned long lastPrintTime = 0;
        if ((waitingForImprov) && (millis() - lastPrintTime >= 1000)) {
            Serial.println("-->[IMPR] Improv functionality enabled forever");
            lastPrintTime = millis();
        }
#endif
        return;
    }

    // If time to wait for Improv is over, disable Improv functionality
    if (waitingForImprov) {
        waitingForImprov = (millis() < timeInitializationCompleted + timeToWaitForImprov * 1000) > 0;
        if (!waitingForImprov) {
            mustInitMenu = true;
            publishMQTTLogData("-->[IMPR] mustInitMenu: " + String(mustInitMenu));
            Serial.println("-->[IMPR] Improv functionality disabled");
        }
#ifdef DEBUG_IMPROV_WIFI
        static unsigned long lastPrintTime = 0;
        if (millis() - lastPrintTime >= 1000) {
            Serial.println("-->[IMPR] Waiting for Improv: " + String(waitingForImprov));
            lastPrintTime = millis();
        }
#endif
    }

#ifdef DEBUG_IMPROV_WIFI
    if (waitingForImprov) {
        static unsigned long lastPrintTime = 0;
        if (millis() - lastPrintTime >= 1000) {
            Serial.println("-->[IMPR] Time left waiting for Improv: " + String((timeInitializationCompleted + timeToWaitForImprov * 1000 - millis()) / 1000) + "sec");
            lastPrintTime = millis();
        }
    }
#endif
}

#endif  // CO2_Gadget_Improv_h