#include "CO2_Gadget_Thresholds.h"

// Class to manage sensor thresholds
ThresholdManager::ThresholdManager() {
    loadThresholdsFromNVR();
#ifdef DEBUG_THRESHOLDS
    Serial.print("-->[THRE] Thresholds loaded from NVRAM\t: ");
    printThresholdsFromNVR();
#endif
}

void ThresholdManager::setThresholds(OutputType outputType, bool enabled, bool useOnlyInLowPower, uint16_t keepAlive, uint16_t co2ThresholdAbsolute, float tempThresholdAbsolute, float humThresholdAbsolute, uint16_t co2ThresholdPercentage, float tempThresholdPercentage, float humThresholdPercentage, bool co2CombineWithAnd, bool tempCombineWithAnd, bool humCombineWithAnd) {
    thresholds[outputType].enabled = enabled;
    thresholds[outputType].useOnlyInLowPower = false;
    thresholds[outputType].keepAlive = 0;
    thresholds[outputType].co2ThresholdAbsolute = co2ThresholdAbsolute;
    thresholds[outputType].tempThresholdAbsolute = tempThresholdAbsolute;
    thresholds[outputType].humThresholdAbsolute = humThresholdAbsolute;
    thresholds[outputType].co2ThresholdPercentage = co2ThresholdPercentage;
    thresholds[outputType].tempThresholdPercentage = tempThresholdPercentage;
    thresholds[outputType].humThresholdPercentage = humThresholdPercentage;
    thresholds[outputType].previousCO2Value = 0;
    thresholds[outputType].previousTemperatureValue = 0.0f;
    thresholds[outputType].previousHumidityValue = 0.0f;
    thresholds[outputType].co2CombineWithAnd = true;
    thresholds[outputType].tempCombineWithAnd = true;
    thresholds[outputType].humCombineWithAnd = true;
#ifdef DEBUG_THRESHOLDS
    Serial.println("-->[THRE] Thresholds set for output type: " + String(outputType));
    Serial.println("-->[THRE] Setting enabled: " + String(enabled));
    Serial.println("-->[THRE] Setting co2ThresholdAbsolute: " + String(co2ThresholdAbsolute));
    Serial.println("-->[THRE] Setting tempThresholdAbsolute: " + String(tempThresholdAbsolute));
    Serial.println("-->[THRE] Setting humThresholdAbsolute: " + String(humThresholdAbsolute));
    Serial.println("-->[THRE] Setting co2ThresholdPercentage: " + String(co2ThresholdPercentage));
    Serial.println("-->[THRE] Setting tempThresholdPercentage: " + String(tempThresholdPercentage));
    Serial.println("-->[THRE] Setting humThresholdPercentage: " + String(humThresholdPercentage));
    Serial.println("-->[THRE] Setting previousCO2Value: " + String(thresholds[outputType].previousCO2Value));
    Serial.println("-->[THRE] Setting previousTemperatureValue: " + String(thresholds[outputType].previousTemperatureValue));
    Serial.println("-->[THRE] Setting previousHumidityValue: " + String(thresholds[outputType].previousHumidityValue));
    Serial.println("-->[THRE] Setting co2CombineWithAnd: " + String(thresholds[outputType].co2CombineWithAnd));
    Serial.println("-->[THRE] Setting tempCombineWithAnd: " + String(thresholds[outputType].tempCombineWithAnd));
    Serial.println("-->[THRE] Setting humCombineWithAnd: " + String(thresholds[outputType].humCombineWithAnd));
#endif
}

ThresholdConfig ThresholdManager::getThresholds(OutputType outputType) {
    return thresholds[outputType];
}

void ThresholdManager::printThresholdsDifferences(OutputType outputType) {
    // Function to print the differences of the thresholds
    Serial.println("-->[THRE] Thresholds differences:");
    Serial.println("-->[THRE] CO2: " + String(thresholds[outputType].co2ThresholdAbsolute) + " ppm or " + String(thresholds[outputType].co2ThresholdPercentage) + "%");
    Serial.println("-->[THRE] Temperature: " + String(thresholds[outputType].tempThresholdAbsolute) + "°C or " + String(thresholds[outputType].tempThresholdPercentage) + "%");
    Serial.println("-->[THRE] Humidity: " + String(thresholds[outputType].humThresholdAbsolute) + "%RH or " + String(thresholds[outputType].humThresholdPercentage) + "%");
}

