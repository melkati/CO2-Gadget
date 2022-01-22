

#ifndef CO2_Gadget_TFT_h
#define CO2_Gadget_TFT_h

#ifdef SUPPORT_TFT

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP TFT DISPLAY FUNCTIONALITY                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

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

uint16_t iconDefaultColor = TFT_CYAN;

TFT_eSPI tft =
    TFT_eSPI(135, 240); // Invoke library, pins defined in User_Setup.h

void setDisplayBrightness(uint32_t newBrightness) {
  Serial.printf("-->[TFT ] Setting display brightness value at %d\n", newBrightness);
  ledcWrite(0, newBrightness); // 0-15, 0-255 (with 8 bit resolution); 0=totally
                               // dark;255=totally shiny
}

void turnOffDisplay() {
  setDisplayBrightness(0); // Turn off the display
}

void displaySplashScreen() {
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  tft.pushImage(60, 12, 118, 40, eMarieteLogo);
  tft.pushImage(10, 50, 92, 72, CO2Logo);
  tft.pushImage(112, 67, 122, 46, GadgetLogo);
}

void initDisplay() {
  pinMode(BACKLIGHT_PIN, OUTPUT);
  ledcSetup(0, 5000, 8);    // 0-15, 5000, 8
  ledcAttachPin(BACKLIGHT_PIN, 0); // TFT_BL, 0 - 15
  setDisplayBrightness(DisplayBrightness);
  tft.init();
  if (displayReverse) {
    tft.setRotation(3);
  } else {
    tft.setRotation(1);
  }

  displaySplashScreen(); // Display init and splash screen
  delay(2000);              // Enjoy the splash screen for 2 seconds
  tft.setTextSize(2);
}

/***************************************************************************************
** Function name:           displayNotification
** Description:             Display a boxed  notification in the display
***************************************************************************************/
// parameters:
//      notificationText = string to display.
//      notificationTypes one of enum notificationTypes notifyNothing, notifyInfo, notifyWarning, notifyError
bool displayNotification(String notificationText, notificationTypes notificationType) {
  uint16_t textColor, boxColor, backgroundColor, boxMarging = 15;
  // if (notificationType=1) {
    textColor = TFT_RED;
    boxColor = TFT_SILVER;
    backgroundColor = TFT_BLUE;
  // }

  // TO-DO: Size rectangle to text size
  // int16_t textWidth = tft.textWidth(notificationText);
  // int16_t textHeight = tft.fontHeight();

  tft.setViewport(boxMarging, boxMarging, tft.width() - boxMarging*2, tft.height() - boxMarging*2);
  tft.fillRect( 0, 0, tft.width(), tft.height(), backgroundColor);
  tft.drawRect( 0, 0, tft.width(), tft.height(), boxColor);
  
  tft.setTextDatum(CC_DATUM);
  tft.setTextColor(textColor, backgroundColor);
  tft.drawString(notificationText, tft.width()/2, tft.height()/2);
  tft.resetViewport();
  return true;
}

/***************************************************************************************
** Function name:           displayNotification
** Description:             Display a boxed two line notification in the display
***************************************************************************************/
// parameters:
//      notificationText = string to display.
//      notificationTypes one of enum notificationTypes notifyNothing, notifyInfo, notifyWarning, notifyError
bool displayNotification(String notificationText, String notificationText2, notificationTypes notificationType) {
  uint16_t textColor, boxColor, backgroundColor, boxMarging = 10;
  // if (notificationType=1) {
    textColor = TFT_RED;
    boxColor = TFT_SILVER;
    backgroundColor = TFT_BLUE;
  // }

  // TO-DO: Size rectangle to text size
  int16_t textWidth  = tft.textWidth(notificationText);
  int16_t textWidth2 = tft.textWidth(notificationText2);
  // int16_t textHeight = tft.fontHeight();

  tft.setViewport(boxMarging, boxMarging, tft.width() - boxMarging*2, tft.height() - boxMarging*2);
  tft.fillRect( 0, 0, tft.width(), tft.height(), backgroundColor);
  tft.drawRect( 0, 0, tft.width(), tft.height(), boxColor);
  
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(textColor, backgroundColor);
  tft.drawString(notificationText, tft.width()/2-textWidth/2, tft.height()/5*1+boxMarging);
  tft.drawString(notificationText2, tft.width()/2-textWidth2/2, tft.height()/5*3-boxMarging);
  tft.resetViewport();
  return true;
}

void showBatteryIcon() {
  if (!displayShowBattery) return;
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

void showWiFiIcon(int32_t posX, int32_t posY) {
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

void showBLEIcon(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_DARKGREY);
  if (!activeBLE) {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_DARKGREY);
  } else {
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, iconDefaultColor);
  }
}

void showMQTTIcon(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX-2, posY-2, 16+4, 16+4, 2, TFT_DARKGREY);
  if (!activeMQTT) {
    tft.drawBitmap(posX-1, posY-1, iconMQTT, 16, 16, TFT_BLACK, TFT_DARKGREY);
  } else {
    tft.drawBitmap(posX-1, posY-1, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
  }
}

void showTemperature() {
  if (!displayShowTemperature) return;
  if (temp >= 30)      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else if (temp >= 10) tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  else                 tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);    
  tft.setTextDatum(BL_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(2, tft.height()-22, 16, 16, iconTemperature);
  if (showFahrenheit) {
    tft.drawString(String(tempFahrenheit, 1) + "~" , 22 , tft.height()-2);  // The "~" symbol has been redefined in custom font as the degree symbol
  } else {
    tft.drawString(String(temp, 1) + "~" , 22 , tft.height()-2);
  }  
}

void showHumidity() {
  if (!displayShowHumidity) return;
  if (hum<=25)     tft.setTextColor(TFT_WHITE, TFT_RED);
  else if (hum<40) tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  else if (hum<=60) tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  else if (hum<75) tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
  else tft.setTextColor(TFT_RED, TFT_SKYBLUE);
  tft.setTextDatum(BR_DATUM);
  tft.setSwapBytes(true);
  tft.pushImage(tft.width()-60, tft.height()-22, 16, 16, iconHumidity);
  tft.drawString(String(hum, 0) + "%", tft.width()-6, tft.height()-2);
}

void showVoltage(int32_t posX, int32_t posY) {
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
}

void showBLEDeviceId(int32_t posX, int32_t posY) {  
  #ifdef SUPPORT_BLE
  if (activeBLE) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(BR_DATUM); // bottom right
    tft.drawString(gadgetBle.getDeviceIdString(), posX, posY);
  }
  #endif
}

void showCO2(uint16_t co2) {
  if (co2 > 9999) {
    co2 = 9999;
  }
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

void displayShowValues(uint16_t co2) {
  tft.fillScreen(TFT_BLACK);
  uint8_t defaultDatum = tft.getTextDatum();
  showCO2(co2);
  // showBLEDeviceId(230, 135);
  // showVoltage(0, 135);
  showTemperature();
  showHumidity();
  showBatteryIcon();
  showWiFiIcon(24, 3);
  showMQTTIcon(46, 3);
  showBLEIcon(2, 3);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);

  // set default font for menu
  tft.setFreeFont(NULL);
  tft.setTextSize(2);
}

#endif  // SUPPORT_WEBCONFIG
#endif  // CO2_Gadget_TFT_h