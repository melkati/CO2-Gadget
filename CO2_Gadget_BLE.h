#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#ifdef SUPPORT_BLE
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

// clang-format on

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;
DataProvider provider(lib, DataType::T_RH_CO2_ALT, true, true, true, &wifi);
#endif

void setBLEHistoryInterval(uint64_t interval) {
#ifdef SUPPORT_BLE
    provider.setHistoryInterval(interval * 1000);
#endif
}

void initBLE() {
#ifdef SUPPORT_BLE
    if (activeBLE) {
        if (bleInitialized) {
            Serial.print(
                "-->[SBLE] Sensirion Gadget BLE Lib already initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            return;
        } else {
            // provider.setSampleIntervalMs(60000); // Set interval for MyAmbiance dataloging at 60 seconds. See https://github.com/melkati/CO2-Gadget/projects/2#card-91517604
            setBLEHistoryInterval(60);
            provider.begin();
            Serial.print("-->[SBLE] Sensirion Gadget BLE Lib initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            Serial.print("-->[SBLE] History interval set to: ");
            Serial.print(provider.getHistoryInterval() / 1000);
            Serial.println(" seconds");
            // Set initial battery level
            provider.setBatteryLevel(batteryLevel);
            bleInitialized = true;
        }
    }
#endif
}

/**
 * @brief Publishes sensor data over BLE (Bluetooth Low Energy).
 *
 * This function is responsible for publishing sensor data over BLE if the BLE support is enabled and the sensor readings are within the valid range.
 * The sensor data includes CO2 level, temperature, and humidity.
 *
 * @note This function should be called periodically to publish the sensor data.
 */
void publishBLE() {
    static int64_t lastMeasurementTimeMs = 0;
    static int measurementIntervalMs = 1000;
    static int64_t lastBatteryLevelUpdateMs = 0;
    static int batteryLevelUpdateIntervalMs = 60000;
#ifdef SUPPORT_BLE
    if (millis() - lastMeasurementTimeMs >= measurementIntervalMs) {
        if ((activeBLE) && (co2 >= 400) && (co2 <= 5000) && (temp >= -40) && (temp <= 85) && (hum >= 0) && (hum <= 100)) {
            provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
            provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
            provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
            provider.commitSample();
            lastMeasurementTimeMs = millis();
        }
#ifdef DEBUG_BLE
        Serial.println("-->[BLE ] Sent CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
        publishMQTTLogData("-->[BLE ] Sent CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
        delay(20);
#endif
    }
    if (millis() - lastBatteryLevelUpdateMs >= batteryLevelUpdateIntervalMs) {
        lastBatteryLevelUpdateMs = millis();
        if (batteryLevel == 0) {
            batteryLevel = 100;
        }
        provider.setBatteryLevel(batteryLevel);
#ifdef DEBUG_BLE
        Serial.println("-->[BLE ] Sent Battery Level: " + String(batteryLevel) + "%");
        publishMQTTLogData("-->[BLE ] Sent Battery Level: " + String(batteryLevel) + "%");
        delay(20);
#endif
    }
#endif
}

void handleBLEwifiChanged() {
#ifdef SUPPORT_BLE
    wifiSSID = provider.getWifiSSID();
    wifiPass = provider.getWifiPassword();
    wifiSSID.trim();
    wifiPass.trim();
    wifiChanged = true;
    activeWIFI = true;
    Serial.println("-->[BLE ] Wifi SSID changed to: #" + wifiSSID + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[BLE ] Wifi password changed to: #" + wifiPass + "#");
#endif
    delay(5);
#endif
}

void handleFrcRequest() {
#ifdef SUPPORT_BLE
    if (!provider.isFRCRequested()) {
        return;
    }

    calibrationValue = provider.getReferenceCO2Level();
    pendingCalibration = true;

    Serial.print("[BLE ] Received FRC request (calibration) with reference CO2 level: ");
    Serial.println(calibrationValue);
    delay(5);
    provider.completeFRCRequest();
#endif
}

void BLELoop() {
#ifdef SUPPORT_BLE
    int connectTries = 0;
    if (!activeBLE) {
        return;
    }
    provider.handleDownload();
    delay(3);
    if (provider.wifiChanged()) handleBLEwifiChanged();
    if (provider.historyIntervalChanged()) {
        Serial.print("-->[BLE ] History interval changed to: ");
        Serial.print(provider.getHistoryInterval() / 1000);
        Serial.println(" seconds");
    }
    handleFrcRequest();
#endif
}

#endif  // CO2_Gadget_BLE_h