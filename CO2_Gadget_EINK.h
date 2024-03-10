// https://rop.nl/truetype2gfx/ to convert fonts to .h files
// https://oleddisplay.squix.ch/#/home
// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

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
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "bootlogo.h"
#include "icons.h"
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = FreeMonoBold24pt7b;
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17      // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif

#ifdef EINKBOARDGDEW029T5
#include <NotoSans_Bold48pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"
// https://github.com/olikraus/u8g2/wiki/fntlist99
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17  // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS  // 17
#define EPD_RST 16
#define EPD_BUSY 4
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=*/5, /*DC=*/27, /*RST=*/25, /*BUSY=*/32));  // GDEM029T94
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/* EPD_CS */ 5, /* EPD_MISO */ 17, /* EPD_RST */ 16, /* EPD_BUSY */ 4));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));
// GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/* EPD_CS */ 5, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT> display(GxEPD2_290_T5D(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEW029T5
#endif

char oled_msg[20];
int displayWidth = 128;
int displayHeight = 64;
// #define MENUFONT u8g2_font_6x10_mf
// #define MENUFONT u8g2_font_5x8_mf

void setDisplayBrightness(uint32_t newBrightness) {
    Serial.printf("-->[EINK] Setting display brightness value at %d\n", newBrightness);
    // display.setContrast(newBrightness);
    // actualDisplayBrightness = newBrightness;
}

void turnOffDisplay() {
    setDisplayBrightness(0);  // Turn off the display
}

void drawScreenCenterText(const String text) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // Calculate text bounds
    display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);

    // Center bounding box by transposition of origin
    uint16_t x = ((display.width() - tbw) / 2) - tbx;
    uint16_t y = ((display.height() - tbh) / 2) - tby;

    // Display text
    display.setCursor(x, y);
    display.print(text);
}

void displaySplashScreenLOGO() {
    display.setFullWindow();
    display.firstPage();
    do {
        // Draw bitmap
        display.fillScreen(GxEPD_WHITE);
        display.drawInvertedBitmap((display.width() - 250) / 2, (display.height() - 128) / 2, Logo250x128, 250, 128, GxEPD_BLACK);
    } while (display.nextPage());
}

void displaySplashScreen() {
    display.setFullWindow();  // Activate full screen refresh
    display.firstPage();      // Clear screen
    do {
        // Show program name & version
        drawScreenCenterText("eMariete CO2 Gadget Monitor");
    } while (display.nextPage());  // Do full refresh
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
    display.setFullWindow();
    display.firstPage();
    do {
        // Draw bitmap
        display.fillScreen(GxEPD_WHITE);
        display.drawInvertedBitmap((display.width() - 128) / 2, (display.height() - 64) / 2, splash, 128, 64, GxEPD_BLACK);
    } while (display.nextPage());
}

void drawHoritzontalCenterText(int16_t y, const String text) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // Calculate text bounds
    display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);

    // Center bounding box by transposition of origin
    uint16_t x = ((display.width() - tbw) / 2) - tbx;

    // Display text
    display.setCursor(x, y);
    display.print(text);
}

void showBattery() {
    Serial.printf("Battery level: %d\n", batteryLevel);
    Serial.flush();

    display.drawRect(display.width() - 27, 2, 26, 12, GxEPD_BLACK);  // Battery outter rectangle
    display.drawLine(display.width() - 2, 4, display.width(), 10, GxEPD_BLACK);
    if (batteryLevel > 20) {
        display.fillRect(display.width() - 25, 4, 4, 10, GxEPD_BLACK);
    }
    if (batteryLevel > 40) {
        display.fillRect(display.width() - 19, 4, 4, 10, GxEPD_BLACK);
    }
    if (batteryLevel > 60) {
        display.fillRect(display.width() - 13, 4, 4, 10, GxEPD_BLACK);
    }
    if (batteryLevel > 80) {
        display.fillRect(display.width() - 7, 4, 4, 10, GxEPD_BLACK);
    }
}

