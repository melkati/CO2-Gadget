#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#if defined(SUPPORT_BTHOME_BLE) && !defined(SUPPORT_BLE)
#error "SUPPORT_BTHOME_BLE requires SUPPORT_BLE"
#endif

#ifdef SUPPORT_BLE
#ifdef SUPPORT_BTHOME_BLE
#include <esp_mac.h>
#include <esp_random.h>
#include <mbedtls/ccm.h>
#endif
#include <NimBLEDevice.h>
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

// clang-format on

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;
DataProvider provider(lib, DataType::T_RH_CO2, true, true, true, &wifi);
static bool sensirionBLEInitialized = false;
#ifdef SUPPORT_BTHOME_BLE
static constexpr uint8_t BTHOME_DEVICE_INFO = 0x44;            // BTHome v2, irregular/trigger based, not encrypted.
static constexpr uint8_t BTHOME_DEVICE_INFO_ENCRYPTED = 0x45;  // BTHome v2, irregular/trigger based, encrypted.
static constexpr uint16_t BTHOME_UUID = 0xFCD2;
static constexpr uint8_t BTHOME_ENCRYPTION_KEY_SIZE = 16;
static constexpr uint8_t BTHOME_ENCRYPTION_MIC_SIZE = 4;
static constexpr uint8_t BTHOME_ENCRYPTION_NONCE_SIZE = 13;
static constexpr uint8_t BTHOME_COUNTER_SAVE_INTERVAL = 64;
static constexpr uint32_t BTHOME_COUNTER_NVS_RESERVE = 4096;
static constexpr uint16_t BTHOME_ADV_INTERVAL = 320;  // 200 ms, in 0.625 ms units.
static uint8_t bthomePacketId = 0;
static uint8_t bthomeCounterSaveSkips = 0;
static std::string bthomeCachedServiceData;
static bool bthomeCachedServiceDataValid = false;
#endif
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

#ifdef SUPPORT_BTHOME_BLE
int16_t encodeBTHomeTemperature(float value) {
    return static_cast<int16_t>(round(value * 100.0f));
}

uint16_t encodeBTHomeHumidity(float value) {
    return static_cast<uint16_t>(round(value * 100.0f));
}

uint16_t encodeBTHomeBatteryVoltage(float value) {
    if (value <= 0.0f) {
        return 0;
    }
    uint32_t millivolts = static_cast<uint32_t>(round(value * 100.0f)) * 10;
    if (millivolts > UINT16_MAX) {
        return UINT16_MAX;
    }
    return static_cast<uint16_t>(millivolts);
}

