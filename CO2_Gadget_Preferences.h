#include <Preferences.h>
Preferences preferences;

void initPreferences() {
  preferences.begin("CO2-Gadget", false);
  // preferences.clear(); // Remove all preferences
  customCalibrationValue = preferences.getUInt("customCalValue", 415);
  altidudeMeters = preferences.getUInt("altidudeMeters", 0);
  autoSelfCalibration = preferences.getBool("autoSelfCal", false);
  co2OrangeRange = preferences.getUInt("co2OrangeRange", 700);
  co2RedRange = preferences.getUInt("co2RedRange", 1000);
  TFTBrightness = preferences.getUInt("TFTBrightness", 100);
  preferences.end();
}

void putPreferences() {
  preferences.begin("CO2-Gadget", false);
  preferences.putUInt("customCalValue", customCalibrationValue);
  preferences.putUInt("altidudeMeters", altidudeMeters);
  preferences.putBool("autoSelfCal", autoSelfCalibration);
  preferences.putUInt("co2OrangeRange", co2OrangeRange);
  preferences.putUInt("co2RedRange", co2RedRange);
  preferences.putUInt("TFTBrightness", TFTBrightness);
  preferences.end();
}