void ThresholdManager::updatePreviousValues(OutputType outputType, uint16_t co2, float temp, float hum) {
    // Update the last measured values
    thresholds[outputType].previousCO2Value = co2;
    thresholds[outputType].previousTemperatureValue = temp;
    thresholds[outputType].previousHumidityValue = hum;
}

/**
 * Checks if the given CO2, temperature, and humidity values exceed the thresholds
 * for the specified output type. If the thresholds are exceeded, the function updates
 * the previous values and returns true. Otherwise, it returns false.
 *
 * @param outputType The output type to check against the thresholds.
 * @param co2 The CO2 value to check against the thresholds.
 * @param temp The temperature value to check against the thresholds.
 * @param hum The humidity value to check against the thresholds.
 *
 * @return True if the thresholds are exceeded, false otherwise.
 */
bool ThresholdManager::checkAndMaybeUpdateThresholds(OutputType outputType, uint16_t co2, float temp, float hum) {
    ThresholdConfig& config = thresholds[outputType];

    if (!config.enabled) {
        // Thresholds are not enabled for this output type
        return false;
    }

    // Check absolute thresholds
    bool co2ExceedsAbsolute = abs(co2 - config.previousCO2Value) >= config.co2ThresholdAbsolute;
    bool tempExceedsAbsolute = abs(temp - config.previousTemperatureValue) >= config.tempThresholdAbsolute;
    bool humExceedsAbsolute = abs(hum - config.previousHumidityValue) >= config.humThresholdAbsolute;
#ifdef DEBUG_THRESHOLDS
    if (co2ExceedsAbsolute) {
        Serial.println("-->[THRE] CO2 exceeds absolute threshold. Old value: " + String(config.previousCO2Value) + ", New value: " + String(co2) + ", Difference: " + String(abs(co2 - config.previousCO2Value)));
    }
#endif

    // Check percentage thresholds (avoid division by zero)
    bool co2ExceedsPercentage = (config.previousCO2Value == 0) ? true : ((config.co2ThresholdPercentage != 0) && (abs(co2 - config.previousCO2Value) >= (config.previousCO2Value * config.co2ThresholdPercentage / 100)));
    bool tempExceedsPercentage = (config.previousTemperatureValue == 0) ? true : ((config.tempThresholdPercentage != 0) && (abs(temp - config.previousTemperatureValue) >= (config.previousTemperatureValue * config.tempThresholdPercentage / 100)));
    bool humExceedsPercentage = (config.previousHumidityValue == 0) ? true : ((config.humThresholdPercentage != 0) && (abs(hum - config.previousHumidityValue) >= (config.previousHumidityValue * config.humThresholdPercentage / 100)));
#ifdef DEBUG_THRESHOLDS
    if (co2ExceedsPercentage) {
        Serial.println("-->[THRE] CO2 exceeds percentage threshold. Old value: " + String(config.previousCO2Value) + ", New value: " + String(co2) + ", Difference (%): " + String(config.previousCO2Value != 0 ? (abs(co2 - config.previousCO2Value) * 100 / config.previousCO2Value) : 0));

    }
#endif

    // Check if both absolute and percentage thresholds are exceeded (AND or OR combination)
    bool co2ThresholdsExceeded = (config.co2CombineWithAnd ? co2ExceedsAbsolute && co2ExceedsPercentage : co2ExceedsAbsolute || co2ExceedsPercentage);
        bool tempThresholdsExceeded = (config.tempCombineWithAnd ? tempExceedsAbsolute && tempExceedsPercentage : tempExceedsAbsolute || tempExceedsPercentage);
        bool humThresholdsExceeded = (config.humCombineWithAnd ? humExceedsAbsolute && humExceedsPercentage : humExceedsAbsolute || humExceedsPercentage);
    #ifdef DEBUG_THRESHOLDS
        if (co2ThresholdsExceeded) {
            // If co2CombineWithAnd = true append AND otherwise append OR
            Serial.println("-->[THRE] CO2 threshold combination meeted evaluating with: " + String(config.co2CombineWithAnd ? "AND" : "OR"));
        } else {
            if (co2ExceedsAbsolute) Serial.println("-->[THRE] CO2 threshold combination not meeted evaluating with: " + String(config.co2CombineWithAnd ? "AND" : "OR"));
        }
    #endif

        // Check if any threshold is exceeded
        if (co2ThresholdsExceeded || tempThresholdsExceeded || humThresholdsExceeded) {
            // Update previous values
            updatePreviousValues(outputType, co2, temp, hum);
            return true;  // Thresholds exceeded
        }

    return false;  // Thresholds not exceeded
}

