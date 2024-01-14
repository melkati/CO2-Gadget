#ifndef CO2_Gadget_Preferences_h
#define CO2_Gadget_Preferences_h

#include <ArduinoJson.h>
#include <Preferences.h>
Preferences preferences;

void printPreferences() {
    Serial.printf("-->[PREF] \n");
    Serial.printf("-->[PREF] LOADED PREFERENCES FROM NVR:\n");
    Serial.printf("-->[PREF] customCalValue: #%d#\n", customCalibrationValue);
    Serial.printf("-->[PREF] tempOffset:\t #%.1f#\n", tempOffset);
    Serial.printf("-->[PREF] altitudeMeters:\t #%d#\n", altitudeMeters);
    Serial.printf("-->[PREF] autoSelfCalibration:\t #%s#\n",
                  ((autoSelfCalibration) ? "Enabled" : "Disabled"));
    Serial.printf("-->[PREF] co2OrangeRange:\t #%d#\n", co2OrangeRange);
    Serial.printf("-->[PREF] co2RedRange:\t #%d#\n", co2RedRange);
    Serial.printf("-->[PREF] DisplayBrightness:\t #%d#\n", DisplayBrightness);
    Serial.printf("-->[PREF] neopixBright:\t #%d#\n", neopixelBrightness);
    Serial.printf("-->[PREF] selNeopxType:\t #%d#\n", selectedNeopixelType);
    Serial.printf("-->[PREF] activeBLE is:\t#%s# (%d)\n", ((activeBLE) ? "Enabled" : "Disabled"), activeBLE);
    Serial.printf("-->[PREF] activeWIFI is:\t#%s# (%d)\n", ((activeWIFI) ? "Enabled" : "Disabled"), activeWIFI);
    Serial.printf("-->[PREF] activeMQTT is:\t#%s# (%d)\n", ((activeMQTT) ? "Enabled" : "Disabled"), activeMQTT);
    Serial.printf("-->[PREF] activeESPNOW is:\t#%s# (%d)\n", ((activeMQTT) ? "Enabled" : "Disabled"), activeESPNOW);
    Serial.printf("-->[PREF] rootTopic:\t#%s#\n", rootTopic.c_str());
    Serial.printf("-->[PREF] batDischgd:\t #%d#\n", batteryDischargedMillivolts);
    Serial.printf("-->[PREF] batChargd:\t #%d#\n", batteryFullyChargedMillivolts);
    Serial.printf("-->[PREF] vRef:\t #%d#\n", vRef);
    Serial.printf("-->[PREF] mqttClientId:\t#%s#\n", mqttClientId.c_str());
    Serial.printf("-->[PREF] mqttBroker:\t#%s#\n", mqttBroker.c_str());
    Serial.printf("-->[PREF] mqttUser:\t#%s#\n", mqttUser.c_str());
#ifndef WIFI_PRIVACY
    Serial.printf("-->[PREF] mqttPass:\t#%s#\n", mqttPass.c_str());
#endif
    Serial.printf("-->[PREF] tToDispOff:\t #%d#\n", timeToDisplayOff);
    Serial.printf("-->[PREF] tToPubMQTT:\t #%d#\n", timeBetweenMQTTPublish);
    Serial.printf("-->[PREF] tToPubESPNow:\t #%d#\n", timeBetweenESPNowPublish);
    Serial.printf("-->[PREF] tKeepAlMQTT:\t #%d#\n", timeToKeepAliveMQTT);
    Serial.printf("-->[PREF] tKeepAlESPNow:\t #%d#\n", timeToKeepAliveESPNow);
    Serial.printf("-->[PREF] dispOffOnExP:\t#%s# (%d)\n", ((displayOffOnExternalPower) ? "Enabled" : "Disabled"), displayOffOnExternalPower);
    Serial.printf("-->[PREF] wifiSSID:\t#%s#\n", wifiSSID.c_str());
#ifndef WIFI_PRIVACY
    Serial.printf("-->[PREF] wifiPass:\t#%s#\n", wifiPass.c_str());
#endif
    Serial.printf("-->[PREF] hostName:\t#%s#\n", hostName.c_str());
    Serial.printf("-->[PREF] selCO2Sensor:\t #%d#\n", selectedCO2Sensor);
    Serial.printf("-->[PREF] debugSensors is:\t#%s# (%d)\n", ((debugSensors) ? "Enabled" : "Disabled"), debugSensors);
    Serial.printf("-->[PREF] displayReverse is:\t#%s# (%d)\n", ((displayReverse) ? "Reversed" : "Normal"), displayReverse);
    Serial.printf("-->[PREF] showFahrenheit is:\t#%s#\n", ((showFahrenheit) ? "Farenheit" : "Celsius"));
    Serial.printf("-->[PREF] measInterval:\t #%d#\n", measurementInterval);
    Serial.printf("-->[PREF] outModeRelay is:\t#%s#\n", ((outputsModeRelay) ? "Relay" : "RGB LED"));
    Serial.printf("-->[PREF] channelESPNow:\t #%d#\n", channelESPNow);
    Serial.printf("-->[PREF] boardIdESPNow:\t #%d#\n", boardIdESPNow);
    Serial.printf("-->[PREF] peerESPNow:\t #%02X:%02X:%02X:%02X:%02X:%02X#\n", peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2], peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);

    Serial.printf("-->[PREF] showTemp:\t #%s#\n", ((displayShowTemperature) ? "Show" : "Hide"));
    Serial.printf("-->[PREF] showHumidity:\t #%s#\n", ((displayShowHumidity) ? "Show" : "Hide"));
    Serial.printf("-->[PREF] showBattery:\t #%s#\n", ((displayShowBattery) ? "Show" : "Hide"));
    Serial.printf("-->[PREF] showCO2:\t #%s#\n", ((displayShowCO2) ? "Show" : "Hide"));
    Serial.printf("-->[PREF] showPM25:\t #%s#\n", ((displayShowPM25) ? "Show" : "Hide"));
    Serial.printf("-->[PREF] \n");
}