// [BTHOME-SENSEL] BTHome Pressure object 0x04: uint24, factor 0.01, unit hPa.
uint32_t encodeBTHomePressure(float hpa) {
    if (hpa <= 0.0f) {
        return 0;
    }
    double raw = static_cast<double>(hpa) * 100.0;
    if (raw > 16777215.0) {
        return 16777215;  // uint24 max
    }
    return static_cast<uint32_t>(llround(raw));
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

// [BTHOME-SENSEL] little-endian uint24 (used by Pressure 0x04).
void appendBTHomeUInt24(std::string &payload, uint32_t value) {
    payload.push_back(static_cast<char>(value & 0xFF));
    payload.push_back(static_cast<char>((value >> 8) & 0xFF));
    payload.push_back(static_cast<char>((value >> 16) & 0xFF));
}

bool decodeHexNibble(char c, uint8_t &value) {
    if ((c >= '0') && (c <= '9')) {
        value = c - '0';
        return true;
    }
    if ((c >= 'a') && (c <= 'f')) {
        value = c - 'a' + 10;
        return true;
    }
    if ((c >= 'A') && (c <= 'F')) {
        value = c - 'A' + 10;
        return true;
    }
    return false;
}

bool decodeBTHomeBindKey(uint8_t key[BTHOME_ENCRYPTION_KEY_SIZE]) {
    String normalizedKey = bthomeBindKey;
    normalizedKey.trim();
    normalizedKey.replace(" ", "");
    normalizedKey.replace(":", "");
    normalizedKey.replace("-", "");

    if (normalizedKey.length() != BTHOME_ENCRYPTION_KEY_SIZE * 2) {
        return false;
    }

    for (uint8_t i = 0; i < BTHOME_ENCRYPTION_KEY_SIZE; ++i) {
        uint8_t high;
        uint8_t low;
        if (!decodeHexNibble(normalizedKey[i * 2], high) || !decodeHexNibble(normalizedKey[i * 2 + 1], low)) {
            return false;
        }
        key[i] = (high << 4) | low;
    }

    return true;
}

void appendBTHomeCounter(std::string &payload, uint32_t counter) {
    appendBTHomeUInt16(payload, static_cast<uint16_t>(counter & 0xFFFF));
    appendBTHomeUInt16(payload, static_cast<uint16_t>((counter >> 16) & 0xFFFF));
}

uint32_t getBTHomeCounterNVSValue() {
    if (!bthomeEncryption) {
        return bthomeCounter;
    }
    if (UINT32_MAX - bthomeCounter < BTHOME_COUNTER_NVS_RESERVE) {
        return UINT32_MAX;
    }
    return bthomeCounter + BTHOME_COUNTER_NVS_RESERVE;
}

void saveBTHomeCounter(bool force) {
    if (!bthomeEncryption) {
        return;
    }
    if (!force && (++bthomeCounterSaveSkips < BTHOME_COUNTER_SAVE_INTERVAL)) {
        return;
    }

    bthomeCounterSaveSkips = 0;
    preferences.begin("CO2-Gadget", false);
    preferences.putUInt("bthomeCounter", getBTHomeCounterNVSValue());
    preferences.end();
}

void seedBTHomeCounter() {
    if (!bthomeEncryption) {
        return;
    }
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && !bthomeCounterNeedsSeed) {
        return;
    }

    uint32_t counterJump = (esp_random() & 0x0FFF) + 1;
    bthomeCounter += counterJump;
    saveBTHomeCounter(true);
    bthomeCounterNeedsSeed = false;
}

bool getBTHomeMacAddress(uint8_t mac[6]) {
    return esp_read_mac(mac, ESP_MAC_BT) == ESP_OK;
}

bool encryptBTHomePayload(const std::string &plainPayload, std::string &encryptedPayload) {
    uint8_t key[BTHOME_ENCRYPTION_KEY_SIZE];
    if (!decodeBTHomeBindKey(key)) {
        Serial.println("-->[BLE ] BTHome encryption enabled but bind key is invalid. Expected 32 hex characters.");
        return false;
    }

    uint8_t mac[6];
    if (!getBTHomeMacAddress(mac)) {
        Serial.println("-->[BLE ] Could not read Bluetooth MAC address for BTHome encryption.");
        return false;
    }

    ++bthomeCounter;

    uint8_t nonce[BTHOME_ENCRYPTION_NONCE_SIZE];
    memcpy(nonce, mac, sizeof(mac));
    nonce[6] = 0xD2;
    nonce[7] = 0xFC;
    nonce[8] = BTHOME_DEVICE_INFO_ENCRYPTED;
    nonce[9] = static_cast<uint8_t>(bthomeCounter & 0xFF);
    nonce[10] = static_cast<uint8_t>((bthomeCounter >> 8) & 0xFF);
    nonce[11] = static_cast<uint8_t>((bthomeCounter >> 16) & 0xFF);
    nonce[12] = static_cast<uint8_t>((bthomeCounter >> 24) & 0xFF);

    std::string cipherText(plainPayload.length(), '\0');
    uint8_t mic[BTHOME_ENCRYPTION_MIC_SIZE];

    mbedtls_ccm_context ccm;
    mbedtls_ccm_init(&ccm);
    int result = mbedtls_ccm_setkey(&ccm, MBEDTLS_CIPHER_ID_AES, key, BTHOME_ENCRYPTION_KEY_SIZE * 8);
    if (result == 0) {
        result = mbedtls_ccm_encrypt_and_tag(&ccm,
                                             plainPayload.length(),
                                             nonce,
                                             sizeof(nonce),
                                             nullptr,
                                             0,
                                             reinterpret_cast<const unsigned char *>(plainPayload.data()),
                                             reinterpret_cast<unsigned char *>(&cipherText[0]),
                                             mic,
                                             sizeof(mic));
    }
    mbedtls_ccm_free(&ccm);

    if (result != 0) {
        Serial.println("-->[BLE ] BTHome encryption failed with mbedTLS error: " + String(result));
        return false;
    }

    encryptedPayload.reserve(1 + cipherText.length() + 4 + BTHOME_ENCRYPTION_MIC_SIZE);
    appendBTHomeUInt8(encryptedPayload, BTHOME_DEVICE_INFO_ENCRYPTED);
    encryptedPayload += cipherText;
    appendBTHomeCounter(encryptedPayload, bthomeCounter);
    encryptedPayload.append(reinterpret_cast<const char *>(mic), sizeof(mic));
    saveBTHomeCounter(false);
    return true;
}

