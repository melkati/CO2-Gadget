
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP TFT DISPLAY FUNCTIONALITY                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_TFT

#include <TFT_eSPI.h>
#include <SPI.h>
#include "bootlogo.h"
#include "icons.h"
#include "FontArchivoNarrow_Regular10pt7b.h"
#include "FontCO2Gadget50ptDigits.h"

// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/
#define GFXFF 1
#define FF90 &ArchivoNarrow_Regular10pt7b
#define FF95 &FontCO2GadgetDigits50pt7b

// RAM:   [==        ]  21.4% (used 69976 bytes from 327680 bytes)
// Flash: [==========]  95.3% (used 1874104 bytes from 1966080 bytes)

uint32_t TFTBrightness = 100;
uint16_t iconDefaultColor = TFT_CYAN;

TFT_eSPI tft =
    TFT_eSPI(135, 240); // Invoke library, pins defined in User_Setup.h
#endif

void setTFTBrightness(uint32_t newBrightness) {
  Serial.printf("Setting screen brightness value at %d\n", newBrightness);
  ledcWrite(0, newBrightness); // 0-15, 0-255 (with 8 bit resolution); 0=totally
                               // dark;255=totally shiny
}

void initDisplayTFT() {
#if defined SUPPORT_TFT
  pinMode(BACKLIGHT_PIN, OUTPUT);
  ledcSetup(0, 5000, 8);    // 0-15, 5000, 8
  ledcAttachPin(BACKLIGHT_PIN, 0); // TFT_BL, 0 - 15
  setTFTBrightness(TFTBrightness);
  tft.init();
  tft.setRotation(1);
#endif
}

void displaySplashScreenTFT() {
#if defined SUPPORT_TFT
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  tft.pushImage(60, 12, 118, 40, eMarieteLogo);
  tft.pushImage(10, 50, 92, 72, CO2Logo);
  tft.pushImage(112, 67, 122, 46, GadgetLogo);
#endif
}

void showBatteryIconTFT() {
  uint8_t batteryLevel = battery.level();
  uint16_t color;
  if (batteryLevel<20) {
    color = TFT_RED;
  } else {
    color = TFT_SILVER;
  } 
  
  if(battery_voltage>4.5){ // Charging...
    color = iconDefaultColor;
  }

  tft.drawRoundRect(tft.width() - 32, 4, 28, 14, 2, color); // Battery outter rectangle
  tft.drawLine(tft.width() - 3, 9, tft.width()-3, 13, color);

  if (batteryLevel > 20)  tft.fillRect(tft.width() - 29, 6, 4, 10, color);
  if (batteryLevel > 40)  tft.fillRect(tft.width() - 23, 6, 4, 10, color);
  if (batteryLevel > 60)  tft.fillRect(tft.width() - 17, 6, 4, 10, color);
  if (batteryLevel > 80)  tft.fillRect(tft.width() - 11, 6, 4, 10, color);
  
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(TFT_SILVER, TFT_BLACK);
  tft.drawString(String(battery_voltage, 1) + "V", tft.width() - 35, 2);
}

void showWiFiIconTFT(int32_t posX, int32_t posY) {
  int8_t rssi = WiFi.RSSI();
    tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_DARKGREY);
  if (!activeWIFI) {
    tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, TFT_DARKGREY );
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      if (rssi < 60)
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
      else if (rssi < 70)
        tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_ORANGE);
      else if (rssi < 90)
        tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_YELLOW);
    } else {
      tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_RED);
    }
  }
}

void showBLEIconTFT(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_DARKGREY);
  if (!activeBLE) {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_DARKGREY);
  } else {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, iconDefaultColor);
  }
}

void showMQTTIconTFT(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_DARKGREY);
  if (!activeMQTT) {
    tft.drawBitmap(posX-1, posY-1, iconMQTT, 16, 16, TFT_BLACK, TFT_DARKGREY);
  } else {
    tft.drawBitmap(posX-1, posY-1, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
  }
}

void showTemperatureTFT() {
#if defined SUPPORT_TFT
  // Draw Voltaje number
  if (temp >= 30)      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else if (temp >= 10) tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  else                 tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    
  tft.setTextDatum(BL_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(2, tft.height()-22, 16, 16, iconTemperature);
  tft.drawString(String(temp, 1) + "~" , 22 , tft.height()-2);
  #endif
}

void showHumidity() {
#if defined SUPPORT_TFT
  if (hum<=25)     tft.setTextColor(TFT_WHITE, TFT_RED);
  else if (hum<40) tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else if (hum<=60) tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  else if (hum<75) tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
  else tft.setTextColor(TFT_RED, TFT_SKYBLUE);

  tft.setTextDatum(BR_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(tft.width()-60, tft.height()-22, 16, 16, iconHumidity);
  tft.drawString(String(hum, 0) + "%", tft.width()-6, tft.height()-2);
#endif
}

void showVoltageTFT(int32_t posX, int32_t posY) {
#if defined SUPPORT_TFT
  // Draw Voltaje number
  uint16_t battery_voltage = battery.voltage();
  if (battery_voltage <= 3.6) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (battery_voltage <= 3.8) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else if (battery_voltage <= 4.5) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
  }

  tft.setTextDatum(TR_DATUM);
  tft.drawString(String(battery_voltage, 1) + "V", posX, posY);
#endif
}

void showBLEDeviceIdTFT(int32_t posX, int32_t posY) {  
  if (activeBLE) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(BR_DATUM); // bottom right
    tft.drawString(gadgetBle.getDeviceIdString(), posX, posY);
  }
}

void showCO2TFT(uint16_t co2) {
  if (co2 >= co2RedRange) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (co2 >= co2OrangeRange) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }

  tft.setTextSize(1);
  // Draw CO2 number
  tft.setTextDatum(BR_DATUM); // bottom right
  tft.setFreeFont(FF95);
  tft.drawString(String(co2), tft.width()-39, 112);

  // Draw CO2 unit
  tft.setTextDatum(BR_DATUM); // bottom right
  tft.setFreeFont(FF90);
  tft.drawString("ppm", tft.width()-4, 102);
}

void showValuesTFT(uint16_t co2) {
#if defined SUPPORT_TFT
  if (co2 > 9999) {
    co2 = 9999;
  }

  tft.fillScreen(TFT_BLACK);
  uint8_t defaultDatum = tft.getTextDatum();
  showCO2TFT(co2);
  // showBLEDeviceIdTFT(230, 135);
  // showVoltageTFT(0, 135);
  showTemperatureTFT();
  showHumidity();
  showBatteryIconTFT();
  showWiFiIconTFT(24, 3);
  showMQTTIconTFT(46, 3);
  showBLEIconTFT(2, 3);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);

  // set default font for menu
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
#endif
}