void initPreferences() {
    preferences.begin("CO2-Gadget", false);
    // preferences.clear(); // Remove all preferences
    customCalibrationValue = preferences.getUInt("customCalValue", 415);
    tempOffset = float(preferences.getFloat("tempOffset", 0));
    altitudeMeters = preferences.getUInt("altitudeMeters", 0);
    autoSelfCalibration = preferences.getBool("autoSelfCal", false);
    co2OrangeRange = preferences.getUInt("co2OrangeRange", 700);
    co2RedRange = preferences.getUInt("co2RedRange", 1000);
    DisplayBrightness = preferences.getUInt("DisplayBright", 100);
    neopixelBrightness = preferences.getUInt("neopixBright", 50);
    selectedNeopixelType = preferences.getUInt("selNeopxType", NEO_GRB + NEO_KHZ800);
    activeBLE = preferences.getBool("activeBLE", true);
    activeWIFI = preferences.getBool("activeWIFI", false);
    activeMQTT = preferences.getBool("activeMQTT", false);
    activeESPNOW = preferences.getBool("activeESPNOW", false);
    rootTopic = preferences.getString("rootTopic", rootTopic);
    mqttClientId = preferences.getString("mqttClientId", mqttClientId);
    mqttBroker = preferences.getString("mqttBroker", mqttBroker).c_str();
    mqttUser = preferences.getString("mqttUser", mqttUser).c_str();
    mqttPass = preferences.getString("mqttPass", mqttPass).c_str();
    if (!activeWIFI) {  // If not WiFi active disable MQTT and save
        activeMQTT = false;
        preferences.putBool("activeMQTT", activeMQTT);
    }
    batteryDischargedMillivolts = preferences.getUInt("batDischgd", 3500);
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

    // Serial.printf("-->[PREF] Current size of \"%s\": %d\n", "peerESPNow", key_size);
    // if (key_size > 0) {
    //   Serial.print("    Data");
    //   uint8_t existingData[key_size];
    //   size_t count = preferences.getBytes("peerESPNow", existingData, key_size);
    //   Serial.printf("-->[PREF] [%d]:", count);
    //   for (size_t i = 0; i < count; i++) {
    //     Serial.printf("-->[PREF]  %02x", existingData[i]);
    //   }
    //   Serial.printf("\n");
    // }
    // Serial.printf("key_size = %d sizeof(peerESPNowAddress) = %d\n", key_size, sizeof(peerESPNowAddress));

    if (key_size = sizeof(peerESPNowAddress)) {
        memcpy(peerESPNowAddress, buffer, sizeof(peerESPNowAddress));
    }

    displayShowTemperature = preferences.getBool("showTemp", true);
    displayShowHumidity = preferences.getBool("showHumidity", true);
    displayShowBattery = preferences.getBool("showBattery", true);
    displayShowCO2 = preferences.getBool("showCO2", true);
    displayShowPM25 = preferences.getBool("showPM25", true);

    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    preferences.end();
    printPreferences();
}

void putPreferences() {
    Serial.printf("-->[PREF] \n");
    Serial.printf("-->[PREF] Saving preferences to NVR\n");
    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    preferences.end();
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
    preferences.putBool("showCO2", displayShowCO2);
    preferences.putBool("showPM25", displayShowPM25);

    preferences.end();
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
    doc["rootTopic"] = preferences.getString("rootTopic", rootTopic);
    doc["batDischgd"] = preferences.getInt("batDischgd", 3500);
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
    preferences.end();

    String preferencesJson;
    serializeJson(doc, preferencesJson);
    // Serial.printf("-->[PREF] Preferences JSON: %s\n", preferencesJson.c_str());
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
    doc["showCO2"] = displayShowCO2;
    doc["showPM25"] = displayShowPM25;
    doc["measInterval"] = measurementInterval;

    String preferencesJson;
    serializeJson(doc, preferencesJson);
    // Serial.printf("-->[PREF] Preferences JSON: %s\n", preferencesJson.c_str());
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
        DisplayBrightness = JsonDocument["DisplayBright"];
        neopixelBrightness = JsonDocument["neopixBright"];
        selectedNeopixelType = JsonDocument["selNeopxType"];
        activeBLE = JsonDocument["activeBLE"];
        activeWIFI = JsonDocument["activeWIFI"];
        activeMQTT = JsonDocument["activeMQTT"];
        activeESPNOW = JsonDocument["activeESPNOW"];
        rootTopic = JsonDocument["rootTopic"].as<String>().c_str();
        batteryDischargedMillivolts = JsonDocument["batDischgd"];
        batteryFullyChargedMillivolts = JsonDocument["batChargd"];
        if (vRef != JsonDocument["vRef"]) {
            vRef = JsonDocument["vRef"];
            battery.begin(vRef, voltageDividerRatio, &sigmoidal);
            delay(10);
            battery_voltage = (float)battery.voltage() / 1000;
        }
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
        displayShowCO2 = JsonDocument["showCO2"];
        displayShowPM25 = JsonDocument["showPM25"];

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
    Serial.println("-->[PREF] Preferences saved successfully @ handleSavePreferencesfromJSON()");
    putPreferences();
    return true;
}

#endif  // CO2_Gadget_Preferences_h