// [BTHOME-SENSEL] ---- Availability-aware, user-selectable BTHome measurements ----
// The advertised set is the intersection of (user selection `bthomeSensors`) AND
// (sensor availability) AND (fits the BLE service-data budget). `priority` decides
// what is kept when the selection does not all fit (lower = kept first).
struct BTHomeMeasurementDef {
    uint32_t bit;
    const char *key;
    const char *label;
    uint8_t objectId;
    uint8_t totalBytes;  // object-id byte + data bytes
    uint8_t priority;    // 1 = highest
    const char *group;   // "core" | "optional" | "nonnative" (no standard BTHome object)
};

// Stored in ascending object-id order (also the order emitted into the payload). PM1.0/PM4.0 have no
// BTHome object; they use non-standard IDs (0xEE/0xEF) emitted LAST so a receiver parses every standard
// object before it stops at the unknown tail (Home Assistant won't show them).
static const BTHomeMeasurementDef BTHOME_MEASUREMENTS[] = {
    {BTHOME_SEL_BATTERY, "battery",     "Battery",         0x01, 2, 4,  "core"},
    {BTHOME_SEL_TEMP,    "temperature", "Temperature",     0x02, 3, 2,  "core"},
    {BTHOME_SEL_HUM,     "humidity",    "Humidity",        0x03, 3, 3,  "core"},
    {BTHOME_SEL_PRESS,   "pressure",    "Pressure",        0x04, 4, 6,  "optional"},
    {BTHOME_SEL_VOLTAGE, "voltage",     "Battery Voltage", 0x0C, 3, 8,  "optional"},
    {BTHOME_SEL_PM25,    "pm25",        "PM2.5",           0x0D, 3, 5,  "optional"},
    {BTHOME_SEL_PM10,    "pm10",        "PM10",            0x0E, 3, 7,  "optional"},
    {BTHOME_SEL_CO2,     "co2",         "CO2",             0x12, 3, 1,  "core"},
    {BTHOME_SEL_PM1,     "pm1",         "PM1.0",           0xEE, 3, 9,  "nonnative"},
    {BTHOME_SEL_PM4,     "pm4",         "PM4.0",           0xEF, 3, 10, "nonnative"},
};
static const size_t BTHOME_MEASUREMENT_COUNT = sizeof(BTHOME_MEASUREMENTS) / sizeof(BTHOME_MEASUREMENTS[0]);

