#include <Preferences.h>
Preferences preferences;

void printPreferences() {
  Serial.println("");
  Serial.println("LOADED PREFERENCES FROM NVR:");
  Serial.printf("customCalibrationValue: #%d#\n", customCalibrationValue);
  Serial.printf("tempOffset:\t #%.1f#\n", tempOffset);
  Serial.printf("altidudeMeters:\t #%d#\n", altidudeMeters);
  Serial.printf("autoSelfCalibration:\t #%s#\n",
                ((autoSelfCalibration) ? "Enabled" : "Disabled"));
  Serial.printf("co2OrangeRange:\t #%d#\n", co2OrangeRange);
  Serial.printf("co2RedRange:\t #%d#\n", co2RedRange);
  Serial.printf("DisplayBrightness:\t #%d#\n", DisplayBrightness);
  Serial.printf("neopixBright:\t #%d#\n", neopixelBrightness);
  Serial.printf("selNeopxType:\t #%d#\n", selectedNeopixelType);
  Serial.printf("activeBLE is:\t#%s# (%d)\n", ((activeBLE) ? "Enabled" : "Disabled"), activeBLE);
  Serial.printf("activeWIFI is:\t#%s# (%d)\n", ((activeWIFI) ? "Enabled" : "Disabled"), activeWIFI);
  Serial.printf("activeMQTT is:\t#%s# (%d)\n", ((activeMQTT) ? "Enabled" : "Disabled"), activeMQTT);
  Serial.printf("activeESPNOW is:\t#%s# (%d)\n", ((activeMQTT) ? "Enabled" : "Disabled"), activeESPNOW);  
  Serial.printf("rootTopic:\t#%s#\n", rootTopic.c_str());
  Serial.printf("batDischgd:\t #%d#\n", batteryDischargedMillivolts);
  Serial.printf("batChargd:\t #%d#\n", batteryFullyChargedMillivolts);
  Serial.printf("vRef:\t #%d#\n", vRef);
  Serial.printf("mqttClientId:\t#%s#\n", mqttClientId.c_str());
  Serial.printf("mqttBroker:\t#%s#\n", mqttBroker.c_str());
  Serial.printf("mqttUser:\t#%s#\n", mqttUser.c_str());
  #ifndef WIFI_PRIVACY
  Serial.printf("mqttPass:\t#%s#\n", mqttPass.c_str());
  #endif
  Serial.printf("tToDispOff:\t #%d#\n", timeToDisplayOff);
  Serial.printf("tToPubMQTT:\t #%d#\n", timeBetweenMQTTPublish);
  Serial.printf("tToPubESPNow:\t #%d#\n", timeBetweenESPNowPublish);
  Serial.printf("tKeepAlMQTT:\t #%d#\n", timeToKeepAliveMQTT);
  Serial.printf("tKeepAlESPNow:\t #%d#\n", timeToKeepAliveESPNow);
  Serial.printf("dispOffOnExP:\t#%s# (%d)\n", ((displayOffOnExternalPower) ? "Enabled" : "Disabled"), displayOffOnExternalPower);
  Serial.printf("wifiSSID:\t#%s#\n", wifiSSID.c_str());
  #ifndef WIFI_PRIVACY
  Serial.printf("wifiPass:\t#%s#\n", wifiPass.c_str());
  #endif
  Serial.printf("hostName:\t#%s#\n", hostName.c_str());
  Serial.printf("selCO2Sensor:\t #%d#\n", selectedCO2Sensor);
  Serial.printf("debugSensors is:\t#%s# (%d)\n", ((debugSensors) ? "Enabled" : "Disabled"), debugSensors);
  Serial.printf("displayReverse is:\t#%s# (%d)\n", ((displayReverse) ? "Reversed" : "Normal"), displayReverse);  
  Serial.printf("showFahrenheit is:\t#%s#\n", ((showFahrenheit) ? "Farenheit" : "Celsius"));
  Serial.printf("measInterval:\t #%d#\n", measurementInterval);
  Serial.printf("outModeRelay is:\t#%s#\n", ((outputsModeRelay) ? "Relay" : "RGB LED"));  
  Serial.printf("channelESPNow:\t #%d#\n", channelESPNow);
  
  Serial.printf("showTemp:\t #%s#\n",  ((displayShowTemperature) ? "Show" : "Hide"));
  Serial.printf("showHumidity:\t #%s#\n",  ((displayShowHumidity) ? "Show" : "Hide"));
  Serial.printf("showBattery:\t #%s#\n",  ((displayShowBattery) ? "Show" : "Hide"));
  Serial.printf("showCO2:\t #%s#\n",  ((displayShowCO2) ? "Show" : "Hide"));
  Serial.printf("showPM25:\t #%s#\n",  ((displayShowPM25) ? "Show" : "Hide"));
  Serial.println("");
}

void initPreferences() {
  preferences.begin("CO2-Gadget", false);
  // preferences.clear(); // Remove all preferences
  customCalibrationValue = preferences.getUInt("customCalValue", 415);
  tempOffset = float(preferences.getFloat("tempOffset", 0));
  altidudeMeters = preferences.getUInt("altidudeMeters", 0);
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
  if (!activeWIFI) { // If not WiFi active disable MQTT and save
    activeMQTT = false;
    preferences.putBool("activeMQTT", activeMQTT);
  }
  batteryDischargedMillivolts = preferences.getUInt("batDischgd", 3500);
  batteryFullyChargedMillivolts = preferences.getUInt("batChargd", 4200);
  vRef = preferences.getUInt("vRef", 930); // Looks like, due to a bug, 930 is a goos starting number for vRef
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
  Serial.println("");
  Serial.println("Saving preferences to NVR");
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
  preferences.putUInt("altidudeMeters", altidudeMeters);
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

  preferences.putBool("showTemp", displayShowTemperature);
  preferences.putBool("showHumidity", displayShowHumidity);
  preferences.putBool("showBattery", displayShowBattery);
  preferences.putBool("showCO2", displayShowCO2);
  preferences.putBool("showPM25", displayShowPM25);
  
  preferences.end();
}