#ifndef CO2_Gadget_Preferences_h
#define CO2_Gadget_Preferences_h

#include <ArduinoJson.h>
#include <Preferences.h>
Preferences preferences;

uint8_t prefVersion = 0;
uint8_t prefRevision = 0;

void upgradePreferences() {
    if ((batteryDischargedMillivolts == 3500) && (prefVersion == 0) && (prefRevision == 0)) {
        batteryDischargedMillivolts = 3200;
        Serial.println("-->[PREF] Upgrading preferences batteryDischargedMillivolts to: " + String(batteryDischargedMillivolts));
        prefRevision = 1;
        putPreferences();
    }
}

void printPreferences() {
    Serial.println("-->[PREF]");
    Serial.println("-->[PREF] LOADED PREFERENCES FROM NVR:");
    Serial.println("-->[PREF] prefVersion:\t #" + String(prefVersion) + "#");
    Serial.println("-->[PREF] prefRevision:\t #" + String(prefRevision) + "#");
    Serial.println("-->[PREF] customCalValue: #" + String(customCalibrationValue) + "#");
    Serial.println("-->[PREF] tempOffset:\t #" + String(tempOffset, 1) + "#");
    Serial.println("-->[PREF] altitudeMeters:\t #" + String(altitudeMeters) + "#");
    Serial.println("-->[PREF] autoSelfCalibration:\t #" + String(autoSelfCalibration ? "Enabled" : "Disabled") + "#");
    Serial.println("-->[PREF] co2OrangeRange:\t #" + String(co2OrangeRange) + "#");
    Serial.println("-->[PREF] co2RedRange:\t #" + String(co2RedRange) + "#");
    Serial.println("-->[PREF] DisplayBrightness:\t #" + String(DisplayBrightness) + "#");
    Serial.println("-->[PREF] neopixBright:\t #" + String(neopixelBrightness) + "#");
    Serial.println("-->[PREF] selNeopxType:\t #" + String(selectedNeopixelType) + "#");
    Serial.println("-->[PREF] activeBLE is:\t#" + String(activeBLE ? "Enabled" : "Disabled") + "# (" + String(activeBLE) + ")");
    Serial.println("-->[PREF] activeWIFI is:\t#" + String(activeWIFI ? "Enabled" : "Disabled") + "# (" + String(activeWIFI) + ")");
    Serial.println("-->[PREF] activeMQTT is:\t#" + String(activeMQTT ? "Enabled" : "Disabled") + "# (" + String(activeMQTT) + ")");
    Serial.println("-->[PREF] activeESPNOW is:\t#" + String(activeESPNOW ? "Enabled" : "Disabled") + "# (" + String(activeESPNOW) + ")");
    Serial.println("-->[PREF] activeOTA is:\t#" + String(activeOTA ? "Enabled" : "Disabled") + "# (" + String(activeOTA) + ")");
    Serial.println("-->[PREF] rootTopic:\t#" + rootTopic + "#");
    Serial.println("-->[PREF] batDischgd:\t #" + String(batteryDischargedMillivolts) + "#");
    Serial.println("-->[PREF] batChargd:\t #" + String(batteryFullyChargedMillivolts) + "#");
    Serial.println("-->[PREF] vRef:\t #" + String(vRef) + "#");
    Serial.println("-->[PREF] mqttClientId:\t#" + mqttClientId + "#");
    Serial.println("-->[PREF] mqttBroker:\t#" + mqttBroker + "#");
    Serial.println("-->[PREF] mqttUser:\t#" + mqttUser + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[PREF] mqttPass:\t#" + mqttPass + "#");
#endif
    Serial.println("-->[PREF] tToDispOff:\t #" + String(timeToDisplayOff) + "#");
    Serial.println("-->[PREF] tToPubMQTT:\t #" + String(timeBetweenMQTTPublish) + "#");
    Serial.println("-->[PREF] tToPubESPNow:\t #" + String(timeBetweenESPNowPublish) + "#");
    Serial.println("-->[PREF] tKeepAlMQTT:\t #" + String(timeToKeepAliveMQTT) + "#");
    Serial.println("-->[PREF] tKeepAlESPNow:\t #" + String(timeToKeepAliveESPNow) + "#");
    Serial.println("-->[PREF] dispOffOnExP:\t#" + String(displayOffOnExternalPower ? "Enabled" : "Disabled") + "# (" + String(displayOffOnExternalPower) + ")");
    Serial.println("-->[PREF] wifiSSID:\t#" + wifiSSID + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[PREF] wifiPass:\t#" + wifiPass + "#");
#endif
    Serial.println("-->[PREF] hostName:\t#" + hostName + "#");
    Serial.println("-->[PREF] selCO2Sensor:\t #" + String(selectedCO2Sensor) + "#");
    Serial.println("-->[PREF] debugSensors is:\t#" + String(debugSensors ? "Enabled" : "Disabled") + "# (" + String(debugSensors) + ")");
    Serial.println("-->[PREF] displayReverse is:\t#" + String(displayReverse ? "Reversed" : "Normal") + "# (" + String(displayReverse) + ")");
    Serial.println("-->[PREF] showFahrenheit is:\t#" + String(showFahrenheit ? "Fahrenheit" : "Celsius") + "#");
    Serial.println("-->[PREF] measInterval:\t #" + String(measurementInterval) + "#");
    Serial.println("-->[PREF] outModeRelay is:\t#" + String(outputsModeRelay ? "Relay" : "RGB LED") + "#");
    Serial.println("-->[PREF] channelESPNow:\t #" + String(channelESPNow) + "#");
    Serial.println("-->[PREF] boardIdESPNow:\t #" + String(boardIdESPNow) + "#");
    Serial.println("-->[PREF] peerESPNow:\t #" + String(peerESPNowAddress[0], HEX) + ":" + String(peerESPNowAddress[1], HEX) + ":" + String(peerESPNowAddress[2], HEX) + ":" + String(peerESPNowAddress[3], HEX) + ":" + String(peerESPNowAddress[4], HEX) + ":" + String(peerESPNowAddress[5], HEX) + "#");

    Serial.println("-->[PREF] showTemp:\t #" + String(displayShowTemperature ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showHumidity:\t #" + String(displayShowHumidity ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showBattery:\t #" + String(displayShowBattery ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showBatteryVolt:\t #" + String(displayShowBatteryVoltage ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showCO2:\t #" + String(displayShowCO2 ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showPM25:\t #" + String(displayShowPM25 ? "Show" : "Hide") + "#");

    // Buzzer preferences
    Serial.println("-->[PREF] toneBuzzerBeep is:\t#" + String(toneBuzzerBeep) + "#");
    Serial.println("-->[PREF] durationBuzzerBeep is:\t#" + String(durationBuzzerBeep) + "#");
    Serial.println("-->[PREF] timeBetweenBuzzerBeeps is:\t#" + String(timeBetweenBuzzerBeeps) + "#");

    Serial.println();
    delay(50);
}

void initPreferences() {
    // preferences.begin("CO2-Gadget", false);
    // if (preferences.clear()) {
    //     Serial.println("-->[PREF] Preferences cleared");
    // } else {
    //     Serial.println("-->[PREF] Preferences NOT cleared");
    // }
    // preferences.end();
    // delay(000);
    preferences.begin("CO2-Gadget", false);
    prefVersion = preferences.getUInt("prefVersion", 0);
    prefRevision = preferences.getUInt("prefRevision", 0);
    customCalibrationValue = preferences.getUInt("customCalValue", 415);
    tempOffset = float(preferences.getFloat("tempOffset", 0));
    altitudeMeters = preferences.getUInt("altitudeMeters", 0);
    autoSelfCalibration = preferences.getBool("autoSelfCal", false);
    co2OrangeRange = preferences.getUInt("co2OrangeRange", 700);
    co2RedRange = preferences.getUInt("co2RedRange", 1000);
#ifdef LILYGO_T_DISPLAY_S3
    DisplayBrightness = preferences.getUInt("DisplayBright", 8);
#else
    DisplayBrightness = preferences.getUInt("DisplayBright", 100);
#endif
    neopixelBrightness = preferences.getUInt("neopixBright", 50);
    selectedNeopixelType = preferences.getUInt("selNeopxType", NEO_GRB + NEO_KHZ800);
    activeBLE = preferences.getBool("activeBLE", true);
    activeWIFI = preferences.getBool("activeWIFI", true);
    activeMQTT = preferences.getBool("activeMQTT", false);
    activeESPNOW = preferences.getBool("activeESPNOW", false);
    activeOTA = preferences.getBool("activeOTA", false);
    rootTopic = preferences.getString("rootTopic", rootTopic);
    mqttClientId = preferences.getString("mqttClientId", mqttClientId);
    mqttBroker = preferences.getString("mqttBroker", mqttBroker).c_str();
    mqttUser = preferences.getString("mqttUser", mqttUser).c_str();
    mqttPass = preferences.getString("mqttPass", mqttPass).c_str();
    if (!activeWIFI) {  // If not WiFi active disable MQTT and save
        activeMQTT = false;
        preferences.putBool("activeMQTT", activeMQTT);
    }
    batteryDischargedMillivolts = preferences.getUInt("batDischgd", 3200);
    batteryFullyChargedMillivolts = preferences.getUInt("batChargd", 4200);
    vRef = preferences.getUInt("vRef", 930);  // Looks like, due to a bug, 930 is a goos starting number for vRef
    timeToDisplayOff = preferences.getUInt("tToDispOff", 60);
    timeBetweenMQTTPublish = preferences.getUInt("tToPubMQTT", 60);
    timeBetweenESPNowPublish = preferences.getUInt("tToPubESPNow", 60);
    timeToKeepAliveMQTT = preferences.getUInt("tKeepAlMQTT", 300);
    timeToKeepAliveESPNow = preferences.getUInt("tKeepAlESPNow", 300);

    displayOffOnExternalPower = preferences.getBool("dispOffOnExP", false);
    wifiSSID = preferences.getString("wifiSSID", wifiSSID).c_str();
    wifiPass = preferences.getString("wifiPass", wifiPass).c_str();
    hostName = preferences.getString("hostName", hostName).c_str();
    selectedCO2Sensor = preferences.getUInt("selCO2Sensor", 0);
    debugSensors = preferences.getBool("debugSensors", false);
    displayReverse = preferences.getBool("displayReverse", false);
    showFahrenheit = preferences.getBool("showFahrenheit", false);
    measurementInterval = preferences.getUInt("measInterval", 10);
    outputsModeRelay = preferences.getBool("outModeRelay", false);
    channelESPNow = preferences.getUInt("channelESPNow", ESPNOW_WIFI_CH);
    boardIdESPNow = preferences.getUInt("boardIdESPNow", 0);

    size_t key_size = preferences.getBytesLength("peerESPNow");
    uint8_t buffer[key_size];
    preferences.getBytes("peerESPNow", buffer, key_size);

    // Serial.println("-->[PREF] Current size of \"peerESPNow\": " + String(key_size));
    // if (key_size > 0) {
    //   Serial.print("    Data: ");
    //   uint8_t existingData[key_size];
    //   size_t count = preferences.getBytes("peerESPNow", existingData, key_size);
    //   Serial.print("-->[PREF] [");
    //   Serial.print(count);
    //   Serial.print("]: ");
    //   for (size_t i = 0; i < count; i++) {
    //     Serial.print(existingData[i], HEX);
    //     Serial.print(" ");
    //   }
    //   Serial.println();
    // }
    // Serial.print("key_size = ");
    // Serial.print(key_size);
    // Serial.print(" sizeof(peerESPNowAddress) = ");
    // Serial.println(sizeof(peerESPNowAddress));

    if (key_size = sizeof(peerESPNowAddress)) {
        memcpy(peerESPNowAddress, buffer, sizeof(peerESPNowAddress));
    }

    displayShowTemperature = preferences.getBool("showTemp", true);
    displayShowHumidity = preferences.getBool("showHumidity", true);
    displayShowBattery = preferences.getBool("showBattery", true);
    displayShowBatteryVoltage = preferences.getBool("showBattVolt", false);
    displayShowCO2 = preferences.getBool("showCO2", true);
    displayShowPM25 = preferences.getBool("showPM25", true);

    // Retrieve buzzer preferences
    toneBuzzerBeep = preferences.getUInt("toneBzrBeep", BUZZER_TONE_MED);          // Frequency of the buzzer beep
    durationBuzzerBeep = preferences.getUInt("durBzrBeep", DURATION_BEEP_MEDIUM);  // Duration of the buzzer beep
    timeBetweenBuzzerBeeps = preferences.getUInt("timeBtwnBzr", 65535);            // Time between consecutive beeps

    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    preferences.end();
// #define DEBUG_PREFERENCES
#ifdef DEBUG_PREFERENCES
    printPreferences();
#endif
    upgradePreferences();
}

void saveWifiCredentials() {
    Serial.println("-->[PREF] Saving WiFi credentials to NVR");
    preferences.begin("CO2-Gadget", false);
    preferences.putString("wifiSSID", wifiSSID);
    preferences.putString("wifiPass", wifiPass);
    preferences.end();
}

void putPreferences() {
    Serial.println("-->[PREF] \n Saving preferences to NVR");
    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    // preferences.end();
    preferences.begin("CO2-Gadget", false);
    preferences.putUInt("customCalValue", customCalibrationValue);
    preferences.putFloat("tempOffset", tempOffset);
    preferences.putUInt("altitudeMeters", altitudeMeters);
    preferences.putBool("autoSelfCal", autoSelfCalibration);
    preferences.putUInt("co2OrangeRange", co2OrangeRange);
    preferences.putUInt("co2RedRange", co2RedRange);
    preferences.putUInt("DisplayBright", DisplayBrightness);
    preferences.putUInt("neopixBright", neopixelBrightness);
    preferences.putUInt("selNeopxType", selectedNeopixelType);
    preferences.putBool("activeBLE", activeBLE);
    preferences.putBool("activeWIFI", activeWIFI);
    preferences.putBool("activeMQTT", activeMQTT);
    preferences.putBool("activeESPNOW", activeESPNOW);
    preferences.putBool("activeOTA", activeOTA);
    preferences.putString("rootTopic", rootTopic);
    preferences.putUInt("batDischgd", batteryDischargedMillivolts);
    preferences.putUInt("batChargd", batteryFullyChargedMillivolts);
    preferences.putUInt("vRef", vRef);
    preferences.putString("mqttClientId", mqttClientId);
    preferences.putString("mqttBroker", mqttBroker);
    preferences.putString("mqttUser", mqttUser);
    preferences.putString("mqttPass", mqttPass);
    preferences.putUInt("tToDispOff", timeToDisplayOff);
    preferences.putUInt("tKeepAlMQTT", timeToKeepAliveMQTT);
    preferences.putUInt("tKeepAlESPNow", timeToKeepAliveESPNow);
    preferences.putUInt("tToPubMQTT", timeBetweenMQTTPublish);
    preferences.putUInt("tToPubESPNow", timeBetweenESPNowPublish);
    preferences.putBool("dispOffOnExP", displayOffOnExternalPower);
    preferences.putString("wifiSSID", wifiSSID);
    preferences.putString("wifiPass", wifiPass);
    preferences.putString("hostName", hostName);
    preferences.putUInt("selCO2Sensor", selectedCO2Sensor);
    preferences.putBool("debugSensors", debugSensors);
    preferences.putBool("displayReverse", displayReverse);
    preferences.putBool("showFahrenheit", showFahrenheit);
    preferences.putUInt("measInterval", measurementInterval);
    preferences.putBool("outModeRelay", outputsModeRelay);
    preferences.putUInt("channelESPNow", channelESPNow);
    preferences.putUInt("boardIdESPNow", boardIdESPNow);
    preferences.putBytes("peerESPNow", peerESPNowAddress, 6);

    preferences.putBool("showTemp", displayShowTemperature);
    preferences.putBool("showHumidity", displayShowHumidity);
    preferences.putBool("showBattery", displayShowBattery);
    preferences.putBool("showBattVolt", displayShowBatteryVoltage);
    preferences.putBool("showCO2", displayShowCO2);
    preferences.putBool("showPM25", displayShowPM25);

    // Buzzer preferences
    preferences.putUInt("toneBzrBeep", toneBuzzerBeep);          // Buzzer frequency
    preferences.putUInt("durBzrBeep", durationBuzzerBeep);       // Buzzer duration
    preferences.putUInt("timeBtwnBzr", timeBetweenBuzzerBeeps);  // Time between beeps

    preferences.end();

#ifdef DEBUG_PREFERENCES
    printPreferences();
#endif
}

String getPreferencesAsJson() {
    preferences.begin("CO2-Gadget", false);

    DynamicJsonDocument doc(1024);

    doc["customCalValue"] = preferences.getInt("customCalValue", 415);
    doc["tempOffset"] = preferences.getFloat("tempOffset", 0);
    doc["altitudeMeters"] = preferences.getInt("altitudeMeters", 0);
    doc["autoSelfCal"] = preferences.getBool("autoSelfCal", false);
    doc["co2OrangeRange"] = preferences.getInt("co2OrangeRange", 700);
    doc["co2RedRange"] = preferences.getInt("co2RedRange", 1000);
    doc["DisplayBright"] = preferences.getInt("DisplayBright", 100);
    doc["neopixBright"] = preferences.getInt("neopixBright", 50);
    doc["selNeopxType"] = preferences.getInt("selNeopxType", NEO_GRB + NEO_KHZ800);
    doc["activeBLE"] = preferences.getBool("activeBLE", true);
    doc["activeWIFI"] = preferences.getBool("activeWIFI", false);
    doc["activeMQTT"] = preferences.getBool("activeMQTT", false);
    doc["activeESPNOW"] = preferences.getBool("activeESPNOW", false);
    doc["activeOTA"] = preferences.getBool("activeOTA", false);
    doc["rootTopic"] = preferences.getString("rootTopic", rootTopic);
    doc["batDischgd"] = preferences.getInt("batDischgd", 3200);
    doc["batChargd"] = preferences.getInt("batChargd", 4200);
    doc["vRef"] = preferences.getInt("vRef", 930);
    doc["mqttClientId"] = preferences.getString("mqttClientId", mqttClientId);
    doc["mqttBroker"] = preferences.getString("mqttBroker", mqttBroker);
    doc["mqttUser"] = preferences.getString("mqttUser", mqttUser);
    // doc["mqttPass"] = preferences.getString("mqttPass", mqttPass);
    doc["tToDispOff"] = preferences.getInt("tToDispOff", 60);
    doc["tKeepAlMQTT"] = preferences.getInt("tKeepAlMQTT", 300);
    doc["tKeepAlESPNow"] = preferences.getInt("tKeepAlESPNow", 300);
    doc["tToPubMQTT"] = preferences.getInt("tToPubMQTT", 60);
    doc["tToPubESPNow"] = preferences.getInt("tToPubESPNow", 60);
    doc["dispOffOnExP"] = preferences.getBool("dispOffOnExP", false);
    doc["wifiSSID"] = preferences.getString("wifiSSID", wifiSSID);
    // doc["wifiPass"] = preferences.getString("wifiPass", wifiPass);
    doc["hostName"] = preferences.getString("hostName", hostName);
    doc["selCO2Sensor"] = preferences.getInt("selCO2Sensor", 0);
    doc["debugSensors"] = preferences.getBool("debugSensors", false);
    doc["displayReverse"] = preferences.getBool("displayReverse", false);
    doc["showFahrenheit"] = preferences.getBool("showFahrenheit", false);
    doc["measurementInterval"] = preferences.getInt("measInterval", 10);
    doc["outModeRelay"] = preferences.getBool("outModeRelay", false);
    doc["channelESPNow"] = preferences.getInt("channelESPNow", ESPNOW_WIFI_CH);
    doc["boardIdESPNow"] = preferences.getInt("boardIdESPNow", 0);
    doc["peerESPNowAddress"] = preferences.getString("peerESPNow", "00:00:00:00:00:00");
    doc["showTemp"] = preferences.getBool("showTemp", true);
    doc["showHumidity"] = preferences.getBool("showHumidity", true);
    doc["showBattery"] = preferences.getBool("showBattery", true);
    doc["showCO2"] = preferences.getBool("showCO2", true);
    doc["showPM25"] = preferences.getBool("showPM25", true);
    doc["measInterval"] = preferences.getInt("measInterval", 10);

    // Buzzer preferences
    doc["toneBzrBeep"] = preferences.getUInt("toneBzrBeep", 1000);   // Buzzer frequency
    doc["durBzrBeep"] = preferences.getUInt("durBzrBeep", 100);      // Buzzer duration
    doc["timeBtwnBzr"] = preferences.getUInt("timeBtwnBzr", 65535);  // Time between beeps

    preferences.end();

    String preferencesJson;
    serializeJson(doc, preferencesJson);
    // Serial.println("-->[PREF] Preferences JSON: " + preferencesJson);
    return preferencesJson;
}

String getActualSettingsAsJson() {
    JsonDocument doc;

    doc["customCalValue"] = customCalibrationValue;
    doc["tempOffset"] = tempOffset;
    doc["altitudeMeters"] = altitudeMeters;
    doc["autoSelfCal"] = autoSelfCalibration;
    doc["co2OrangeRange"] = co2OrangeRange;
    doc["co2RedRange"] = co2RedRange;
    doc["DisplayBright"] = DisplayBrightness;
    doc["neopixBright"] = neopixelBrightness;
    doc["selNeopxType"] = selectedNeopixelType;
    doc["activeBLE"] = activeBLE;
    doc["activeWIFI"] = activeWIFI;
    doc["activeMQTT"] = activeMQTT;
    doc["activeESPNOW"] = activeESPNOW;
    doc["activeOTA"] = activeOTA;
    doc["rootTopic"] = rootTopic;
    doc["batDischgd"] = batteryDischargedMillivolts;
    doc["batChargd"] = batteryFullyChargedMillivolts;
    doc["vRef"] = vRef;
    doc["mqttClientId"] = mqttClientId;
    doc["mqttBroker"] = mqttBroker;
    doc["mqttUser"] = mqttUser;
    // doc["mqttPass"] = mqttPass;
    doc["tToDispOff"] = timeToDisplayOff;
    doc["tKeepAlMQTT"] = timeToKeepAliveMQTT;
    doc["tKeepAlESPNow"] = timeToKeepAliveESPNow;
    doc["tToPubMQTT"] = timeBetweenMQTTPublish;
    doc["tToPubESPNow"] = timeBetweenESPNowPublish;
    doc["dispOffOnExP"] = displayOffOnExternalPower;
    doc["wifiSSID"] = wifiSSID;
    // doc["wifiPass"] = wifiPass;
    doc["hostName"] = hostName;
    doc["selCO2Sensor"] = selectedCO2Sensor;
    doc["debugSensors"] = debugSensors;
    doc["displayReverse"] = displayReverse;
    doc["showFahrenheit"] = showFahrenheit;
    doc["measurementInterval"] = measurementInterval;
    doc["outModeRelay"] = outputsModeRelay;
    doc["channelESPNow"] = channelESPNow;
    doc["boardIdESPNow"] = boardIdESPNow;

    // Convert peerESPNowAddress byte array to a string representation
    char peerAddressString[18];
    snprintf(peerAddressString, sizeof(peerAddressString),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
             peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
    // Add the string representation to the JSON document
    doc["peerESPNowAddress"] = peerAddressString;

    doc["showTemp"] = displayShowTemperature;
    doc["showHumidity"] = displayShowHumidity;
    doc["showBattery"] = displayShowBattery;
    doc["showBattVolt"] = displayShowBatteryVoltage;
    doc["showCO2"] = displayShowCO2;
    doc["showPM25"] = displayShowPM25;
    doc["measInterval"] = measurementInterval;

    // Buzzer preferences
    doc["toneBzrBeep"] = toneBuzzerBeep;          // Buzzer frequency
    doc["durBzrBeep"] = durationBuzzerBeep;       // Buzzer duration
    doc["timeBtwnBzr"] = timeBetweenBuzzerBeeps;  // Time between beeps

    String preferencesJson;
    serializeJson(doc, preferencesJson);
    // Serial.println("-->[PREF] Preferences JSON: " + preferencesJson);
    return preferencesJson;
}

bool handleSavePreferencesfromJSON(String jsonPreferences) {
    // Create a JSON object to store preferences
    JsonDocument JsonDocument;

    // Try to deserialize the JSON body from the request
    DeserializationError error = deserializeJson(JsonDocument, jsonPreferences);
    if (error) {
        // Handle the error when deserializing JSON
        Serial.print("Error deserializing JSON: ");
        Serial.println(error.c_str());
        // request->send(400, "text/plain", "Error in preferences format");
        return false;
    }

    // Save preferences to non-volatile memory (Preferences)
    try {
        preferences.begin("CO2-Gadget", false);
        customCalibrationValue = JsonDocument["customCalValue"];
        if (tempOffset != float(JsonDocument["tempOffset"])) {
            tempOffset = float(JsonDocument["tempOffset"]);
            sensors.setTempOffset(tempOffset);
        }
        altitudeMeters = JsonDocument["altitudeMeters"];
        autoSelfCalibration = JsonDocument["autoSelfCal"];
        co2OrangeRange = JsonDocument["co2OrangeRange"];
        co2RedRange = JsonDocument["co2RedRange"];
        if (DisplayBrightness != JsonDocument["DisplayBright"]) {
            DisplayBrightness = JsonDocument["DisplayBright"];
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
            // setDisplayBrightness(DisplayBrightness);
#endif
        }
        neopixelBrightness = JsonDocument["neopixBright"];
        selectedNeopixelType = JsonDocument["selNeopxType"];
        activeBLE = JsonDocument["activeBLE"];
        activeWIFI = JsonDocument["activeWIFI"];
        activeMQTT = JsonDocument["activeMQTT"];
        activeESPNOW = JsonDocument["activeESPNOW"];
        activeOTA = JsonDocument["activeOTA"];
        rootTopic = JsonDocument["rootTopic"].as<String>().c_str();
        batteryDischargedMillivolts = JsonDocument["batDischgd"];
        batteryFullyChargedMillivolts = JsonDocument["batChargd"];
        if (vRef != JsonDocument["vRef"]) {  // If battery reference changed, apply it
            vRef = JsonDocument["vRef"];
            battery.begin(vRef, voltageDividerRatio, &asigmoidal);
            readBatteryVoltage();
        }
        vRef = JsonDocument["vRef"];
        mqttClientId = JsonDocument["mqttClientId"].as<String>().c_str();
        mqttBroker = JsonDocument["mqttBroker"].as<String>().c_str();
        mqttUser = JsonDocument["mqttUser"].as<String>().c_str();
        timeToDisplayOff = JsonDocument["tToDispOff"];
        timeToKeepAliveMQTT = JsonDocument["tKeepAlMQTT"];
        timeToKeepAliveESPNow = JsonDocument["tKeepAlESPNow"];
        timeBetweenMQTTPublish = JsonDocument["tToPubMQTT"];
        timeBetweenESPNowPublish = JsonDocument["tToPubESPNow"];
        displayOffOnExternalPower = JsonDocument["dispOffOnExP"];
        wifiSSID = JsonDocument["wifiSSID"].as<String>().c_str();
        hostName = JsonDocument["hostName"].as<String>().c_str();
        selectedCO2Sensor = JsonDocument["selCO2Sensor"];
        debugSensors = JsonDocument["debugSensors"];
        displayReverse = JsonDocument["displayReverse"];
        showFahrenheit = JsonDocument["showFahrenheit"];
        measurementInterval = JsonDocument["measurementInterval"];
        outputsModeRelay = JsonDocument["outModeRelay"];
        channelESPNow = JsonDocument["channelESPNow"];
        boardIdESPNow = JsonDocument["boardIdESPNow"];

        // Get the MAC address for peerESPNowAddress as a string from JSON
        String peerESPNowAddressStr = JsonDocument["peerESPNowAddress"].as<String>();
        // Convert the string to an array of uint8_t
        uint8_t peerESPNowAddress[6];
        const char* peerESPNowAddressChar = peerESPNowAddressStr.c_str();
        for (int i = 0; i < 6; i++) {
            peerESPNowAddress[i] = strtoul(peerESPNowAddressChar + i * 3, NULL, 16);
        }

        displayShowTemperature = JsonDocument["showTemp"];
        displayShowHumidity = JsonDocument["showHumidity"];
        displayShowBattery = JsonDocument["showBattery"];
        displayShowBatteryVoltage = JsonDocument["showBattVolt"];
        displayShowCO2 = JsonDocument["showCO2"];
        displayShowPM25 = JsonDocument["showPM25"];

        // Buzzer preferences
        toneBuzzerBeep = JsonDocument["toneBzrBeep"];          // Buzzer frequency
        durationBuzzerBeep = JsonDocument["durBzrBeep"];       // Buzzer duration
        timeBetweenBuzzerBeeps = JsonDocument["timeBtwnBzr"];  // Time between beeps

        // mqttPass = JsonDocument["mqttPass"].as<String>().c_str();
        // wifiPass = JsonDocument["wifiPass"].as<String>().c_str();
        preferences.end();
    } catch (const std::exception& e) {
        // Manage error while storing preferences
        Serial.print("Error storing preferences: ");
        Serial.println(e.what());
        // request->send(500, "text/plain", "Internal Server Error Storing Preferences");
        return false;
    }

    // Send a successful response
    // request->send(200, "text/plain", "Preferences saved successfully");
    // Serial.println("-->[PREF] Preferences saved successfully @ handleSavePreferencesfromJSON()");
    putPreferences();
    return true;
}

#endif  // CO2_Gadget_Preferences_h