// Usable BTHome service-data bytes: 31-byte legacy advert - 3 (flags AD) - 4 (service-data
// AD header) = 24. Conservative for a BTHome-only primary advert; scan-response mode has ~27.
static constexpr uint8_t BTHOME_MAX_SERVICE_DATA = 24;

bool bthomeMeasurementAvailable(uint32_t bit) {
    switch (bit) {
        case BTHOME_SEL_CO2:
            return sensors.isUnitRegistered(UNIT::CO2);
        case BTHOME_SEL_TEMP:
            return sensors.isUnitRegistered(UNIT::TEMP) || sensors.isUnitRegistered(UNIT::CO2TEMP) || sensors.isUnitRegistered(UNIT::CO2);
        case BTHOME_SEL_HUM:
            return sensors.isUnitRegistered(UNIT::HUM) || sensors.isUnitRegistered(UNIT::CO2HUM) || sensors.isUnitRegistered(UNIT::CO2);
        case BTHOME_SEL_PRESS:
            return sensors.isUnitRegistered(UNIT::PRESS);
        case BTHOME_SEL_PM25:
        case BTHOME_SEL_PM10:
        case BTHOME_SEL_PM1:
        case BTHOME_SEL_PM4:
            return sensors.isUnitRegistered(UNIT::PM25);
        case BTHOME_SEL_BATTERY:
            return true;  // battery level is always encodable
        case BTHOME_SEL_VOLTAGE:
            return hasBattery;
        default:
            return false;
    }
}

// Remove selections for hardware that is not registered. When called after
// startup sensor detection, persist only when the loaded NVS mask was stale.
bool sanitizeBTHomeSensorSelection(bool persistIfChanged, const char *reason) {
    uint32_t sanitized = bthomeSensors;
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        if (!bthomeMeasurementAvailable(m.bit)) {
            sanitized &= ~m.bit;
        }
    }
    if (sanitized == bthomeSensors) {
        return false;
    }

    uint32_t removed = bthomeSensors & ~sanitized;
    bthomeSensors = sanitized;
    Serial.printf("-->[BLE ] Cleared unavailable BTHome sensor bits: 0x%08lX (%s)\n",
                  static_cast<unsigned long>(removed),
                  reason ? reason : "availability changed");

    if (persistIfChanged) {
        preferences.begin("CO2-Gadget", false);
        preferences.putUInt("bthomeSensors", bthomeSensors);
        preferences.end();
        Serial.println("-->[PREF] Persisted sanitized BTHome sensor selection to NVR.");
    }
    refreshBTHomeBLESettings(reason ? reason : "BTHome sensor availability changed", true);
    return true;
}

bool bthomeMeasurementValid(uint32_t bit) {
    switch (bit) {
        case BTHOME_SEL_CO2:
            return (co2 >= 400) && (co2 <= 5000);
        case BTHOME_SEL_TEMP:
            return (temp >= -40) && (temp <= 85);
        case BTHOME_SEL_HUM:
            return (hum >= 0) && (hum <= 100);
        case BTHOME_SEL_PRESS:
            return (pressureHpa >= 300.0f) && (pressureHpa <= 1100.0f);
        case BTHOME_SEL_PM25:
        case BTHOME_SEL_PM10:
        case BTHOME_SEL_PM1:
        case BTHOME_SEL_PM4:
            return true;
        default:
            return false;  // battery / voltage are not stand-alone sensor readings
    }
}

