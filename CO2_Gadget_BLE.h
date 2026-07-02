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
#endif

#ifndef BLE_WAKE_ADVERTISEMENT_MS
#define BLE_WAKE_ADVERTISEMENT_MS 2500
#endif

void disableBLE();

static inline bool isValidBLEMeasurement() {
    return (co2 >= 400) && (co2 <= 5000) && (temp >= -40) && (temp <= 85) && (hum >= 0) && (hum <= 100);
}

static inline bool writeSensirionCurrentSample() {
#ifdef SUPPORT_BLE
    if (!isValidBLEMeasurement()) {
        return false;
    }

    provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
    provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
    provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
    return true;
#else
    return false;
#endif
}

static inline bool isBLETimerWakeFromDeepSleep() {
#ifdef SUPPORT_LOW_POWER
    return (esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode;
#else
    return false;
#endif
}

#ifdef SUPPORT_LOW_POWER
typedef struct {
    bool valid = false;
    uint16_t previousCO2Value = 0;
    float previousTemperatureValue = 0.0f;
    float previousHumidityValue = 0.0f;
    uint64_t lastPublishTimeMs = 0;
} bleThresholdState_t;

RTC_DATA_ATTR bleThresholdState_t bleThresholdStateRTC;
#endif

bool evaluateBLEPublishThresholds(uint16_t currentCO2, float currentTemp, float currentHum, bool evaluateCO2, bool evaluateTemp, bool evaluateHum) {
#ifdef SUPPORT_LOW_POWER
    if (isBLETimerWakeFromDeepSleep()) {
        if (bleThresholdStateRTC.valid) {
            thresholdsManager.setRuntimeState(BLE_SEND, bleThresholdStateRTC.previousCO2Value, bleThresholdStateRTC.previousTemperatureValue, bleThresholdStateRTC.previousHumidityValue, bleThresholdStateRTC.lastPublishTimeMs);
        }

        uint64_t nowMs = (deepSleepData.uptimeMillis + millis());
        bool shouldPublish = thresholdsManager.evaluateThresholdsAt(BLE_SEND, currentCO2, currentTemp, currentHum, nowMs, evaluateCO2, evaluateTemp, evaluateHum);
        ThresholdConfig config = thresholdsManager.getThresholds(BLE_SEND);
        if (shouldPublish || !bleThresholdStateRTC.valid) {
            bleThresholdStateRTC.valid = true;
            bleThresholdStateRTC.previousCO2Value = config.previousCO2Value;
            bleThresholdStateRTC.previousTemperatureValue = config.previousTemperatureValue;
            bleThresholdStateRTC.previousHumidityValue = config.previousHumidityValue;
            bleThresholdStateRTC.lastPublishTimeMs = config.lastPublishTimeMs;
        }
        return shouldPublish;
    }
#endif
    return thresholdsManager.evaluateThresholdsAt(BLE_SEND, currentCO2, currentTemp, currentHum, millis(), evaluateCO2, evaluateTemp, evaluateHum);
}

#include "CO2_Gadget_BTHome.h"

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
    if (!enableBLE) {
        disableBLE();
        return;
    }

    if (!activeBLE && !activeBTHome) {
        disableBLE();
        return;
    }

    if (bleInitialized && (!activeBLE || sensirionBLEInitialized)) {
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
        bool initialSampleReady = writeSensirionCurrentSample();
        provider.begin();
        if (initialSampleReady) {
            provider.commitSample();
        }
        sensirionBLEInitialized = true;
        bleInitialized = true;
#ifdef SUPPORT_BTHOME_BLE
        if (!isBLETimerWakeFromDeepSleep()) {
            updateBTHomeAdvertisementData(false);
        }
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
            seedBTHomeCounter();
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
        seedBTHomeCounter();
        if (!isBLETimerWakeFromDeepSleep()) {
            updateBTHomeAdvertisementData(false);
        }
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

#ifdef SUPPORT_BTHOME_BLE
    invalidateBTHomeServiceDataCache();
#endif
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
bool publishBLE(bool ignoreMeasurementInterval = false, bool bypassThresholds = false) {
    static int64_t lastMeasurementTimeMs = 0;
    static int measurementIntervalMs = 1000;
    static int64_t lastBatteryLevelUpdateMs = 0;
    static int batteryLevelUpdateIntervalMs = 60000;
#ifdef SUPPORT_BLE
    if (!enableBLE) {
        return false;
    }

    if (sensirionBLEInitialized && isDownloadingBLE) {
        return false;
    }
    bool published = false;
    if (ignoreMeasurementInterval || (millis() - lastMeasurementTimeMs >= measurementIntervalMs)) {
        bool outputEnabled = activeBLE || activeBTHome;
        bool validForSensirion = activeBLE && sensirionBLEInitialized && isValidBLEMeasurement();
#ifdef SUPPORT_BTHOME_BLE
        uint32_t bthomeEligibleMask = activeBTHome ? bthomeFitMask(bthomeEncryption, !bthomeEncryption) : 0;
        bool validForBTHome = false;
        for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
            uint32_t bit = BTHOME_MEASUREMENTS[i].bit;
            if ((bit != BTHOME_SEL_BATTERY) && (bit != BTHOME_SEL_VOLTAGE) && (bthomeEligibleMask & bit)) {
                validForBTHome = true;
                break;
            }
        }
#else
        bool validForBTHome = false;
#endif
        bool anyValid = validForSensirion || validForBTHome;
        bool evaluateCO2 = validForSensirion;
        bool evaluateTemp = validForSensirion;
        bool evaluateHum = validForSensirion;
#ifdef SUPPORT_BTHOME_BLE
        if (activeBTHome) {
            evaluateCO2 = evaluateCO2 || ((bthomeEligibleMask & BTHOME_SEL_CO2) != 0);
            evaluateTemp = evaluateTemp || ((bthomeEligibleMask & BTHOME_SEL_TEMP) != 0);
            evaluateHum = evaluateHum || ((bthomeEligibleMask & BTHOME_SEL_HUM) != 0);
        }
#endif
        bool thresholdsPassed = outputEnabled && anyValid && (bypassThresholds || evaluateBLEPublishThresholds(co2, temp, hum, evaluateCO2, evaluateTemp, evaluateHum));

        if (outputEnabled && thresholdsPassed) {
            if (activeBLE && sensirionBLEInitialized && validForSensirion) {
                if (writeSensirionCurrentSample()) {
                    provider.commitSample();
                    published = true;
                }
            }
#ifdef SUPPORT_BTHOME_BLE
            if (activeBTHome && validForBTHome) {
                bool bthomeAdvertised = updateBTHomeAdvertisementData(true);
                published = bthomeAdvertised || published;
                if (ignoreMeasurementInterval) {
                    Serial.println("-->[BLE ] BTHome wake payload " + String(bthomeAdvertised ? "installed" : "skipped") + ". thresholdsPassed: " + String(thresholdsPassed) + ", CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %, Scan response: " + String((activeBLE && sensirionBLEInitialized) ? "yes" : "no"));
                }
            }
#endif
            lastMeasurementTimeMs = millis();
        } else if (ignoreMeasurementInterval) {
            Serial.println("-->[BLE ] BLE wake publish skipped. activeBLE: " + String(activeBLE) + ", activeBTHome: " + String(activeBTHome) + ", validForSensirion: " + String(validForSensirion) + ", validForBTHome: " + String(validForBTHome) + ", thresholdsPassed: " + String(thresholdsPassed) + ", CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
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
        if (activeBLE && sensirionBLEInitialized) {
            provider.setBatteryLevel(batteryLevel);
        }
        // BTHome carries battery data with the next threshold/keepalive-driven payload.
#ifdef DEBUG_BLE
        Serial.println("-->[BLE ] Sent Battery Level: " + String(batteryLevel) + "%");
        publishMQTTLogData("-->[BLE ] Sent Battery Level: " + String(batteryLevel) + "%");
        delay(20);
#endif
    }
    return published;
#else
    return false;
#endif
}

void refreshBLEOutputs(const char* reason, bool forcePublish) {
#ifdef SUPPORT_BLE
    String logReason = reason ? String(reason) : String("BLE settings changed");
    Serial.println("-->[BLE ] " + logReason + "; refreshing BLE advertising.");

    if (!enableBLE || (!activeBLE && !activeBTHome)) {
        disableBLE();
        Serial.println("-->[BLE ] BLE output is disabled after settings refresh.");
        return;
    }

    if (!bleInitialized || (activeBLE && !sensirionBLEInitialized)) {
        initBLE();
    }

#ifdef SUPPORT_BTHOME_BLE
    if (!activeBTHome) {
        bool cleared = clearBTHomeAdvertisementData();
        if (activeBLE && sensirionBLEInitialized) {
            restoreSensirionAdvertisementData();
        }
        Serial.println("-->[BLE ] BTHome advertisement " + String(cleared ? "cleared." : "clear failed."));
        return;
    }

    if (!activeBLE) {
        bool advertised = updateBTHomeAdvertisementData(forcePublish, true, true);
        Serial.println("-->[BLE ] BTHome primary advertisement " + String(advertised ? "updated." : "skipped."));
        return;
    }

    if (sensirionBLEInitialized) {
        restoreSensirionAdvertisementData();
    }
    if (forcePublish) {
        bool published = publishBLE(true, true);
        Serial.println("-->[BLE ] BLE refresh publish " + String(published ? "completed." : "skipped."));
    } else {
        updateBTHomeAdvertisementData(false);
    }
#else
    if (forcePublish) {
        bool published = publishBLE(true, true);
        Serial.println("-->[BLE ] BLE refresh publish " + String(published ? "completed." : "skipped."));
    }
#endif
#endif
}

void refreshBTHomeBLESettings(const char* reason, bool forcePublish) {
#if defined(SUPPORT_BLE) && defined(SUPPORT_BTHOME_BLE)
    refreshBLEOutputs(reason, forcePublish);
#endif
}

void handleBLEwifiChanged() {
#ifdef SUPPORT_BLE
    wifiSSID = provider.getWifiSSID();
    wifiPass = provider.getWifiPassword();
    wifiSSID.trim();
    // Do not trim wifiPass: leading/trailing spaces are valid password characters
    // and must be preserved to match the save/load paths.
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
    if (!enableBLE || (!activeBLE && !activeBTHome)) {
        return;
    }

#ifdef SUPPORT_BTHOME_BLE
    ensureBTHomeAdvertisingActive();
#endif

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