/**
 * Evaluates if the sensor readings pass the thresholds for the given output type.
 *
 * @param outputType The output type to check against the thresholds.
 * @param co2 The CO2 value to check against the thresholds.
 * @param temp The temperature value to check against the thresholds.
 * @param hum The humidity value to check against the thresholds.
 *
 * @return True if the sensor readings pass the thresholds, false otherwise.
 */
bool ThresholdManager::evaluateThresholds(OutputType outputType, uint16_t co2, float temp, float hum) {
    // If threshold is not enabled for outputType, return true
    if (!thresholds[outputType].enabled) return true;
    // Evaluate if the sensor readings pass the thresholds
    return checkAndMaybeUpdateThresholds(outputType, co2, temp, hum);
}

/**
 * Returns the threshold data for the specified output type as a JSON string.
 *
 * @param outputType The output type for which to retrieve the threshold data.
 *
 * @return The threshold data as a JSON string.
 *
 * @throws None
 */
String ThresholdManager::getThresholdsAsJson(OutputType outputType) {
    const size_t capacity = JSON_ARRAY_SIZE(NUM_OUTPUTS) + NUM_OUTPUTS * JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);

    doc["enabled"] = thresholds[outputType].enabled;
    doc["thrOnlyInLowP"] = thresholds[outputType].useOnlyInLowPower;
    doc["thrKeepAlive"] = thresholds[outputType].keepAlive;
    doc["thrCo2Abs"] = thresholds[outputType].co2ThresholdAbsolute;
    doc["thrTempAbs"] = thresholds[outputType].tempThresholdAbsolute;
    doc["thrHumAbs"] = thresholds[outputType].humThresholdAbsolute;
    doc["thrCo2Per"] = thresholds[outputType].co2ThresholdPercentage;
    doc["thrTempPer"] = thresholds[outputType].tempThresholdPercentage;
    doc["thrHumPer"] = thresholds[outputType].humThresholdPercentage;
    doc["prevCO2"] = thresholds[outputType].previousCO2Value;
    doc["prevTemp"] = thresholds[outputType].previousTemperatureValue;
    doc["prevHum"] = thresholds[outputType].previousHumidityValue;
    doc["thrCo2CombAnd"] = thresholds[outputType].co2CombineWithAnd;
    doc["thrTempCombAnd"] = thresholds[outputType].tempCombineWithAnd;
    doc["thrHumCombAnd"] = thresholds[outputType].humCombineWithAnd;

    String json;
    serializeJson(doc, json);
#ifdef DEBUG_THRESHOLDS
    Serial.print("-->[THRE] Thresholds as JSON: ");
    Serial.println(json);
#endif
    return json;
}

/**
 * Method to return all threshold data as a JSON string.
 *
 * @param None
 *
 * @return The threshold data as a JSON string.
 *
 * @throws None
 */
String ThresholdManager::getAllThresholdsAsJson() {
    const size_t capacity = JSON_ARRAY_SIZE(NUM_OUTPUTS) + NUM_OUTPUTS * JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);

    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        JsonObject obj = doc.createNestedObject();
        obj["enabled"] = thresholds[i].enabled;
        obj["thrOnlyInLowP"] = thresholds[i].useOnlyInLowPower;
        obj["thrKeepAlive"] = thresholds[i].keepAlive;
        obj["thrCo2Abs"] = thresholds[i].co2ThresholdAbsolute;
        obj["thrTempAbs"] = thresholds[i].tempThresholdAbsolute;
        obj["thrHumAbs"] = thresholds[i].humThresholdAbsolute;
        obj["thrCo2Per"] = thresholds[i].co2ThresholdPercentage;
        obj["thrTempPer"] = thresholds[i].tempThresholdPercentage;
        obj["thrHumPer"] = thresholds[i].humThresholdPercentage;
        obj["prevCO2"] = thresholds[i].previousCO2Value;
        obj["prevTemp"] = thresholds[i].previousTemperatureValue;
        obj["prevHum"] = thresholds[i].previousHumidityValue;
        obj["thrCo2CombAnd"] = thresholds[i].co2CombineWithAnd;
        obj["thrTempCombAnd"] = thresholds[i].tempCombineWithAnd;
        obj["thrHumCombAnd"] = thresholds[i].humCombineWithAnd;
    }

    String json;
    serializeJson(doc, json);