void appendBTHomeMeasurement(std::string &payload, uint32_t bit) {
    switch (bit) {
        case BTHOME_SEL_BATTERY:
            appendBTHomeUInt8(payload, 0x01);
            appendBTHomeUInt8(payload, getBTHomeBatteryLevel());
            break;
        case BTHOME_SEL_TEMP:
            appendBTHomeUInt8(payload, 0x02);
            appendBTHomeInt16(payload, encodeBTHomeTemperature(temp));
            break;
        case BTHOME_SEL_HUM:
            appendBTHomeUInt8(payload, 0x03);
            appendBTHomeUInt16(payload, encodeBTHomeHumidity(hum));
            break;
        case BTHOME_SEL_PRESS:
            appendBTHomeUInt8(payload, 0x04);
            appendBTHomeUInt24(payload, encodeBTHomePressure(pressureHpa));
            break;
        case BTHOME_SEL_VOLTAGE:
            appendBTHomeUInt8(payload, 0x0C);
            appendBTHomeUInt16(payload, encodeBTHomeBatteryVoltage(batteryVoltage));
            break;
        case BTHOME_SEL_PM25:
            appendBTHomeUInt8(payload, 0x0D);
            appendBTHomeUInt16(payload, pm25);
            break;
        case BTHOME_SEL_PM10:
            appendBTHomeUInt8(payload, 0x0E);
            appendBTHomeUInt16(payload, pm10);
            break;
        case BTHOME_SEL_CO2:
            appendBTHomeUInt8(payload, 0x12);
            appendBTHomeUInt16(payload, static_cast<uint16_t>(co2));
            break;
        case BTHOME_SEL_PM1:  // non-standard object id (no BTHome PM1.0); emitted last
            appendBTHomeUInt8(payload, 0xEE);
            appendBTHomeUInt16(payload, pm1);
            break;
        case BTHOME_SEL_PM4:  // non-standard object id (no BTHome PM4.0); emitted last
            appendBTHomeUInt8(payload, 0xEF);
            appendBTHomeUInt16(payload, pm4);
            break;
        default:
            break;
    }
}

// Bytes available for the measurement objects (excludes device-info, packet-id, counter, MIC).
uint8_t bthomeMeasurementsBudget(bool encrypted, bool includePacketId) {
    uint8_t overhead = encrypted ? (1 + 4 + BTHOME_ENCRYPTION_MIC_SIZE) : (1 + (includePacketId ? 2 : 0));
    return (BTHOME_MAX_SERVICE_DATA > overhead) ? (BTHOME_MAX_SERVICE_DATA - overhead) : 0;
}

// Pick, by priority and budget, which selected+available measurements are advertised.
uint32_t bthomeFitMask(bool encrypted, bool includePacketId, uint8_t *outUsedBytes = nullptr) {
    uint8_t budget = bthomeMeasurementsBudget(encrypted, includePacketId);
    uint8_t used = 0;
    uint32_t included = 0;
    for (uint8_t prio = 1; prio <= BTHOME_MEASUREMENT_COUNT; ++prio) {
        for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
            const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
            if (m.priority != prio) {
                continue;
            }
            if ((bthomeSensors & m.bit) && bthomeMeasurementAvailable(m.bit) && (used + m.totalBytes <= budget)) {
                used += m.totalBytes;
                included |= m.bit;
            }
            break;
        }
    }
    if (outUsedBytes) {
        *outUsedBytes = used;
    }
    return included;
}

// Replaces the CO2-mandatory isValidBLEMeasurement() gate for BTHome: true when at least one
// selected, available, non-battery measurement is in range (so environment-only units advertise).
bool isValidBTHomeMeasurement() {
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        if ((m.bit == BTHOME_SEL_BATTERY) || (m.bit == BTHOME_SEL_VOLTAGE)) {
            continue;
        }
        if ((bthomeSensors & m.bit) && bthomeMeasurementAvailable(m.bit) && bthomeMeasurementValid(m.bit)) {
            return true;
        }
    }
    return false;
}

struct BTHomePayloadFit {
    uint8_t usedBytes;
    uint8_t budgetBytes;
    uint8_t fittedCount;
    uint8_t availableSelectedCount;
    uint8_t totalSelectedCount;
};

