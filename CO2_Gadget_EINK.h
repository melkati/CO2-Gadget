
#ifndef CO2_Gadget_EINK_h
#define CO2_Gadget_EINK_h

#ifdef SUPPORT_EINK

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP EINK DISPLAY FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <GxEPD2_BW.h>


#ifdef EINKBOARDDEPG0213BN
#include "bootlogo.h"
#include "icons.h"
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = FreeMonoBold24pt7b;
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17      // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> u8g2(GxEPD2_213_BN(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif

#ifdef EINKBOARDGDEW029T5
#include "bootlogo.h" // Made with https://javl.github.io/image2cpp/
#include "icons.h"
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = FreeMonoBold18pt7b;
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17  // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS  // 17
#define EPD_RST 16
#define EPD_BUSY 4
GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> u8g2(GxEPD2_290_T5(/* EPD_CS */ 5, /* EPD_MISO */ 17, /* EPD_RST */ 16, /* EPD_BUSY */ 4));  // GDEW029T5
#endif

char oled_msg[20];
int displayWidth = 128;
int displayHeight = 64;
// #define MENUFONT u8g2_font_6x10_mf
#define MENUFONT u8g2_font_5x8_mf

void setDisplayBrightness(uint32_t newBrightness) {
    Serial.printf("-->[EINK] Setting display brightness value at %d\n", newBrightness);
    // u8g2.setContrast(newBrightness);
    // actualDisplayBrightness = newBrightness;
}

void turnOffDisplay() {
    setDisplayBrightness(0);  // Turn off the display
}

void displaySplashScreen() {
    u8g2.setFullWindow();
    u8g2.firstPage();
    do {
        // Draw bitmap
        u8g2.fillScreen(GxEPD_WHITE);
        u8g2.drawInvertedBitmap((u8g2.width() - 250) / 2, (u8g2.height() - 128) / 2, splash, 250, 128, GxEPD_BLACK);
    } while (u8g2.nextPage());
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

void drawLogoScreen() {
    u8g2.setFullWindow();
    u8g2.firstPage();
    do {
        // Draw bitmap
        u8g2.fillScreen(GxEPD_WHITE);
        u8g2.drawInvertedBitmap((u8g2.width() - 128) / 2, (u8g2.height() - 64) / 2, splash, 128, 64, GxEPD_BLACK);
    } while (u8g2.nextPage());
}

void drawMainScreen() {
    u8g2.setFullWindow();
    u8g2.firstPage();
    do {
        // Draw bitmap
        u8g2.fillScreen(GxEPD_WHITE);
        u8g2.setFont(&BigFont);
        u8g2.setCursor(0, 40);
        u8g2.print("CO2: 0 ppm");
        u8g2.setFont(&SmallFont);
        u8g2.setCursor(0, 80);
        u8g2.print("Temp: 0.0 C");
        u8g2.setCursor(0, 100);
        u8g2.print("Hum: 0.0 %");
    } while (u8g2.nextPage());
}

void initDisplay() {
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);

    // configDisplay();

    // Show splash screen the first time
    // if (bootCount == 1) {
    displaySplashScreen();
    delay(1000);
    drawLogoScreen();
    delay(1000);
    drawMainScreen();
    // }

    // // Each 25 boots do a full screen refresh
    // if (bootCount > 1 && (bootCount % bootsToFullUpdate == 0)) {
    //     display.fillScreen(GxEPD_WHITE);
    //     display.display();
    //     drawMainScreen();
    // }
    // DisplayInititialized = true;
}

void displayShowValues(bool forceRedraw = false) {
    if (forceRedraw) {
        // tft.fillScreen(TFT_BLACK);
    }
    // showCO2(co2, elementPosition.co2X, elementPosition.co2Y, elementPosition.pixelsToBaseline, forceRedraw);
    // showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY, forceRedraw);
    // showTemperature(temp, elementPosition.tempX, elementPosition.tempY, forceRedraw);
    // showHumidity(hum, elementPosition.humidityX, elementPosition.humidityY, forceRedraw);
    // showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, forceRedraw);
    // showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    // showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    // showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    // showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    // showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    forceRedraw = false;
}

#endif  // SUPPORT_EINK
#endif  // CO2_Gadget_EINK_h