#ifdef DEBUG_THRESHOLDS
    Serial.print("-->[THRE] All thresholds as JSON: ");
    Serial.println(json);
#endif
    return json;
}

/**
 * Saves the thresholds to non-volatile memory (NVRAM).
 *
 * This function opens the NVRAM with the key "thresholds" and saves the thresholds for each output.
 * The thresholds are stored as key-value pairs in the NVRAM. The keys are constructed by appending
 * "_enabled", "_OnlyInLowP", "_KeepAlive", "_Co2Abs", "_TempAbs", "_HumAbs", "_Co2Per", "_TempPer",
 * "_HumPer", "_prevCO2", "_prevTemp", "_prevHum", "_Co2CombAnd", "_TempCombAnd", and "_HumCombAnd" to
 * the output index.
 *
 * @throws None
 */
void ThresholdManager::saveThresholdsToNVR() {
    Preferences thresholdsPrefs;
    thresholdsPrefs.begin("thresholds", false);  // Open NVRAM with THRESHOLD_PREF_KEY key

    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        String key = String(i);
        thresholdsPrefs.putBool((key + "_enabled").c_str(), thresholds[i].enabled);
        thresholdsPrefs.putBool((key + "_OnlyInLowP").c_str(), thresholds[i].useOnlyInLowPower);
        thresholdsPrefs.putUShort((key + "_KeepAlive").c_str(), thresholds[i].keepAlive);
        thresholdsPrefs.putUShort((key + "_Co2Abs").c_str(), thresholds[i].co2ThresholdAbsolute);
        thresholdsPrefs.putFloat((key + "_TempAbs").c_str(), thresholds[i].tempThresholdAbsolute);
        thresholdsPrefs.putFloat((key + "_HumAbs").c_str(), thresholds[i].humThresholdAbsolute);
        thresholdsPrefs.putUShort((key + "_Co2Per").c_str(), thresholds[i].co2ThresholdPercentage);
        thresholdsPrefs.putUShort((key + "_TempPer").c_str(), thresholds[i].tempThresholdPercentage);
        thresholdsPrefs.putUShort((key + "_HumPer").c_str(), thresholds[i].humThresholdPercentage);
        thresholdsPrefs.putUShort((key + "_prevCO2").c_str(), thresholds[i].previousCO2Value);
        thresholdsPrefs.putFloat((key + "_prevTemp").c_str(), thresholds[i].previousTemperatureValue);
        thresholdsPrefs.putFloat((key + "_prevHum").c_str(), thresholds[i].previousHumidityValue);
        thresholdsPrefs.putBool((key + "_Co2CombAnd").c_str(), thresholds[i].co2CombineWithAnd);
        thresholdsPrefs.putBool((key + "_TempCombAnd").c_str(), thresholds[i].tempCombineWithAnd);
        thresholdsPrefs.putBool((key + "_HumCombAnd").c_str(), thresholds[i].humCombineWithAnd);
    }
    thresholdsPrefs.end();  // Finish using NVRAM
}