BTHomePayloadFit computeBTHomePayloadFit(bool encrypted) {
    BTHomePayloadFit fit = {};
    bool includePacketId = !encrypted;
    fit.budgetBytes = bthomeMeasurementsBudget(encrypted, includePacketId);
    uint32_t included = bthomeFitMask(encrypted, includePacketId, &fit.usedBytes);
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        if (!(bthomeSensors & m.bit)) {
            continue;
        }
        fit.totalSelectedCount++;
        if (bthomeMeasurementAvailable(m.bit)) {
            fit.availableSelectedCount++;
        }
        if (included & m.bit) {
            fit.fittedCount++;
        }
    }
    return fit;
}

void printBTHomePayloadProjection() {
    BTHomePayloadFit fit = computeBTHomePayloadFit(bthomeEncryption);
    String msg = "-->[BLE ] BTHome payload (" + String(bthomeEncryption ? "encrypted" : "plain") + "): " +
                 String(fit.usedBytes) + "/" + String(fit.budgetBytes) + " bytes, " +
                 String(fit.fittedCount) + " of " + String(fit.availableSelectedCount) + " available selected sensors fit";
    if (fit.totalSelectedCount > fit.availableSelectedCount) {
        msg += " (" + String(fit.totalSelectedCount - fit.availableSelectedCount) + " selected, not detected)";
    }
    Serial.println(msg);
}

void appendBTHomeSensorsJson(JsonDocument &doc) {
    JsonArray arr = doc["bthomeSensors"].to<JsonArray>();
    uint32_t includedPlain = bthomeFitMask(false, true);
    uint32_t includedEnc = bthomeFitMask(true, false);
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        JsonObject o = arr.add<JsonObject>();
        o["key"] = m.key;
        o["label"] = m.label;
        o["obj"] = m.objectId;
        o["bytes"] = m.totalBytes;
        o["prio"] = m.priority;
        o["group"] = m.group;
        o["native"] = strcmp(m.group, "nonnative") != 0;
        bool available = bthomeMeasurementAvailable(m.bit);
        o["available"] = available;
        o["selected"] = available && ((bthomeSensors & m.bit) != 0);
        o["willSendPlain"] = (includedPlain & m.bit) != 0;
        o["willSendEnc"] = (includedEnc & m.bit) != 0;
    }
    doc["bthomeBudgetMax"] = BTHOME_MAX_SERVICE_DATA;
}

uint32_t bthomeApplySelectionJson(JsonObjectConst sel, uint32_t current) {
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        if (!bthomeMeasurementAvailable(m.bit)) {
            current &= ~m.bit;
            continue;
        }
        if (sel[m.key].is<bool>()) {
            if (sel[m.key].as<bool>()) {
                current |= m.bit;
            } else {
                current &= ~m.bit;
            }
        }
    }
    return current;
}

std::string buildBTHomeMeasurements(bool incrementPacketId, bool includePacketId = true) {
    std::string payload;
    if (!activeBTHome || !isValidBTHomeMeasurement()) {
        return payload;
    }

    if (includePacketId && incrementPacketId) {
        ++bthomePacketId;
    }

    uint32_t included = bthomeFitMask(bthomeEncryption, includePacketId);

    payload.reserve(BTHOME_MAX_SERVICE_DATA);
    if (includePacketId) {
        appendBTHomeUInt8(payload, 0x00);
        appendBTHomeUInt8(payload, bthomePacketId);
    }
    // Emit in ascending object-id order (table order) as BTHome receivers expect.
    for (size_t i = 0; i < BTHOME_MEASUREMENT_COUNT; ++i) {
        const BTHomeMeasurementDef &m = BTHOME_MEASUREMENTS[i];
        if (included & m.bit) {
            appendBTHomeMeasurement(payload, m.bit);
        }
    }

    return payload;
}

