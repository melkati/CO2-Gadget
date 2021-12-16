
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
U8X8_SH1106_128X64_NONAME_HW_I2C u8g2(/* reset=*/U8X8_PIN_NONE);
// U8G2_SSD1306_128X64_NONAME_HW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


char oled_msg[20];
int displayWidth = 128;
int displayHeight = 64;

void setOLEDBrightness(uint32_t newBrightness) {
  Serial.printf("Setting screen brightness value at %d\n (unfunctional. TO DO", newBrightness);
}

void turnOffDisplay() {
  setOLEDBrightness(0); // Turn off the display
}

void initDisplayOLED() {
#if defined SUPPORT_OLED
  u8g2.begin();
  u8g2.setPowerSave(0);
  u8g2.setFont(u8x8_font_chroma48medium8_r);
  u8g2.drawString(0, 1, "  eMariete.com");
  u8g2.drawString(0, 2, "   Sensirion");
  u8g2.drawString(0, 3, "CO2 Gadget");
  u8g2.drawString(0, 4, "Concentration Monitor");
#endif
}

void showValuesOLED(String text) {
#if defined SUPPORT_OLED
  u8g2.clearLine(2);
  u8g2.clearLine(3);
  u8g2.setFont(u8x8_font_chroma48medium8_r);
  u8g2.drawString(0, 4, "CO2: ");
  u8g2.setFont(u8x8_font_courB18_2x3_r);
  sprintf(oled_msg, "%4d", co2); // If parameter string then: co2.c_str()
  u8g2.drawString(4, 3, oled_msg);
  u8g2.setFont(u8x8_font_chroma48medium8_r);
  u8g2.drawString(12, 4, "ppm");

  u8g2.clearLine(6);
  sprintf(oled_msg, "T:%.1fC RH:%.0f%%", temp, hum);
  u8g2.drawUTF8(0, 6, oled_msg);

  if (activeWIFI) {
  if (WiFi.status() != WL_CONNECTED) {
    u8g2.clearLine(7);
    u8g2.drawUTF8(0, 6, "WiFi unconnected");
  } else {
    u8g2.clearLine(7);
    IPAddress ip = WiFi.localIP();
    sprintf(oled_msg, "%s", ip.toString().c_str());
    // sprintf("IP:%u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) &
    // 0xff, ip >> 24);
    u8g2.drawString(0, 7, oled_msg);
  }
  }
#endif
}

#endif  // SUPPORT_OLED
#endif  // CO2_Gadget_OLED_h