/**
 * Sets the thresholds for the ThresholdManager object from a JSON string.
 *
 * @param response The JSON string containing the thresholds.
 * @returns None
 * @example {"thresholds":[{"enabled":"on","thrOnlyInLowP":false,"thrKeepAlive":"5","thrCo2Abs":"20","thrTempAbs":"0.5","thrHumAbs":"1","thrCo2Per":"4","thrTempPer":"1","thrHumPer":"1","thrCo2CombAnd":true,"thrTempCombAnd":true,"thrHumCombAnd":true},{"enabled":"on","thrOnlyInLowP":false,"thrKeepAlive":"0","thrCo2Abs":"20","thrTempAbs":"0.5","thrHumAbs":"1","thrCo2Per":"5","thrTempPer":"1","thrHumPer":"1","thrCo2CombAnd":false,"thrTempCombAnd":false,"thrHumCombAnd":false},{"enabled":"on","thrOnlyInLowP":false,"thrKeepAlive":"0","thrCo2Abs":"20","thrTempAbs":"0.5","thrHumAbs":"1","thrCo2Per":"5","thrTempPer":"1","thrHumPer":"1","thrCo2CombAnd":false,"thrTempCombAnd":false,"thrHumCombAnd":false},{"enabled":false,"thrOnlyInLowP":false,"thrKeepAlive":"0","thrCo2Abs":"20","thrTempAbs":"0.5","thrHumAbs":"1","thrCo2Per":"5","thrTempPer":"1","thrHumPer":"1","thrCo2CombAnd":false,"thrTempCombAnd":false,"thrHumCombAnd":false}]}
 *
 * @throws None
 */
void ThresholdManager::setThresholdsFromJSON(String response) {
    const size_t capacity = JSON_ARRAY_SIZE(NUM_OUTPUTS) + NUM_OUTPUTS * JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);

    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        thresholds[i].enabled = doc["thresholds"][i]["enabled"];
        thresholds[i].useOnlyInLowPower = doc["thresholds"][i]["thrOnlyInLowP"];
        thresholds[i].keepAlive = doc["thresholds"][i]["thrKeepAlive"];
        thresholds[i].co2ThresholdAbsolute = doc["thresholds"][i]["thrCo2Abs"];
        thresholds[i].tempThresholdAbsolute = doc["thresholds"][i]["thrTempAbs"];
        thresholds[i].humThresholdAbsolute = doc["thresholds"][i]["thrHumAbs"];
        thresholds[i].co2ThresholdPercentage = doc["thresholds"][i]["thrCo2Per"];
        thresholds[i].tempThresholdPercentage = doc["thresholds"][i]["thrTempPer"];
        thresholds[i].humThresholdPercentage = doc["thresholds"][i]["thrHumPer"];
        thresholds[i].previousCO2Value = doc["thresholds"][i]["prevCO2"];
        thresholds[i].previousTemperatureValue = doc["thresholds"][i]["prevTemp"];
        thresholds[i].previousHumidityValue = doc["thresholds"][i]["prevHum"];
        thresholds[i].co2CombineWithAnd = doc["thresholds"][i]["thrCo2CombAnd"];
        thresholds[i].tempCombineWithAnd = doc["thresholds"][i]["thrTempCombAnd"];
        thresholds[i].humCombineWithAnd = doc["thresholds"][i]["thrHumCombAnd"];
    }
    saveThresholdsToNVR();
}

/**
 * Loads the thresholds from the non-volatile memory (NVRAM) into the ThresholdManager object.
 *
 * @throws None
 */
void ThresholdManager::loadThresholdsFromNVR() {
    Preferences thresholdsPrefs;
    thresholdsPrefs.begin("thresholds", true);  // Open NVRAM with THRESHOLD_PREF_KEY key

    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        String key = String(i);
        // Method to load thresholds from NVRAM
        thresholds[i].enabled = thresholdsPrefs.getBool((key + "_enabled").c_str(), false);
        thresholds[i].useOnlyInLowPower = thresholdsPrefs.getBool((key + "_OnlyInLowP").c_str(), true);
        thresholds[i].keepAlive = thresholdsPrefs.getUShort((key + "_KeepAlive").c_str(), 5);
        thresholds[i].co2ThresholdAbsolute = thresholdsPrefs.getUShort((key + "_Co2Abs").c_str(), 20);
        thresholds[i].tempThresholdAbsolute = thresholdsPrefs.getFloat((key + "_TempAbs").c_str(), 0.5f);
        thresholds[i].humThresholdAbsolute = thresholdsPrefs.getFloat((key + "_HumAbs").c_str(), 1.0f);
        thresholds[i].co2ThresholdPercentage = thresholdsPrefs.getUShort((key + "_Co2Per").c_str(), 5);
        thresholds[i].tempThresholdPercentage = thresholdsPrefs.getUShort((key + "_TempPer").c_str(), 1);
        thresholds[i].humThresholdPercentage = thresholdsPrefs.getUShort((key + "_HumPer").c_str(), 1);
        thresholds[i].previousCO2Value = thresholdsPrefs.getUShort((key + "_prevCO2").c_str(), 0);
        thresholds[i].previousTemperatureValue = thresholdsPrefs.getFloat((key + "_prevTemp").c_str(), 0.0f);
        thresholds[i].previousHumidityValue = thresholdsPrefs.getFloat((key + "_prevHum").c_str(), 0.0f);
        thresholds[i].co2CombineWithAnd = thresholdsPrefs.getBool((key + "_Co2CombAnd").c_str(), false);
        thresholds[i].tempCombineWithAnd = thresholdsPrefs.getBool((key + "_TempCombAnd").c_str(), false);
        thresholds[i].humCombineWithAnd = thresholdsPrefs.getBool((key + "_HumCombAnd").c_str(), false);
    }
    thresholdsPrefs.end();  // Finish using NVRAM
}

