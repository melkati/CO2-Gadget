#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#ifdef SUPPORT_BLE
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

// clang-format on

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;
DataProvider provider(lib, DataType::T_RH_CO2_ALT, true, false, false, &wifi);
// DataProvider provider(lib, DataType::T_RH_CO2_ALT);
#endif

void initBLE() {
#ifdef SUPPORT_BLE
    if (activeBLE) {
        if (bleInitialized) {
            Serial.print(
                "-->[SBLE] Sensirion Gadget BLE Lib already initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            return;  // If BLE is already initialized do nothing and return
        } else {
            // provider.setSampleIntervalMs(60000); // Set interval for MyAmbiance dataloging at 60 seconds. See https://github.com/melkati/CO2-Gadget/projects/2#card-91517604
            provider.begin();
            Serial.print("-->[SBLE] Sensirion Gadget BLE Lib initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            bleInitialized = true;
        }
        // if (activeWIFI) {
        //   gadgetBle.enableWifiSetupSettings(onWifiSettingsChanged);
        //   gadgetBle.setCurrentWifiSsid(WIFI_SSID_CREDENTIALS);
        // }
    }
#endif
}

void publishBLE() {
#ifdef SUPPORT_BLE
    if ((activeBLE) && (co2 > 0)) {
        provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
        provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
        provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
        provider.commitSample();
    }
#endif
}

void BLELoop() {
#ifdef SUPPORT_BLE
    if (activeBLE) {
        provider.handleDownload();
        delay(3);
        if (provider.wifiChanged()) {
            Serial.print("[BLE] Wifi SSID changed to: ");
            Serial.println(provider.getWifiSSID());
            Serial.print("[BLE] Wifi password changed to: ");
            Serial.println(provider.getWifiPassword());
            wifiSSID = provider.getWifiSSID();
            wifiPass = provider.getWifiPassword();
            connectToWiFi();
        }
    }
#endif
}

#endif  // CO2_Gadget_BLE_h