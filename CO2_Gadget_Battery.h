
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP BATTERY FUNCTIONALITY                           *********/
/*********  Uses Library https://github.com/rlogiacco/BatterySense/blob/master/Battery.cpp   *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

	// Battery info (percent charge).
	const uint32_t voltageDividerR1Ohms = 100000; // Resistance of "R1" for voltage divider.
	const uint32_t voltageDividerR2Ohms = 100000; // Resistance of "R2" for voltage divider.	

  // Voltage divider ratio for battery sense must be (R1 + R2) / R2 - see https://github.com/rlogiacco/BatterySense.
  const float voltageDividerRatio = (voltageDividerR1Ohms + voltageDividerR2Ohms) / voltageDividerR2Ohms;

#include <Battery.h>

Battery battery(batteryDischargedMillivolts, batteryFullyChargedMillivolts, ADC_PIN);
/**
 * 1 cell li-ion/li-poly battery wired to A0, continuous sensing, sigmoidal mapping function, cut off at 3000mV
 * https://github.com/rlogiacco/BatterySense#lesser-than-5v-with-voltage-booster
 **/
void initBattery() {
  battery.onDemand(battery.ON_DEMAND_DISABLE, LOW);
	battery.begin(vRef, voltageDividerRatio, &sigmoidal);
}

float readBatteryVoltage() {
  static uint16_t batterySecondsBetweenReads = 60;
  static uint64_t batteryTimeStamp = (-1 * (batterySecondsBetweenReads*1000));
  if (millis() > batteryTimeStamp + (batterySecondsBetweenReads*1000)) { 
    battery_voltage = (float)battery.voltage() / 1000;    
  }
  return(battery_voltage);
}

uint8_t getBatteryPercentage() {
  Serial.printf("Battery Level: %d%%\n", battery.level());
  return battery.level();
}