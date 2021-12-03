
// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP TFT DISPLAY FUNCTIONALITY                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_TFT
// Go to TTGO T-Display's Github Repository
// Download the code as zip, extract it and copy the Folder TFT_eSPI
//  => https://github.com/Xinyuan-LilyGO/TTGO-T-Display/archive/master.zip
// to your Arduino library path

#include <TFT_eSPI.h>
#include <SPI.h>
#include "bootlogo.h"
#include "icons.h"
#include "ArchivoNarrow_Regular10pt7b.h"
#include "FontCO2Gadget50ptDigits.h"
// #define SENSIRION_GREEN 0x6E66
// #define sw_version "v0.1"

#define GFXFF 1
#define FF99 &SensirionSimple25pt7b
#define FF90 &ArchivoNarrow_Regular10pt7b

// RAM:   [==        ]  21.4% (used 69976 bytes from 327680 bytes)
// Flash: [==========]  96.6% (used 1899972 bytes from 1966080 bytes)
// #define FF95 &ArchivoNarrow_Regular50pt7b

// RAM:   [==        ]  21.4% (used 69976 bytes from 327680 bytes)
// Flash: [==========]  95.4% (used 1876292 bytes from 1966080 bytes)
#define FF95 &FontCO2GadgetDigits50pt7b


uint32_t TFTBrightness = 100;

TFT_eSPI tft =
    TFT_eSPI(135, 240); // Invoke library, pins defined in User_Setup.h
// TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
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
  ledcAttachPin(BACKLIGHT_PIN, 0); // BACKLIGHT_PIN, 0 - 15
  setTFTBrightness(TFTBrightness);
  tft.init();
  tft.setRotation(1);
#endif
}

void displaySplashScreenTFT() {
#if defined SUPPORT_TFT
  // tft.fillScreen(TFT_WHITE);
  // tft.setTextColor(SENSIRION_GREEN, TFT_WHITE);

  // uint8_t defaultDatum = tft.getTextDatum();
  // tft.setTextDatum(TC_DATUM); // Top centre

  // // tft.setTextSize(1);
  // // tft.setFreeFont(FF99);
  // // tft.drawString("B", 120, 40);

  // // tft.setTextSize(1);
  // // tft.drawString(sw_version, 120, 90, 2);

  // // Revert datum setting
  // tft.setTextDatum(defaultDatum);
  // delay(500);
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, bootlogo);

#endif
}



void showBatteryIconTFT() {
  uint8_t batteryLevel = battery.level();
  
  uint16_t color;
  if (batteryLevel<20) {
    color = TFT_RED;
  } else {
    color = TFT_WHITE;
  } 
  
  tft.drawRect(tft.width() - 27, 2, 26, 12, color); // Battery outter rectangle
  tft.drawLine(tft.width() - 2, 4, tft.width(), 10, color);

  if (batteryLevel > 20)
  {
    tft.fillRect(tft.width() - 25, 4, 4, 10, color);
  }
  if (batteryLevel > 40)
  {
    tft.fillRect(tft.width() - 19, 4, 4, 10, color);
  }
  if (batteryLevel > 60)
  {
    tft.fillRect(tft.width() - 13, 4, 4, 10, color);
  }
  if (batteryLevel > 80)
  {
    tft.fillRect(tft.width() - 7, 4, 4, 10, color);
  }
}

void showWiFiIconTFT(int32_t posX, int32_t posY) {
  int8_t rssi = WiFi.RSSI();
  tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_BLUE);
  if (!activeWIFI) {
    tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, TFT_DARKGREY);
    tft.drawLine(posX+3, posY+3, posX+10, posY+10, TFT_RED);
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      if (rssi < 60)
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, TFT_GREEN);
      else if (rssi < 70)
        tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_ORANGE);
      else if (rssi < 90)
        tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_YELLOW);
    } else {
      tft.drawBitmap(posX, posY, iconWiFiEmpty, 16, 16, TFT_BLACK, TFT_RED);
    }
  }
}

void showBLEIconTFT(int32_t posX, int32_t posY) {
  tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_BLUE);
  if (!activeBLE) {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_DARKGREY);
    tft.drawLine(posX+3, posY+3, posX+10, posY+10, TFT_RED);
  } else {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_GREEN);
  }
}

void showMQTTIconTFT(int32_t posX, int32_t posY) {
  tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_BLUE);
  if (!activeMQTT) {
    tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, TFT_DARKGREY);
    tft.drawLine(posX+3, posY+3, posX+10, posY+10, TFT_RED);
  } else {
    tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, TFT_GREEN);
  }
}

void showTemperatureTFT(int32_t posX, int32_t posY) {
#if defined SUPPORT_TFT
  // Draw Voltaje number
  if (temp >= 28) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (temp >= 23) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else if (temp >= 18) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  } else if (temp >= 13) {
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
  } else if (temp >= 8) {
    tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
  }  
  tft.setTextDatum(BL_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(posX, posY-20, 16, 16, iconTemperature);
  tft.drawString(String(temp, 1) + "~", posX+18, posY);
#endif
}

void showHumidity(int32_t posX, int32_t posY) {
#if defined SUPPORT_TFT
  if (hum<=25) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (hum<30) tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else if (hum<60) tft.setTextColor(TFT_GREEN, TFT_BLACK);
  else if (hum<70) tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else tft.setTextColor(TFT_RED, TFT_BLACK);

  tft.setTextDatum(BL_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(posX, posY-20, 16, 16, iconHumidity);
  tft.drawString(String(hum, 0) + "%", posX+18, posY);
#endif
}

void showVoltageTFT(int32_t posX, int32_t posY) {
#if defined SUPPORT_TFT
  // Draw Voltaje number
  if (battery_voltage <= 3.6) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (battery_voltage <= 3.8) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else if (battery_voltage <= 4.5) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
  }

  tft.setTextDatum(BL_DATUM);
  tft.drawString(String(battery_voltage, 1) + "V", 5*10, posY);
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
tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setTextDatum(BL_DATUM); // bottom left
  // tft.drawString("CO2", 0, 135);

  
  
  // Draw CO2 number
  if (co2 >= co2RedRange) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (co2 >= co2OrangeRange) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }

  tft.setTextDatum(BR_DATUM); // bottom right
  tft.setTextSize(1);
  tft.setFreeFont(FF95);
  tft.drawString(String(co2), 195, 115);

  // Draw CO2 unit
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.drawString("ppm", 230, 100);
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
  showTemperatureTFT(0, 135);
  showHumidity(180, 135);
  showBatteryIconTFT();
  showWiFiIconTFT(5, 3);
  showBLEIconTFT(31, 3);
  showMQTTIconTFT(57, 3);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);

  // set default font for menu
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
#endif
}
