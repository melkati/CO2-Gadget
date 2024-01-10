#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#include "Sensirion_Gadget_BLE.h"
// GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);
NimBLELibraryWrapper lib;
DataProvider provider(lib, DataType::T_RH_CO2_ALT);

void initBLE() {
    if (activeBLE) {
        if (bleInitialized) {
            Serial.print(
                "-->[SBLE] Sensirion Gadget BLE Lib already initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            return;  // If BLE is already initialized do nothing and return
        } else {
            // provider.setSampleIntervalMs(60000); // Set interval for MyAmbiance dataloging at 60 seconds
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
}

void publishBLE() {
    if (activeBLE) {
        provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
        provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
        provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
        provider.commitSample();
    }
}

void BLELoop() {
    if (activeBLE) {
        provider.handleDownload();
        delay(3);
    }
}

#endif  // CO2_Gadget_BLE_h