/**
 * Retrieves and prints thresholds stored in non-volatile memory.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void printThresholdsFromNVR() {
    Preferences thresholdsPrefs;
    thresholdsPrefs.begin("thresholds", true);  // Open NVRAM with THRESHOLD_PREF_KEY key
    Serial.println("-->[THRE] Thresholds in NVRAM");
    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        String key = String(i);
        Serial.println("-->[THRE] Threshold " + String(i) + " enabled: " + String(thresholdsPrefs.getBool((key + "_enabled").c_str(), false)));
        Serial.println("-->[THRE] Threshold " + String(i) + " useOnlyInLowPower: " + String(thresholdsPrefs.getBool((key + "_OnlyInLowP").c_str(), true)));
        Serial.println("-->[THRE] Threshold " + String(i) + " keepAlive: " + String(thresholdsPrefs.getUShort((key + "_KeepAlive").c_str(), 5)));
        Serial.println("-->[THRE] Threshold " + String(i) + " co2ThresholdAbsolute: " + String(thresholdsPrefs.getUShort((key + "_Co2Abs").c_str(), 20)));
        Serial.println("-->[THRE] Threshold " + String(i) + " tempThresholdAbsolute: " + String(thresholdsPrefs.getFloat((key + "_TempAbs").c_str(), 0.5f)));
        Serial.println("-->[THRE] Threshold " + String(i) + " humThresholdAbsolute: " + String(thresholdsPrefs.getFloat((key + "_HumAbs").c_str(), 1.0f)));
        Serial.println("-->[THRE] Threshold " + String(i) + " co2ThresholdPercentage: " + String(thresholdsPrefs.getUShort((key + "_Co2Per").c_str(), 5)));
        Serial.println("-->[THRE] Threshold " + String(i) + " tempThresholdPercentage: " + String(thresholdsPrefs.getUShort((key + "_TempPer").c_str(), 1)));
        Serial.println("-->[THRE] Threshold " + String(i) + " humThresholdPercentage: " + String(thresholdsPrefs.getUShort((key + "_HumPer").c_str(), 1)));
        Serial.println("-->[THRE] Threshold " + String(i) + " previousCO2Value: " + String(thresholdsPrefs.getUShort((key + "_prevCO2").c_str(), 0)));
        Serial.println("-->[THRE] Threshold " + String(i) + " previousTemperatureValue: " + String(thresholdsPrefs.getFloat((key + "_prevTemp").c_str(), 0.0f)));
        Serial.println("-->[THRE] Threshold " + String(i) + " previousHumidityValue: " + String(thresholdsPrefs.getFloat((key + "_prevHum").c_str(), 0.0f)));
        Serial.println("-->[THRE] Threshold " + String(i) + " co2CombineWithAnd: " + String(thresholdsPrefs.getBool((key + "_Co2CombAnd").c_str(), false)));
        Serial.println("-->[THRE] Threshold " + String(i) + " tempCombineWithAnd: " + String(thresholdsPrefs.getBool((key + "_TempCombAnd").c_str(), false)));
        Serial.println("-->[THRE] Threshold " + String(i) + " humCombineWithAnd: " + String(thresholdsPrefs.getBool((key + "_HumCombAnd").c_str(), false)));
    }
    thresholdsPrefs.end();  // Finish using NVRAM
}