std::string buildBTHomeServiceData(bool incrementPacketId) {
    std::string measurements = buildBTHomeMeasurements(incrementPacketId, !bthomeEncryption);
    if (measurements.empty()) {
        return measurements;
    }

    if (bthomeEncryption) {
        std::string encryptedPayload;
        if (encryptBTHomePayload(measurements, encryptedPayload)) {
            return encryptedPayload;
        }
        return std::string();
    }

    std::string payload;
    payload.reserve(1 + measurements.length());
    appendBTHomeUInt8(payload, BTHOME_DEVICE_INFO);
    payload += measurements;
    return payload;
}

void invalidateBTHomeServiceDataCache() {
    bthomeCachedServiceData.clear();
    bthomeCachedServiceDataValid = false;
}

bool updateBTHomeAdvertisementData(bool incrementPacketId, bool forcePrimaryAdvertisement = false, bool rebuildPayload = true) {
    if (!activeBTHome) {
        invalidateBTHomeServiceDataCache();
        return false;
    }

    if (!isValidBTHomeMeasurement()) {
        invalidateBTHomeServiceDataCache();
        Serial.println("-->[BLE ] BTHome payload skipped: no valid selected measurement. CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
        return false;
    }

    if (rebuildPayload || !bthomeCachedServiceDataValid) {
        bthomeCachedServiceData = buildBTHomeServiceData(incrementPacketId);
        bthomeCachedServiceDataValid = !bthomeCachedServiceData.empty();
    }

    if (!bthomeCachedServiceDataValid) {
        Serial.println("-->[BLE ] BTHome payload skipped: service data is empty.");
        return false;
    }

    NimBLEAdvertisementData advertisementData;
    advertisementData.setServiceData(NimBLEUUID(static_cast<uint16_t>(BTHOME_UUID)), bthomeCachedServiceData);

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    if (sensirionBLEInitialized && !forcePrimaryAdvertisement) {
        advertising->enableScanResponse(true);
        bool updated = advertising->setScanResponseData(advertisementData);
        updated = advertising->refreshAdvertisingData() && updated;
        if (!advertising->isAdvertising()) {
            updated = advertising->start() && updated;
        }
        if (!updated) {
            Serial.println("-->[BLE ] BTHome scan response update failed.");
            return false;
        }
    } else {
        advertisementData.setFlags(0x06);
        advertising->enableScanResponse(false);
        advertising->setAdvertisingInterval(BTHOME_ADV_INTERVAL);
        advertising->stop();
        bool updated = advertising->setAdvertisementData(advertisementData);
        updated = advertising->start() && updated;
        if (!updated) {
            Serial.println("-->[BLE ] BTHome advertisement update failed.");
            return false;
        }
    }

#ifdef DEBUG_BLE
    String bthomeDebug = "-->[BLE ] BTHome CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %, Battery: " + String(getBTHomeBatteryLevel()) + "%, Voltage: " + String(batteryVoltage, 2) + " V";
    if (sensors.isUnitRegistered(UNIT::PM25)) {
        bthomeDebug += ", PM2.5: " + String(pm25) + " ug/m3, PM10: " + String(pm10) + " ug/m3";
    }
    bthomeDebug += ", Encrypted: " + String(bthomeEncryption ? "yes" : "no");
    Serial.println(bthomeDebug);
#endif
    return true;
}

bool restoreSensirionAdvertisementData() {
    if (!sensirionBLEInitialized) {
        return false;
    }

    if (!writeSensirionCurrentSample()) {
        return false;
    }
    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    NimBLEAdvertisementData emptyScanResponse;
    advertising->stop();
    advertising->setScanResponseData(emptyScanResponse);
    advertising->enableScanResponse(false);
    provider.commitSample();
    return true;
}

bool clearBTHomeAdvertisementData() {
    invalidateBTHomeServiceDataCache();
    if (!bleInitialized) {
        return true;
    }

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    if (sensirionBLEInitialized) {
        NimBLEAdvertisementData emptyScanResponse;
        advertising->setScanResponseData(emptyScanResponse);
        advertising->enableScanResponse(false);
        return advertising->refreshAdvertisingData();
    }

    advertising->stop();
    return true;
}

void ensureBTHomeAdvertisingActive() {
    if (!activeBTHome || !bleInitialized) {
        return;
    }

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    if (advertising->isAdvertising()) {
        return;
    }

    Serial.println("-->[BLE ] BTHome advertising was stopped; restarting.");
    if (sensirionBLEInitialized) {
        advertising->start();
        updateBTHomeAdvertisementData(false);
    } else {
        updateBTHomeAdvertisementData(false);
    }
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
    if (!enableBLE) {
        disableBLE();
        return;
    }

    if (!activeBLE && !activeBTHome) {
        disableBLE();
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
        bool validForSensirion = isValidBLEMeasurement();
#ifdef SUPPORT_BTHOME_BLE
        bool validForBTHome = isValidBTHomeMeasurement();  // [BTHOME-SENSEL] env-only devices can be valid here without CO2
#else
        bool validForBTHome = false;
#endif
        bool anyValid = validForSensirion || validForBTHome;
        bool evaluateCO2 = validForSensirion;
        bool evaluateTemp = validForSensirion;
        bool evaluateHum = validForSensirion;
#ifdef SUPPORT_BTHOME_BLE
        if (activeBTHome) {
            evaluateCO2 = evaluateCO2 || ((bthomeSensors & BTHOME_SEL_CO2) && bthomeMeasurementAvailable(BTHOME_SEL_CO2) && bthomeMeasurementValid(BTHOME_SEL_CO2));
            evaluateTemp = evaluateTemp || ((bthomeSensors & BTHOME_SEL_TEMP) && bthomeMeasurementAvailable(BTHOME_SEL_TEMP) && bthomeMeasurementValid(BTHOME_SEL_TEMP));
            evaluateHum = evaluateHum || ((bthomeSensors & BTHOME_SEL_HUM) && bthomeMeasurementAvailable(BTHOME_SEL_HUM) && bthomeMeasurementValid(BTHOME_SEL_HUM));
        }
#endif
        bool thresholdsPassed = outputEnabled && anyValid && (bypassThresholds || evaluateBLEPublishThresholds(co2, temp, hum, evaluateCO2, evaluateTemp, evaluateHum));

        if (outputEnabled && thresholdsPassed) {
            if (sensirionBLEInitialized && validForSensirion) {
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
                    Serial.println("-->[BLE ] BTHome wake payload " + String(bthomeAdvertised ? "installed" : "skipped") + ". thresholdsPassed: " + String(thresholdsPassed) + ", CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %, Scan response: " + String(sensirionBLEInitialized ? "yes" : "no"));
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
        if (sensirionBLEInitialized) {
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

void refreshBTHomeBLESettings(const char* reason, bool forcePublish) {
#if defined(SUPPORT_BLE) && defined(SUPPORT_BTHOME_BLE)
    String logReason = reason ? String(reason) : String("BTHome settings changed");
    Serial.println("-->[BLE ] " + logReason + "; refreshing BLE advertising.");

    if (!enableBLE || (!activeBLE && !activeBTHome)) {
        if (bleInitialized) {
            clearBTHomeAdvertisementData();
        }
        Serial.println("-->[BLE ] BLE output is disabled after BTHome settings refresh.");
        return;
    }

    if (!bleInitialized) {
        initBLE();
    }

    if (!activeBTHome) {
        bool cleared = clearBTHomeAdvertisementData();
        if (activeBLE && sensirionBLEInitialized) {
            restoreSensirionAdvertisementData();
        }
        Serial.println("-->[BLE ] BTHome advertisement " + String(cleared ? "cleared." : "clear failed."));
        return;
    }

    if (forcePublish) {
        bool published = publishBLE(true, true);
        Serial.println("-->[BLE ] BLE refresh publish " + String(published ? "completed." : "skipped."));
    } else if (activeBTHome) {
        updateBTHomeAdvertisementData(false);
    }
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
