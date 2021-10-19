/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP OLED DISPLAY FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_OLED
#include <U8x8lib.h>
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
char oled_msg[20];
int displayWidth = 128;
int displayHeight = 64;

#endif

void initDisplayOLED() {
#if defined SUPPORT_OLED
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "  eMariete.com");
  u8x8.drawString(0, 2, "   Sensirion");
  u8x8.drawString(0, 3, "CO2 Gadget");
  u8x8.drawString(0, 4, "Concentration Monitor");
#endif
}

void showValuesOLED(String text) {
#if defined SUPPORT_OLED
  u8x8.clearLine(2);
  u8x8.clearLine(3);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 4, "CO2: ");
  u8x8.setFont(u8x8_font_courB18_2x3_r);
  sprintf(oled_msg, "%4d", co2); // If parameter string then: co2.c_str()
  u8x8.drawString(4, 3, oled_msg);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(12, 4, "ppm");

  u8x8.clearLine(6);
  sprintf(oled_msg, "T:%.1fC RH:%.0f%%", temp, hum);
  u8x8.drawUTF8(0, 6, oled_msg);

#ifdef SUPPORT_WIFI
  if (WiFiMulti.run() != WL_CONNECTED) {
    u8x8.clearLine(7);
    u8x8.drawUTF8(0, 6, "WiFi unconnected");
  } else {
    u8x8.clearLine(7);
    IPAddress ip = WiFi.localIP();
    sprintf(oled_msg, "%s", ip.toString().c_str());
    // sprintf("IP:%u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
    u8x8.drawString(0, 7, oled_msg);
  }
#endif
#endif
}