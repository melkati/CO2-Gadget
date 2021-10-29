#include "Sensirion_GadgetBle_Lib.h"
GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);

void initBLE() {
  // Initialize the GadgetBle Library
  if (activeBLE) {
    gadgetBle.begin();
    Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
    Serial.println(gadgetBle.getDeviceIdString());
  }
}

void publishBLE() {
    if (activeBLE) {
        gadgetBle.writeCO2(co2);
        gadgetBle.writeTemperature(temp);
        gadgetBle.writeHumidity(hum);
        gadgetBle.commit();
      }
}

void loopBLE() {
    if (activeBLE) {
        gadgetBle.handleEvents();
        delay(3);
    }
}