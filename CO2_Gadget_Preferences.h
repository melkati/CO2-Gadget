#include <Preferences.h>
Preferences preferences;

void printPreferences() {
  Serial.println("LOADED PREFERENCES FROM NVR:");
  Serial.printf("customCalibrationValue: %d\n", customCalibrationValue);
  Serial.printf("altidudeMeters:\t %d\n", altidudeMeters);
  Serial.printf("autoSelfCalibration:\t %s\n",
                ((autoSelfCalibration) ? "Enabled" : "Disabled"));
  Serial.printf("co2OrangeRange:\t %d\n", co2OrangeRange);
  Serial.printf("co2RedRange:\t %d\n", co2RedRange);
  Serial.printf("TFTBrightness:\t %d\n", TFTBrightness);
  Serial.printf("activeBLE is:\t%s\n", ((activeBLE) ? "Enabled" : "Disabled"));
  Serial.printf("activeWIFI:\t%s\n", ((activeWIFI) ? "Enabled" : "Disabled"));
  Serial.printf("activeMQTT:\t%s\n", ((activeMQTT) ? "Enabled" : "Disabled"));
  Serial.printf("rootTopic:\t%s\n", rootTopic.c_str());
  Serial.printf("batDischgd:\t %d\n", batteryDischargedMillivolts);
  Serial.printf("batChargd:\t %d\n", batteryFullyChargedMillivolts);
  Serial.printf("vRef:\t %d\n", vRef);
  Serial.printf("mqttClientId:\t%s\n", mqttClientId.c_str());
  Serial.printf("mqttBroker:\t%s\n", mqttBroker.c_str());
  Serial.printf("timeToDisplayOff:\t %d\n", timeToDisplayOff);
  Serial.printf("displayOffOnExternalPower:\t%s\n", ((displayOffOnExternalPower) ? "Enabled" : "Disabled"));
  Serial.printf("wifiSSID:\t%s\n", wifiSSID.c_str());
  Serial.printf("wifiPass:\t%s\n", wifiPass.c_str());
}

void initPreferences() {
  preferences.begin("CO2-Gadget", false);
  // preferences.clear(); // Remove all preferences
  customCalibrationValue = preferences.getUInt("customCalValue", 415);
  altidudeMeters = preferences.getUInt("altidudeMeters", 0);
  autoSelfCalibration = preferences.getBool("autoSelfCal", false);
  co2OrangeRange = preferences.getUInt("co2OrangeRange", 700);
  co2RedRange = preferences.getUInt("co2RedRange", 1000);
  TFTBrightness = preferences.getUInt("TFTBrightness", 100);
  activeBLE = preferences.getBool("activeBLE", true);
  activeWIFI = preferences.getBool("activeWIFI", false);
  activeMQTT = preferences.getBool("activeMQTT", false);
  rootTopic = preferences.getString("rootTopic", rootTopic);
  mqttClientId = preferences.getString("mqttClientId", mqttClientId);
  mqttBroker = preferences.getString("mqttBroker", mqttBroker).c_str();
  if (!activeWIFI) { // If not WiFi active disable MQTT and save
    activeMQTT = false;
    preferences.putBool("activeMQTT", activeMQTT);
  }
  batteryDischargedMillivolts = preferences.getUInt("batDischgd", 3500);
  batteryFullyChargedMillivolts = preferences.getUInt("batChargd", 4200);
  vRef = preferences.getUInt("vRef", 1100);
  timeToDisplayOff = preferences.getUInt("timeToDisplayOff", 0);
  displayOffOnExternalPower = preferences.getBool("displayOffOnExternalPower", false);
  wifiSSID = preferences.getString("wifiSSID", wifiSSID).c_str();
  wifiPass = preferences.getString("wifiPass", wifiPass).c_str();
  preferences.end();
  printPreferences();
}

void putPreferences() {
  preferences.begin("CO2-Gadget", false);
  preferences.putUInt("customCalValue", customCalibrationValue);
  preferences.putUInt("altidudeMeters", altidudeMeters);
  preferences.putBool("autoSelfCal", autoSelfCalibration);
  preferences.putUInt("co2OrangeRange", co2OrangeRange);
  preferences.putUInt("co2RedRange", co2RedRange);
  preferences.putUInt("TFTBrightness", TFTBrightness);
  preferences.putBool("activeBLE", activeBLE);
  preferences.putBool("activeWIFI", activeWIFI);
  preferences.putBool("activeMQTT", activeMQTT);
  preferences.putString("rootTopic", rootTopic);
  preferences.putUInt("batDischgd", batteryDischargedMillivolts);
  preferences.putUInt("batChargd", batteryFullyChargedMillivolts);
  preferences.putUInt("vRef", vRef);
  preferences.putString("mqttClientId", mqttClientId);
  preferences.putString("mqttBroker", mqttBroker);
  preferences.putUInt("timeToDisplayOff", timeToDisplayOff);
  preferences.putBool("displayOffOnExternalPower", displayOffOnExternalPower);
  preferences.putString("wifiSSID", wifiSSID);
  preferences.putString("wifiPass", wifiPass);
  preferences.end();
}