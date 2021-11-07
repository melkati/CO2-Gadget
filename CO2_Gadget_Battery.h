
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP BATTERY FUNCTIONALITY                           *********/
/*********  Uses Library https://github.com/rlogiacco/BatterySense/blob/master/Battery.cpp   *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

	// Battery info (percent charge).
	const uint16_t voltageDividerR1Ohms = 100000; // Resistance of "R1" for voltage divider.
	const uint16_t voltageDividerR2Ohms = 100000; // Resistance of "R2" for voltage divider.
	const uint16_t batteryDischargedMillivolts = 3600; // Voltage of battery when we consider it discharged (0%).
	const uint16_t batteryFullyChargedMillivolts = 4200; // Voltage of battery when it is considered fully charged (100%).

  // Voltage divider ratio for battery sense must be (R1 + R2) / R2 - see https://github.com/rlogiacco/BatterySense.
  const float voltageDividerRatio = (voltageDividerR1Ohms + voltageDividerR2Ohms) / voltageDividerR2Ohms;

#include <Battery.h>

float battery_voltage = 0;

Battery battery(batteryDischargedMillivolts, batteryFullyChargedMillivolts, ADC_PIN);
/**
 * 1 cell li-ion/li-poly battery wired to A0, continuous sensing, sigmoidal mapping function, cut off at 3000mV
 * https://github.com/rlogiacco/BatterySense#lesser-than-5v-with-voltage-booster
 **/
void initBattery() {
  battery.onDemand(battery.ON_DEMAND_DISABLE, LOW);
	battery.begin(vref, voltageDividerRatio, &sigmoidal);
}

void readBatteryVoltage() {
  static uint16_t batterySecondsBetweenReads = 1;
  static uint64_t batteryTimeStamp = (-1 * (batterySecondsBetweenReads*1000));
  if (millis() > batteryTimeStamp + (batterySecondsBetweenReads*1000)) { 
    battery_voltage = (float)battery.voltage() / 1000;
    Serial.print("battery.voltage() (mV): ");
    Serial.println(battery.voltage());
    Serial.print("Battery vref (mV): ");
    Serial.println(vref);
    Serial.printf("battery_voltage: %.2f\n", battery_voltage);
  }
}

// void readBatteryVoltage() {
//   static uint64_t batteryTimeStamp = -60000;
//   if (millis() >
//       batteryTimeStamp + 60000) { // Read battery voltage one time each minute
//     uint16_t v = analogRead(ADC_PIN);
//     battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
//     Serial.printf("Battery voltage: %.2f\n", battery_voltage);
//   }
// }