void drawMainScreen() {
    static uint16_t drawTimes = 0;
    if (drawTimes > 0) {
        return;
    }
    timer.start();
    drawTimes++;

    // Enable partial refresh
    display.setPartialWindow(0, 0, display.width(), display.height());

    // Clear screen
    display.fillScreen(GxEPD_WHITE);

    // Draw labels and field rectangles
    display.drawRoundRect(0, 20, display.width(), display.height() - 30, 6, GxEPD_BLACK);
    display.setRotation(1);
    // display.setFont(&Orbitron_Bold_12);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(0, 12);
    display.print("TEMP: ");
    // display.setCursor((display.width() / 2), 12);
    display.setCursor((display.width()) - 5 * 9 * 2 - 35, 12);
    display.print("HUM: ");

    // display.drawRect(display.width()-27, 2, 27, 12, GxEPD_BLACK); // Battery outter rectangle

    // display.setCursor(((display.width() / 3) * 2) + 12, 12);
    // display.print("BATT: ");

    display.setRotation(4);
    display.setCursor((display.width() / 2) - 20, display.height() - 3);
    display.print("PPM");
    display.setRotation(1);

    // Refresh screen in partial mode
    display.displayWindow(0, 0, display.width(), display.height());
    Serial.print("time used to drawMainScreen: ");
    Serial.println(timer.read());
}

void showValues() {
    static uint16_t old_co2value = 0;
    static float old_co2temp = 0;
    static float old_co2humi = 0;

    if (old_co2value == co2 && old_co2temp == temp && old_co2humi == hum) {
        return;
    }

    timer.start();

    // Erase old values
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&BigFont);
    display.setTextSize(1);
    drawHoritzontalCenterText((display.height() / 2) + 40, String(old_co2value));
    display.setTextSize(1);
    display.setFont(&SmallFont);
    display.setCursor(55, 12);
    display.printf("%.1fºC", old_co2temp);
    display.setCursor((display.width()) - 5 * 9, 12);
    display.printf("%.0f%%", old_co2humi);

    // Show values
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&BigFont);
    display.setTextSize(1);
    drawHoritzontalCenterText((display.height() / 2) + 40, String(co2));
    display.setTextSize(1);
    display.setFont(&SmallFont);
    display.setCursor(55, 12);
    display.printf("%.1fºC", temp);
    display.setCursor((display.width()) - 5 * 9 - 35, 12);
    display.printf("%.0f%%", hum);

    // Refresh screen in partial mode
    // display.displayWindow(0, 0, display.width(), display.height());

    showBattery();

    // display.setTextColor(GxEPD_BLACK);
    // display.setCursor(240, 12);
    // display.print(readBattery());

    display.displayWindow(0, 0, display.width(), display.height());

    old_co2value = co2;
    old_co2temp = temp;
    old_co2humi = hum;

    Serial.print("time used to showValues: ");
    Serial.println(timer.read());
}

void initDisplay() {
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(115200, true, 2, false);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

    // Set default options to draw
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    display.setRotation(1);

    // Show splash screen the first time
    // if (bootCount == 1) {
    displaySplashScreenLOGO();
    delay(4000);
    displaySplashScreen();
    // delay(1000);
    // drawLogoScreen();
    // delay(1000);
    drawMainScreen();
    // }

    // // Each 25 boots do a full screen refresh
    // if (bootCount > 1 && (bootCount % bootsToFullUpdate == 0)) {
    //     display.fillScreen(GxEPD_WHITE);
    //     display.display();
    //     drawMainScreen();
    // }
    // DisplayInititialized = true;
    display.hibernate();
}

void displayShowValues(bool forceRedraw = false) {    
    if (forceRedraw) {
        // tft.fillScreen(TFT_BLACK);
    }
    drawMainScreen();
    showValues();
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
