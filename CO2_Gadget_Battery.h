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
        batteryVoltage = avrgBatteryVoltage;
        // Serial.print("-->[BATT] Battery read: ");
        // Serial.print(batteryVoltage);
        // Serial.println("V");
        lastTimeBatteryRead = millis();
    }
    return (batteryVoltage);
}

uint8_t getBatteryPercentage() {
    return battery.level();
}

const unsigned long BATTERY_READ_INTERVAL = 3000; // 3 seconds

void batteryLoop() {
    static float lastBatteryVoltage = readBatteryVoltage();
    static unsigned long lastBatteryReadTime = 0;
    
    if (millis() - lastBatteryReadTime >= BATTERY_READ_INTERVAL) {  // Check if at least 3 segundos have passed
        readBatteryVoltage();
        if (!inMenu) {
            if (abs(lastBatteryVoltage - batteryVoltage) >= 0.1) {  // If battery voltage changed by at least 0.1V, update battery level
                batteryLevel = getBatteryPercentage();
                // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage changed from: %.4fV to %.4fV\n", batteryLevel, lastBatteryVoltage, batteryVoltage);
                lastBatteryVoltage = batteryVoltage;
            }
        }
        // If battery voltage is more than 9% of the fully charged battery voltage (~4.58V), assume it's working on external power
        workingOnExternalPower = (batteryVoltage * 1000 > batteryFullyChargedMillivolts + (batteryFullyChargedMillivolts * 9 / 100));
        // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage: %.4fV. Working on external power: %s\n", batteryLevel, batteryVoltage, workingOnExternalPower ? "Yes" : "No");
        lastBatteryReadTime = millis();  // Update last read time
    }
}


#endif  // CO2_Gadget_Battery_h