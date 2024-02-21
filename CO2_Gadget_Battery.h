#ifndef CO2_Gadget_Battery_h
#define CO2_Gadget_Battery_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP BATTERY FUNCTIONALITY                           *********/
/*********               Uses Library https://github.com/rlogiacco/BatterySense              *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

uint16_t timeBetweenBatteryRead = 5;
uint64_t lastTimeBatteryRead = 0;  // Time of last battery reading
const uint8_t batterySamples = 3;  // Number of samples to average for battery voltage.

// Battery info (percent charge).
const uint32_t voltageDividerR1Ohms = 100000;  // Resistance of "R1" for voltage divider.
const uint32_t voltageDividerR2Ohms = 100000;  // Resistance of "R2" for voltage divider.

// Voltage divider ratio for battery sense must be (R1 + R2) / R2
const float voltageDividerRatio = (voltageDividerR1Ohms + voltageDividerR2Ohms) / voltageDividerR2Ohms;

#include <Battery.h>

Battery battery(batteryDischargedMillivolts, batteryFullyChargedMillivolts, ADC_BATTERY_PIN);
/**
 * 1 cell li-ion/li-poly battery wired to A0, continuous sensing, sigmoidal mapping function, cut off at 3000mV
 * https://github.com/rlogiacco/BatterySense#lesser-than-5v-with-voltage-booster
 **/
void initBattery() {
    battery.onDemand(battery.ON_DEMAND_DISABLE, LOW);
    battery.begin(vRef, voltageDividerRatio, &sigmoidal);
}

float readBatteryVoltage() {
    float avrgBatteryVoltage = 0;
    if ((millis() - lastTimeBatteryRead >= timeBetweenBatteryRead * 1000) || (lastTimeBatteryRead == 0)) {
        for (uint8_t i = 0; i < batterySamples; i++) {
            avrgBatteryVoltage += (float)battery.voltage() / 1000;
            delay(10);
        }
        avrgBatteryVoltage /= 3;
        battery_voltage = avrgBatteryVoltage;
        // Serial.print("-->[BATT] Battery read: ");
        // Serial.print(battery_voltage);
        // Serial.println("V");
        lastTimeBatteryRead = millis();
    }
    return (battery_voltage);
}

uint8_t getBatteryPercentage() {
    return battery.level();
}

void batteryLoop() {
    static float lastBatteryVoltage = readBatteryVoltage();
    readBatteryVoltage();
    if (!inMenu) {
        if (abs(lastBatteryVoltage - battery_voltage) >= 0.1) {  // If battery voltage changed by at least 0.1V, update battery level
            battery_level = getBatteryPercentage();
            // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage changed from: %.4fV to %.4fV\n", battery_level, lastBatteryVoltage, battery_voltage);
            lastBatteryVoltage = battery_voltage;
        }
    }
    // If battery voltage is more than 5% of the fully charged battery voltage, asume it's working on external power
    workingOnExternalPower = (battery_voltage * 1000 > batteryFullyChargedMillivolts + (batteryFullyChargedMillivolts * 5 / 100));
}

#endif  // CO2_Gadget_Battery_h