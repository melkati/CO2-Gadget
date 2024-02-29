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
            batteryVoltageNow += float(battery.voltage()) / 1000;
            delay(10);
        }
        batteryVoltageNow /= 3;
        batteryVoltage = batteryVoltageNow;
        batteryLevel = battery.level(batteryVoltage * 1000);
        lastTimeBatteryRead = millis();

        // If battery voltage is more than 9% of the fully charged battery voltage (~4.58V) or if battery voltage is less than 1V (no battery connected to sense pin), then assume that the device is working on external power.
        workingOnExternalPower = (batteryVoltageNow * 1000 > batteryFullyChargedMillivolts + (batteryFullyChargedMillivolts * 9 / 100)) || (batteryVoltageNow < 1000);

        // publishMQTTLogData("Battery Level: " + String(batteryLevel) + "%   Battery voltage changed from: " + String(lastBatteryVoltage) + "V to " + String(batteryVoltage) + "V");
    }
}

// #include <esp_adc_cal.h>

// void readEfuse() {
//     esp_adc_cal_characteristics_t chars;
//     auto val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &chars);
//     if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
//         Serial.println("eFuse Vref");
//     } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
//         Serial.println("Two Point");
//     } else {
//         Serial.println("Default");
//     }
// }

void batteryLoop() {
    float batteryVoltageNow = 0;
    readBatteryVoltage();
    // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage: %.4fV\n", batteryLevel, batteryVoltageNow);
    if (!inMenu) {
        if (abs(lastBatteryVoltage - batteryVoltage) >= 0.1) {  // If battery voltage changed by at least 0.1V, update battery level
            // Serial.printf("-->[BATT] Battery Level: %d%%. Battery voltage changed from: %.4fV to %.4fV\n", batteryLevel, lastBatteryVoltage, batteryVoltage);
            lastBatteryVoltage = batteryVoltage;
        }
    }
}

#endif  // CO2_Gadget_Battery_h