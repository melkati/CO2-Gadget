
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

#define SENSIRION_GREEN 0x6E66
#define sw_version "v0.1"

#define GFXFF 1
#define FF99 &SensirionSimple25pt7b
#define FF90 &ArchivoNarrow_Regular10pt7b
#define FF95 &ArchivoNarrow_Regular50pt7b

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
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8);    // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0); // TFT_BL, 0 - 15
  setTFTBrightness(TFTBrightness);
  tft.init();
  tft.setRotation(1);
#endif
}

void displaySplashScreenTFT() {
#if defined SUPPORT_TFT
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(SENSIRION_GREEN, TFT_WHITE);

  uint8_t defaultDatum = tft.getTextDatum();
  tft.setTextDatum(TC_DATUM); // Top centre

  tft.setTextSize(1);
  tft.setFreeFont(FF99);
  tft.drawString("B", 120, 40);

  tft.setTextSize(1);
  tft.drawString(sw_version, 120, 90, 2);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);
  delay(500);
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 240, 135, bootlogo);

#endif
}

void showVoltageTFT() {
#if defined SUPPORT_TFT
  // Draw Voltaje number
  if (battery_voltage <= 3.6) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (battery_voltage <= 3.8) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }

  tft.setTextDatum(BC_DATUM); // Bottom centre
  tft.drawString(String(battery_voltage) + "V", tft.width() / 2, 125);
#endif
}

void showValuesTFT(uint16_t co2) {
#if defined SUPPORT_TFT
  if (co2 > 9999) {
    co2 = 9999;
  }

  tft.fillScreen(TFT_BLACK);

  uint8_t defaultDatum = tft.getTextDatum();

  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setTextDatum(BL_DATUM); // bottom left
  tft.drawString("CO2", 10, 125);

  tft.setTextDatum(BR_DATUM); // bottom right
  if (activeBLE) {
    tft.drawString(gadgetBle.getDeviceIdString(), 230, 125);
  }
  
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
  tft.drawString(String(co2), 195, 105);

  // Draw CO2 unit
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.drawString("ppm", 230, 90);

  showVoltageTFT();

  // Revert datum setting
  tft.setTextDatum(defaultDatum);

  // set default font for menu
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
#endif
}
