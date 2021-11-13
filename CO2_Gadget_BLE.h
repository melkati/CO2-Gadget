#include "Sensirion_GadgetBle_Lib.h"
GadgetBle gadgetBle = GadgetBle(GadgetBle::DataType::T_RH_CO2_ALT);

void initBLE() {
  if (activeBLE) {
    if (bleInitialized) return; // If BLE is already initialized do nothing and return
    // if (activeWIFI) {
    //   gadgetBle.enableWifiSetupSettings(onWifiSettingsChanged);
    //   gadgetBle.setCurrentWifiSsid(WIFI_SSID_CREDENTIALS);
    // }
    gadgetBle.begin();
    Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
    Serial.println(gadgetBle.getDeviceIdString());
    bleInitialized = true;
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

void BLELoop() {
  if (activeBLE) {
    gadgetBle.handleEvents();
    delay(3);
  }
}

void enableBLE() {
  if (!bleInitialized) return; // If BLE is NOT initialized do nothing and return
  esp_bt_controller_enable(ESP_BT_MODE_BLE);
}

void disableBLE() {
  if (!bleInitialized) return; // If BLE is NOT initialized do nothing and return
  btStop();
}