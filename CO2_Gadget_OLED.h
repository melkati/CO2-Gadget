
#ifndef CO2_Gadget_OLED_h
#define CO2_Gadget_OLED_h

#ifdef SUPPORT_OLED

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP OLED DISPLAY FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
// #include <U8x8lib.h>
#include <U8g2lib.h>
#include "bootlogo.h"
#include "icons.h"
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // Frame Buffer: clearBuffer/sendBuffer. More RAM usage, Faster
// U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // Page Buffer: firstPage/nextPage. Less RAM usage, Slower

char oled_msg[20];
int displayWidth = 128;
int displayHeight = 64;
// #define MENUFONT u8g2_font_6x10_mf
#define MENUFONT u8g2_font_5x8_mf

void setDisplayBrightness(uint32_t newBrightness) {
  Serial.printf("Setting screen brightness value at %d\n (unfunctional. TO DO", newBrightness);
}

void turnOffDisplay() {
  setDisplayBrightness(0); // Turn off the display
}

void displaySplashScreen() {
  u8g2.clearDisplay();
  u8g2.firstPage();
  do {
    // u8g2.drawXBMP(30, 0, 59, 20, eMarieteLogo);
    // u8g2.drawXBM(7, 23, 46, 36, CO2Logo);
    // u8g2.drawXBM(60, 32, 61, 23, GadgetLogo);
    u8g2.drawXBM(0, 0, 128, 64, splash);
  } while (u8g2.nextPage());
  u8g2.setFont(MENUFONT);
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
  return true;
}

bool displayNotification(String notificationText, String notificationText2, notificationTypes notificationType) {
  uint16_t textColor, boxColor, backgroundColor, boxMarging = 15;
  return true;
}

void initDisplay() {
  Serial.printf("-->[OLED] Initialized: \t#%s#\n",
                ((u8g2.begin()) ? "OK" : "Failed"));
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(0, 15, "  eMariete.com");
    u8g2.drawStr(0, 33, "   CO2 Gadget");
    u8g2.drawStr(0, 51, "  Air  Quality");
  } while (u8g2.nextPage());
  u8g2.setFont(MENUFONT);
  if (displayReverse) {
    u8g2.setDisplayRotation(U8G2_R2);
  } else {
    u8g2.setDisplayRotation(U8G2_R0);
  }

  displaySplashScreen();
  delay(1000);
}

void displayShowValues(uint16_t co2) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_7Segments_26x42_mn);
    u8g2.setCursor(0, 44);
    u8g2.print(co2);
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(110, 51);
    u8g2.print("ppm");
    u8g2.setCursor(1, u8g2.getDisplayHeight());
    u8g2.print("T: ");
    u8g2.print(temp,1);
    u8g2.print(showFahrenheit ? "F" : "C");
    u8g2.setCursor(85, u8g2.getDisplayHeight());
    u8g2.print("HR: ");
    u8g2.print(hum,0);
    u8g2.print("%");
  } while (u8g2.nextPage());
  u8g2.setFont(MENUFONT);
}

#endif  // SUPPORT_OLED
#endif  // CO2_Gadget_OLED_h
