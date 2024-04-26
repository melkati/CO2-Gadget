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

float lastBatteryVoltage = 0;
uint16_t timeBetweenBatteryRead = 1;
uint64_t lastTimeBatteryRead = 0;  // Time of last battery reading
const uint8_t batterySamples = 3;  // Number of samples to average for battery voltage.

// Battery info (percent charge).
const uint32_t voltageDividerR1Ohms = 100000;  // Resistance of "R1" for voltage divider.
const uint32_t voltageDividerR2Ohms = 100000;  // Resistance of "R2" for voltage divider.

// Voltage divider ratio for battery sense must be (R1 + R2) / R2
const float voltageDividerRatio = (voltageDividerR1Ohms + voltageDividerR2Ohms) / voltageDividerR2Ohms;

#include <Battery.h>

Battery battery(batteryDischargedMillivolts, batteryFullyChargedMillivolts, ADC_BATTERY_PIN);

void initBattery() {
    battery.onDemand(battery.ON_DEMAND_DISABLE, LOW);
    battery.begin(vRef, voltageDividerRatio, &asigmoidal);
    // Serial.println("-->[BATT***] Battery initialized with vRef: " + String(vRef) + " and voltage divider ratio: " + String(voltageDividerRatio));
}

void readBatteryVoltage() {
    float batteryVoltageNow = 0;

    if ((millis() - lastTimeBatteryRead >= timeBetweenBatteryRead * 1000) || (lastTimeBatteryRead == 0)) {
        for (uint8_t i = 0; i < batterySamples; i++) {
            batteryVoltageNow += float(battery.voltage(20)) / 1000;
            delay(5);
        }
        batteryVoltageNow /= 3;
        batteryVoltage = batteryVoltageNow;
        batteryLevel = battery.level(batteryVoltage * 1000);
        lastTimeBatteryRead = millis();

        // If battery voltage is more than 6% of the fully charged battery voltage (~4.45V) or if battery voltage is less
        // than 1V (no battery connected to sense pin), then assume that the device is working on external power.
        const float workingOnExternalPowerThreshold = batteryFullyChargedMillivolts * 1.06 / 1000;
        workingOnExternalPower = ((batteryVoltageNow) > workingOnExternalPowerThreshold) || (batteryVoltageNow < 1);

        // workingOnExternalPower = (batteryVoltageNow * 1000 > batteryFullyChargedMillivolts + (batteryFullyChargedMillivolts * 6 / 100)) || (batteryVoltageNow < 1);
        // publishMQTTLogData("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltageNow) + "V  External power: " + String(workingOnExternalPower));
        // if (!inMenu) {
        //     Serial.println("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V  External power: " + String(workingOnExternalPower) + " workingOnExternalPowerThreshold: " + String(workingOnExternalPowerThreshold));
        //     delay(20);
        // }
        //     publishMQTTLogData("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltageNow) + "V  External power: " + String(workingOnExternalPower) + " workingOnExternalPowerThreshold: " + String(workingOnExternalPowerThreshold));
    }
}

void batteryLoop() {
    readBatteryVoltage();
    if (!inMenu) {
        if (abs(lastBatteryVoltage - batteryVoltage) >= 0.1) {  // If battery voltage changed by at least 0.1V, update battery level
            // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage changed from: %.4fV to %.4fV\n", batteryLevel, lastBatteryVoltage, batteryVoltage);
            lastBatteryVoltage = batteryVoltage;
        }
    }
}

#endif  // CO2_Gadget_Battery_h