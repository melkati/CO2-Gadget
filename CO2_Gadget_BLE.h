#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#if defined(SUPPORT_BTHOME_BLE) && !defined(SUPPORT_BLE)
#error "SUPPORT_BTHOME_BLE requires SUPPORT_BLE"
#endif

#ifdef SUPPORT_BLE
#include <NimBLEDevice.h>
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

// clang-format on

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;
DataProvider provider(lib, DataType::T_RH_CO2, true, true, true, &wifi);
static bool sensirionBLEInitialized = false;
#ifdef SUPPORT_BTHOME_BLE
static uint8_t bthomePacketId = 0;
#endif
#endif

bool isValidBLEMeasurement() {
    return (co2 >= 400) && (co2 <= 5000) && (temp >= -40) && (temp <= 85) && (hum >= 0) && (hum <= 100);
}

#ifdef SUPPORT_BTHOME_BLE
int16_t encodeBTHomeTemperature(float value) {
    return static_cast<int16_t>(round(value * 100.0f));
}

uint16_t encodeBTHomeHumidity(float value) {
    return static_cast<uint16_t>(round(value * 100.0f));
}

uint8_t getBTHomeBatteryLevel() {
    if (batteryLevel == 0) {
        return 100;
    }
    if (batteryLevel > 100) {
        return 100;
    }
    return batteryLevel;
}

void appendBTHomeUInt8(std::string &payload, uint8_t value) {
    payload.push_back(static_cast<char>(value));
}

void appendBTHomeUInt16(std::string &payload, uint16_t value) {
    payload.push_back(static_cast<char>(value & 0xFF));
    payload.push_back(static_cast<char>((value >> 8) & 0xFF));
}

void appendBTHomeInt16(std::string &payload, int16_t value) {
    appendBTHomeUInt16(payload, static_cast<uint16_t>(value));
}

std::string buildBTHomeServiceData(bool incrementPacketId) {
    std::string payload;
    if (!activeBTHome || !isValidBLEMeasurement()) {
        return payload;
    }

    if (incrementPacketId) {
        ++bthomePacketId;
    }

    payload.reserve(13);
    appendBTHomeUInt8(payload, 0x40);  // BTHome v2, not encrypted, regular updates
    appendBTHomeUInt8(payload, 0x00);
    appendBTHomeUInt8(payload, bthomePacketId);
    appendBTHomeUInt8(payload, 0x01);
    appendBTHomeUInt8(payload, getBTHomeBatteryLevel());
    appendBTHomeUInt8(payload, 0x02);
    appendBTHomeInt16(payload, encodeBTHomeTemperature(temp));
    appendBTHomeUInt8(payload, 0x03);
    appendBTHomeUInt16(payload, encodeBTHomeHumidity(hum));
    appendBTHomeUInt8(payload, 0x12);
    appendBTHomeUInt16(payload, static_cast<uint16_t>(co2));

    return payload;
}

void updateBTHomeAdvertisementData(bool incrementPacketId) {
    if (!activeBTHome || !isValidBLEMeasurement()) {
        return;
    }

    std::string payload = buildBTHomeServiceData(incrementPacketId);
    if (payload.empty()) {
        return;
    }

    NimBLEAdvertisementData advertisementData;
    advertisementData.setServiceData(NimBLEUUID(static_cast<uint16_t>(0xFCD2)), payload);

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    if (sensirionBLEInitialized) {
        advertising->enableScanResponse(true);
        advertising->setScanResponseData(advertisementData);
        advertising->refreshAdvertisingData();
    } else {
        advertisementData.setFlags(0x06);
        advertising->stop();
        advertising->setAdvertisementData(advertisementData);
        advertising->start();
    }

#ifdef DEBUG_BLE
    Serial.println("-->[BLE ] BTHome CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %, Battery: " + String(getBTHomeBatteryLevel()) + "%");
#endif
}
#endif

void setBLEHistoryInterval(uint64_t interval) {
#ifdef SUPPORT_BLE
    if (provider.getHistoryInterval() != interval * 1000) {
#ifdef DEBUG_BLE
        Serial.flush();
        Serial.println("-->[BLE ] Actual history interval: " + String(provider.getHistoryInterval() / 1000) + " seconds");
        Serial.println("-->[BLE ] Setting history interval to: " + String(interval) + " seconds");
        delay(20);
#endif
        provider.setHistoryInterval(interval * 1000);
    }
#endif
}

void initBLE() {
#ifdef SUPPORT_BLE
    if (!activeBLE && !activeBTHome) {
        return;
    }

    if (bleInitialized) {
        if (sensirionBLEInitialized) {
            Serial.print("-->[BLE ] Sensirion Gadget BLE Lib already initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
        } else {
            Serial.println("-->[BLE ] BTHome BLE already initialized");
        }
        return;
    }

    if (activeBLE) {
        setBLEHistoryInterval(sampleInterval);
        provider.begin();
        sensirionBLEInitialized = true;
        bleInitialized = true;
#ifdef SUPPORT_BTHOME_BLE
        updateBTHomeAdvertisementData(false);
#endif
        Serial.print("-->[BLE ] Sensirion Gadget BLE Lib initialized with deviceId = ");
        Serial.println(provider.getDeviceIdString());
        Serial.print("-->[BLE ] History interval set to: ");
        Serial.print(provider.getHistoryInterval() / 1000);
        Serial.println(" seconds");
        // Set initial battery level
        provider.setBatteryLevel(batteryLevel);
#ifdef SUPPORT_BTHOME_BLE
        if (activeBTHome) {
            Serial.println("-->[BLE ] BTHome BLE scan response enabled");
        }
#endif
        return;
    }

#ifdef SUPPORT_BTHOME_BLE
    if (activeBTHome) {
        NimBLEDevice::init(hostName.c_str());
        NimBLEDevice::setPower(3);
        bleInitialized = true;
        updateBTHomeAdvertisementData(false);
        Serial.println("-->[BLE ] BTHome BLE initialized");
    }
#endif
#endif
}

void disableBLE() {
#ifdef SUPPORT_BLE
    if (!bleInitialized) {
        return;
    }

    if (sensirionBLEInitialized) {
        lib.stopAdvertising();
    } else {
        NimBLEDevice::getAdvertising()->stop();
    }
    delay(10);
    NimBLEDevice::deinit(true);
    bleInitialized = false;
    sensirionBLEInitialized = false;
    delay(20);
    Serial.println("-->[BLE ] BLE disabled!");
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
    if (sensirionBLEInitialized && isDownloadingBLE) {
        return;
    }
    if (millis() - lastMeasurementTimeMs >= measurementIntervalMs) {
        if ((activeBLE || activeBTHome) && isValidBLEMeasurement() && thresholdsManager.evaluateThresholds(BLE_SEND, co2, temp, hum)) {
            if (sensirionBLEInitialized) {
                provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
                provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
                provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
                provider.commitSample();
            }
#ifdef SUPPORT_BTHOME_BLE
            if (activeBTHome) {
                updateBTHomeAdvertisementData(true);
            }
#endif
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
        if (sensirionBLEInitialized) {
            provider.setBatteryLevel(batteryLevel);
        }
#ifdef SUPPORT_BTHOME_BLE
        if (activeBTHome) {
            updateBTHomeAdvertisementData(true);
        }
#endif
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
    if (!activeBLE || !sensirionBLEInitialized) {
        return;
    }
    provider.handleDownload();
    isDownloadingBLE = provider.isDownloading();
    if (isDownloadingBLE) return;
    // delay(3);
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
