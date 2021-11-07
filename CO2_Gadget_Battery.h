
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP BATTERY FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

float battery_voltage = 0;

void readBatteryVoltage() {
  static uint64_t batteryTimeStamp = -60000;
  if (millis() >
      batteryTimeStamp + 60000) { // Read battery voltage one time each minute
    uint16_t v = analogRead(ADC_PIN);
    battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    Serial.printf("Battery voltage: %.2f\n", battery_voltage);
  }
}