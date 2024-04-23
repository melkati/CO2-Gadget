// https://rop.nl/truetype2gfx/ to convert fonts to .h files
// https://oleddisplay.squix.ch/#/home
// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

#ifndef CO2_Gadget_EINK_h
#define CO2_Gadget_EINK_h

#ifdef SUPPORT_EINK
#define DEBUG_EINK

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP EINK DISPLAY FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <GxEPD2_BW.h>

// Variables for e-ink functionality -> move to platformio.ini or CO2_Gadget.ino in a future for menu config
uint16_t co2_HYSTERESIS = 10;           // Minimum change in CO2 value to refresh display
int16_t timeBetweenEinkUpdates = 10;    // 10 seconds between refresh display
uint16_t redrawDisplayEveryCycles = 5;  // Redraw display every X partial updates

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEW0213M21)

#include "bootlogo.h"
#include "icons.h"
// #define EPD_SCLK SCK
// #define EPD_MISO 17
// #define EPD_DC 17
// #define EPD_MOSI MOSI
// #define EPD_CS SS
// #define EPD_RST 16
// #define EPD_BUSY 4

#ifdef EINKBOARDDEPG0213BN
#include <NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#ifdef EINKBOARDGDEM0213B74
#include <NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM0213B74 https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#ifdef EINKBOARDGDEW0213M21
#include <NotoSans_Bold38pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold38pt7b;
int displayWidth = 212;
int displayHeight = 104;
// GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex ::HEIGHT> display(GxEPD2_213_flex(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
// GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D ::HEIGHT> display(GxEPD2_213_T5D(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21 ::HEIGHT> display(GxEPD2_213_M21(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW0213M21 104x212, SSD1608 (GDEW0213Z16LW) https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#endif

#ifdef EINKBOARD_WEACT_DEPG0213BN
#include <NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

// #define EPD_SCLK SCK
// #define EPD_MISO 17
// #define EPD_MOSI MOSI
// #define EPD_CS SS
// #define EPD_DC 27
// #define EPD_RST 25
// #define EPD_BUSY 32
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_DDvVZ4N
#endif

#ifdef EINKBOARDGDEM029T94
#include <NotoSans_Bold48pt7b.h>
#include <NotoSans_Bold6pt7b.h>
const GFXfont SmallFont = NotoSans_Bold6pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
int displayWidth = 296;
int displayHeight = 128;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

// #define EPD_SCLK SCK
// #define EPD_MISO 17
// #define EPD_MOSI MOSI
// #define EPD_CS SS
// #define EPD_DC 27
// #define EPD_RST 25
// #define EPD_BUSY 32
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94
// GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94 V2 GDEM029T94  128x296, SSD1680, (FPC-7519 rev.b), Waveshare 2.9" V2 variant
// GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94 BS
// GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW029T5 with special pinout
#endif

#ifdef EINKBOARDGDEH0154D67
#include <NotoSans_Bold38pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold38pt7b;
int displayWidth = 200;
int displayHeight = 200;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

// #define EPD_SCLK SCK
// #define EPD_MISO 17
// #define EPD_MOSI MOSI
// #define EPD_CS SS
// #define EPD_DC 27
// #define EPD_RST 25
// #define EPD_BUSY 32
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEH0154D67 200x200, SSD1681 https://s.click.aliexpress.com/e/_DDvVZ4N
#endif

// Define a structure for the locations of elements
struct ElementLocations {
    u_int16_t bifFontDigitsHeight;
    u_int16_t smallFontDigitsHeight;
    int32_t co2X;
    int32_t co2Y;
    int32_t co2W;
    int32_t co2H;
    int32_t tempXUnits;
    int32_t tempYUnits;
    int32_t tempXValue;
    int32_t tempYValue;
    int32_t tempWValue;
    int32_t tempHValue;
    int32_t humidityXIcon;
    int32_t humidityYUnits;
    int32_t humidityXValue;
    int32_t humidityYValue;
    int32_t humidityWValue;
    int32_t humidityHValue;
    int32_t co2XUnits;
    int32_t co2YUnits;

    int32_t batteryIconX;
    int32_t batteryIconY;
    int32_t batteryVoltageX;
    int32_t batteryVoltageY;
    int32_t wifiIconX;
    int32_t wifiIconY;
    int32_t mqttIconX;
    int32_t mqttIconY;
    int32_t bleIconX;
    int32_t bleIconY;
    int32_t espNowIconX;
    int32_t espNowIconY;
};

// Define an instance of the structure for the chosen resolution
ElementLocations elementPosition;

// Function to set element locations based on screen resolution
void setElementLocations() {
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // Common positions for most displays
    elementPosition.co2X = 0;       // Left screen
    elementPosition.co2Y = 16 + 8;  // Center text in screen. 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2W = display.width() - 16;
    elementPosition.co2H = (display.height() - 32);  // 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2XUnits = 16;                  // Display is rotated so vertical orientation (swaped width & height)
    elementPosition.co2YUnits = display.width() - 18;

    elementPosition.tempXUnits = 0;  // down left corner
    elementPosition.tempYUnits = display.height() - 16;
    elementPosition.tempXValue = 16 + 4;  // down left corner iconTemp + 4px
    elementPosition.tempYValue = elementPosition.tempYUnits;
    elementPosition.tempWValue = 48;  // width of temp value box
    elementPosition.tempHValue = 16;

    elementPosition.humidityWValue = 40;
    elementPosition.humidityXIcon = display.width() - 16 - elementPosition.humidityWValue - 4;  // 4 pixels between iconHumidity and humidity
    elementPosition.humidityYUnits = display.height() - 16;
    elementPosition.humidityHValue = 16;
    elementPosition.humidityXValue = elementPosition.humidityXIcon + 16 + 4;  // 4 pixels between iconHumidity and humidity
    elementPosition.humidityYValue = elementPosition.humidityYUnits;

    elementPosition.batteryIconX = display.width() - 32;
    elementPosition.batteryIconY = 2;
    elementPosition.batteryVoltageX = display.width() - 92;
    elementPosition.batteryVoltageY = 0;

    elementPosition.bleIconX = 0;
    elementPosition.bleIconY = 0;
    elementPosition.wifiIconX = 24;  // 16 pixels bleIcon + 8 pixels between icons
    elementPosition.wifiIconY = 0;
    elementPosition.mqttIconX = 48;  // 16 + 8 + 16 pixels wifiIcon + 8 pixels between icons
    elementPosition.mqttIconY = 0;
    elementPosition.espNowIconX = 72;  // 16 + 8 + 16 + 8 + 16 pixels mqttIcon + 8 pixels between icons
    elementPosition.espNowIconY = 0;
#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEM029T94)
    ;
#endif
#if defined(EINKBOARDGDEW0213M21)
    elementPosition.co2Y = 16 + 6;
#endif
#if defined(EINKBOARDEINKBOARD_WEACT_DEPG0213BN)
    ;
#endif
#if defined(EINKBOARDGDEH0154D67)
    elementPosition.co2Y = 16 + 4;                                                                        // Center text in screen. 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2XUnits = ((display.width() / 4) + (elementPosition.bifFontDigitsHeight / 2) - 8);  // Display is rotated so vertical orientation (swaped width & height)
#endif
    display.setFont(&BigFont);
    display.getTextBounds("0000", 0, 0, &tbx, &tby, &tbw, &tbh);  // Set elementPosition.bifFontDigitsHeight to tbh and elementPosition.co2W to tbw for static assignment
    elementPosition.bifFontDigitsHeight = tbh;
    display.setFont(&SmallFont);
    display.getTextBounds("0000", 0, 0, &tbx, &tby, &tbw, &tbh);
    elementPosition.smallFontDigitsHeight = tbh;
}

bool showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw);
bool showCO2(uint16_t co2, int32_t posX, int32_t posY, bool forceRedraw);
bool showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw);
bool showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw);
bool showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw);
bool showWiFiIcon(int32_t posX, int32_t posY, bool forceRedraw);
bool showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw);
bool showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw);

void turnOffDisplay() {
    // Ignore for eInk displays
}

void displaySleep(bool value = true)  // https://github.com/Bodmer/TFT_eSPI/issues/715
{
    display.hibernate();  // TODO: Investigate display.hibernate() vs display.powerOff(). Check if this is the correct way to turn off the display. Specially for GDEM029T94
    // display.powerOff();
    if (value) {
        display.powerOff();  // Send command to put the display to sleep.
        delay(10);           // Delay for shutdown time before another command can be sent.
    }
}

void drawTextAligned(int16_t x, int16_t y, int16_t w, int16_t h, const String text, char h_align = 'C', char v_align = 'C') {
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    uint16_t pos_x, pos_y;
    // Calculate text bounds
    display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);

    switch (h_align) {
        case 'r':  // right
            pos_x = x + w - tbw;
            break;
        case 'c':  // centered
            pos_x = x + (w / 2) - (tbw / 2);
            break;
        case 'l':  // left
        default:
            pos_x = x;
            break;
    }
    switch (v_align) {
        case 'd':  // down
            pos_y = y + h;
            break;
        case 'c':  // centered
            pos_y = y + (h / 2) + (tbh / 2);
            break;
        case 'u':  // upper
        default:
            pos_y = y + tbh;
            break;
    }
    // Display text
    display.setCursor(pos_x, pos_y);
    display.print(text);
}

void displaySplashScreenLOGO() {
#ifdef TIMEDEBUG
    timer.start();
#endif
    display.setFullWindow();
    display.firstPage();
    do {
        // Draw bitmap
        display.fillScreen(GxEPD_WHITE);
        //        display.drawInvertedBitmap((display.width() - 250) / 2, (display.height() - 128) / 2, Logo250x128, 250, 128, GxEPD_BLACK);
    } while (display.nextPage());
#ifdef TIMEDEBUG
    Serial.print("time used to displaySplashScreenLOGO: ");
    Serial.println(timer.read());
#endif
}

void displaySplashScreen(bool fullRefresh = false) {
    uint16_t eMarieteLogoWidth = 250;
    uint16_t eMarieteLogoHeight = 128;
    uint16_t eMarieteLogoX = (display.width() - 250) / 2;
    uint16_t eMarieteLogoY = (display.height() - 128) / 2;
    if (fullRefresh) {
        display.setFullWindow();  // Activate full screen refresh
        display.firstPage();      // Clear screen
        do {
            // Draw Logo250x128 bitmap at the center of the screen
            display.fillScreen(GxEPD_WHITE);
            display.drawInvertedBitmap(eMarieteLogoX + 4, eMarieteLogoY, Logo250x128, eMarieteLogoWidth, eMarieteLogoHeight, GxEPD_BLACK);
            // drawTextAligned(0, 0, display.width(), display.height(), "eMariete CO2 Gadget Monitor", 'c', 'c');
        } while (display.nextPage());  // Do full refresh
    } else {
        display.setPartialWindow(0, 0, display.width(), display.height());  // Enable partial refresh
        display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
        display.drawInvertedBitmap(eMarieteLogoX + 4, eMarieteLogoY, Logo250x128, eMarieteLogoWidth, eMarieteLogoHeight, GxEPD_BLACK);
        // drawTextAligned(0, 0, display.width(), display.height(), "eMariete CO2 Gadget Monitor", 'c', 'c');
        display.displayWindow(0, 0, display.width(), display.height());  // Partial update
    }
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

// busyCallback function called during waiting for BUSY to end, to light sleep or service other tasks
void busyCallback(const void* p) {
#ifdef DEBUG_EINK
    // Serial.println("Serial.println("[EINK] busyCallback light sleep");
#endif
    esp_sleep_enable_timer_wakeup(0.2 * 1000000);  // 0.2 seconds
    Serial.flush();
#ifdef TIMEDEBUG
    timerLightSleep.resume();
#endif
    esp_light_sleep_start();
#ifdef TIMEDEBUG
    timerLightSleep.pause();
#endif
}

void busyHighPerformanceCallback(const void* p) {
#ifdef DEBUG_EINK
    // Serial.println("[EINK] busyHighPerformanceCallback light sleep");
#endif
    menuLoop();
}

void initDisplayFromDeepSleep(bool forceRedraw = false) {
    RTC_DATA_ATTR static bool firstBoot = true;
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.epd2.setBusyCallback(busyCallback);  // register callback to be called during BUSY active time
    setElementLocations();
    if (firstBoot) {
        forceRedraw = true;
        display.init(115200, true, 2, false);
        firstBoot = false;
    } else {
        display.init(115200, false, 2, false);
    }

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    // display.setTextColor(GxEPD_BLACK);
    // display.setFullWindow();
    // display.setPartialWindow(0, 0, display.width(), display.height());
#ifdef DEBUG_EINK
    // Serial.println("-->[EINK] Width: " + String(display.width()) + ", Height: " + String(display.height()));
#endif

    // Each cyclesToRedrawDisplay boots do a full screen refresh
    if (forceRedraw) {
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] Initializing display from deep sleep with full refresh from: ");
        Serial.println(__func__);
#endif
        display.fillScreen(GxEPD_WHITE);
        display.display();
        //        deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;
    } else {
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] Initializing display from deep sleep with partial refresh from: ");
        Serial.println(__func__);
#endif
        // display.setPartialWindow(0, 0, display.width(), display.height());
        // display.fillRect(20, 45, display.width() - 40, display.height() - 40, GxEPD_WHITE);
        // display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
        // display.displayWindow(0, 0, display.width(), display.height());
    }
}

void initDisplay(bool fastMode = false) {
#ifdef DEBUG_EINK
    if (fastMode) {
        Serial.print("-->[EINK] Initializing display in fast mode from: ");
        Serial.println(__func__);
    } else {
        Serial.print("-->[EINK] Initializing display in normal mode from: ");
        Serial.println(__func__);
    }
#endif
    display.epd2.setBusyCallback(busyHighPerformanceCallback);  // register callback to be called during BUSY active time (attend menu)
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    // display.init(115200, true, 2, false);
    display.init(115200, !fastMode, 2, false);

    //    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    // display.setFullWindow();
    display.setPartialWindow(0, 0, display.width(), display.height());
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Display hasPartialUpdate " + String(display.epd2.hasPartialUpdate));
    Serial.println("-->[EINK] Display hasFastPartialUpdate " + String(display.epd2.hasFastPartialUpdate));
#endif

    setElementLocations();

    displaySplashScreen();
    delay(2000);  // Enjoy the splash screen 2 seconds
}

bool showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static int16_t oldBatteryValue = -1;
    int16_t newBatteryValue;
    display.fillRect(posX, posY, 32, 16, GxEPD_WHITE);  // clear previous icon

    publishMQTTLogData("-->[EINK] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V");
    // Serial.println("-->[EINK] Drawn battery icon at " + String(posX) + ", " + String(posY) + " with level " + String(batteryLevel) + "% and voltage " + String(batteryVoltage) + "V");
    if (workingOnExternalPower) {
        newBatteryValue = 0;
        // display.drawRoundRect(posX + 8, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        // Serial.println("-->[EINK] Drawn round rect at " + String(posX + 8) + ", " + String(posY) + " with size 16x16");
        //    display.setSwapBytes(true);
        display.drawBitmap(posX + 16, posY, iconUSB, 16, 16, GxEPD_WHITE, GxEPD_BLACK);
        // Serial.println("-->[EINK] Drawn USB icon at " + String(posX + 12) + ", " + String(posY) + " with size 16x16");
    } else {
        display.fillRect(posX, posY + 4, 2, 6, GxEPD_BLACK);
        display.drawRoundRect(posX + 2, posY, 27, 14, 3, GxEPD_BLACK);  // Battery outter rectangle
        // Serial.println("-->[EINK] Drawn round rect at " + String(posX + 2) + ", " + String(posY) + " with size 27x14");
        newBatteryValue = 10;
        if (batteryLevel > 20) {
            newBatteryValue = 20;
            display.fillRect(posX + 6, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 8) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 40) {
            newBatteryValue = 40;
            display.fillRect(posX + 11, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 14) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 60) {
            newBatteryValue = 60;
            display.fillRect(posX + 16, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 20) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 80) {
            newBatteryValue = 80;
            display.fillRect(posX + 21, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 26) + ", " + String(posY + 2) + " with size 4x10");
        }
    }
    if (oldBatteryValue == newBatteryValue)
        return false;
    else
        oldBatteryValue = newBatteryValue;
    return true;
}

bool showCO2(uint16_t co2, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    if (!forceRedraw && (co2 <= (oldCO2Value + co2_HYSTERESIS)) && (co2 >= (oldCO2Value - co2_HYSTERESIS))) return false;  // don't update screen CO2 value
    if ((co2 == 0) || (co2 > 9999)) return false;

    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.fillRect(posX, posY, display.width(), elementPosition.co2H, GxEPD_WHITE);  // clear previous values

    //    display.fillRoundRect(0, elementPosition.co2Y, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_WHITE);  // 10 = 2px for top and bottom rectangle borders + 8px for top and bottom margin
    display.drawRoundRect(0, elementPosition.co2Y, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_BLACK);

    display.setRotation(4);
    display.setFont(&SmallFont);
    drawTextAligned(elementPosition.co2XUnits, elementPosition.co2YUnits, elementPosition.co2H - 5, elementPosition.smallFontDigitsHeight + 3, "PPM", 'c', 'b');

    display.setRotation(1);
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);
    drawTextAligned(elementPosition.co2X, elementPosition.co2Y + 2, elementPosition.co2W, elementPosition.bifFontDigitsHeight, String(co2), 'c', 'b');
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] CO2 value width: " + String(elementPosition.co2W) + " and height: " + String(elementPosition.co2H) + " in: " + __func__);
#endif
    oldCO2Value = co2;
    return true;
}

void showCO2OLD(uint16_t co2, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    if (!forceRedraw && (co2 == oldCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;

    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());

    // // Erase old CO2 value
    // display.fillRect(elementPosition.co2X, elementPosition.co2Y, elementPosition.co2W, elementPosition.bifFontDigitsHeight, GxEPD_WHITE);  // Clear previous co2 value

    display.fillRoundRect(0, elementPosition.bleIconY + 16 + 4, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_WHITE);
    display.drawRoundRect(0, elementPosition.bleIconY + 16 + 4, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_BLACK);

    display.setRotation(4);
    display.setFont(&SmallFont);
    drawTextAligned(elementPosition.co2XUnits, elementPosition.co2YUnits, elementPosition.co2H, 16, "PPM", 'c', 'b');

    // Show new CO2 value
    display.setRotation(1);
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);
    // #ifdef DEBUG_EINK
    display.getTextBounds("0000", 0, 0, &tbx, &tby, &tbw, &tbh);  // Set elementPosition.bifFontDigitsHeight to tbh and elementPosition.co2W to tbw for static assignment
    Serial.println("-->[EINK] CO2 value width: " + String(elementPosition.co2W) + " and height: " + String(elementPosition.co2H) + " Text bound x: " + String(tbw) + " y: " + String(tbh) + " w: " + String(tbw) + " h: " + String(tbh) + " in: " + __func__ + "()");
    // #endif
    drawTextAligned(elementPosition.co2X, elementPosition.co2Y + 5, elementPosition.co2W, elementPosition.bifFontDigitsHeight, String(co2), 'c', 'b');
    oldCO2Value = co2;
}

bool showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldHumiValue = -200;
    String newHumidityStr, oldHumidityStr;

    if ((!forceRedraw && round(hum) == oldHumiValue) || hum == 0 || !displayShowHumidity)
        return false;
    else
        oldHumiValue = round(hum);
    String humidityStr;
    if (!forceRedraw && (hum == oldHumiValue)) return false;
    if ((hum == 0) && (temp == 0)) return false;
    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.drawBitmap(elementPosition.humidityXIcon, elementPosition.humidityYUnits, iconHumidityBW, 16, 16, GxEPD_BLACK);
    display.fillRect(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, GxEPD_WHITE);  // Clear previous humidity value
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    newHumidityStr = String(int(round(hum))) + "%";
    drawTextAligned(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, newHumidityStr, 'l', 'c');
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn humidity value: " + newHumidityStr + " at: " + String(posX) + ", " + String(posY) + " with width: " + String(elementPosition.humidityWValue) + " and height: " + String(elementPosition.humidityHValue) + " in: " + __func__);
#endif
    return true;
}

bool showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldTempValue = -200;
    String tempStr;
    if ((!forceRedraw && round(temp * 10) == oldTempValue) || ((temp == 0) && (hum == 0)) || !displayShowTemperature)
        return false;
    else
        oldTempValue = round(temp * 10);
    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.drawBitmap(elementPosition.tempXUnits, elementPosition.tempYUnits, iconTempBW, 16, 16, GxEPD_BLACK);
    display.fillRect(elementPosition.tempXValue, elementPosition.tempYValue, elementPosition.tempWValue, elementPosition.tempHValue, GxEPD_WHITE);  // Clear previous temperature value
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    if (showFahrenheit) {
        tempStr = String(tempFahrenheit, 1) + "F";
    } else {
        tempStr = String(temp, 1) + "C";
    }
    drawTextAligned(elementPosition.tempXValue, elementPosition.tempYValue, elementPosition.tempWValue, elementPosition.tempHValue, tempStr, 'l', 'c');
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn temperature value: " + tempStr + " at: " + String(posX) + ", " + String(posY) + " with width: " + String(elementPosition.humidityWValue) + " and height: " + String(elementPosition.humidityHValue) + " in: " + __func__);
#endif
    return true;
}

bool showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static bool oldBLEstatus = false;
    //    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
    //    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (activeBLE == oldBLEstatus && !forceRedraw)
        return false;
    else
        oldBLEstatus = activeBLE;
    if (activeBLE) {
        display.drawBitmap(posX, posY, iconBluetoothBW, 16, 16, GxEPD_BLACK);
    } else {
        // if it's not active I think is better not to display it.
        display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
        // display.drawInvertedBitmap(posX, posY, iconBluetoothBW, 16, 16, GxEPD_BLACK);
    }
    return true;
}

bool showWiFiIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldWiFiState = -200;
    static float newWiFiState = 0;
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    int8_t rssi = WiFi.RSSI();
    if (troubledWIFI) {
        newWiFiState = -1;  // troubled
        display.drawBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
    } else {
        // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        if (!activeWIFI) {
            newWiFiState = -2;  // inactive
            // when is disabled I think is better show nothing but for debug purposes show it in inverse mode
            display.drawBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
        } else {
            if (WiFi.status() == WL_CONNECTED) {
                newWiFiState = 100;  // active signal strengh 80
                if (rssi < 60) {
                    newWiFiState = 60;  // active signal strengh 60
                    display.drawInvertedBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
                } else if (rssi < 70) {
                    newWiFiState = 70;  // active signal strengh 70
                    display.drawInvertedBitmap(posX, posY, iconWiFiMed, 16, 16, GxEPD_BLACK);
                } else if (rssi < 80) {
                    newWiFiState = 80;  // active signal strengh 80
                    display.drawInvertedBitmap(posX, posY, iconWiFiMed, 16, 16, GxEPD_BLACK);
                }
            } else {
                newWiFiState = 10;  // active signal strengh 80
                display.drawInvertedBitmap(posX, posY, iconWiFiLow, 16, 16, GxEPD_BLACK);
            }
        }
    }
    if (oldWiFiState == newWiFiState)
        return false;
    else
        oldWiFiState = newWiFiState;
    return true;
}

bool showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_MQTT
    RTC_DATA_ATTR static uint16_t oldMQTTState = -999;
    uint16_t newMQTTState;
    if (troubledMQTT)
        newMQTTState = -1;
    else
        newMQTTState = activeMQTT;

    if (oldMQTTState == newMQTTState)
        return false;
    else
        oldMQTTState = newMQTTState;
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    if (activeMQTT) {
        if (troubledMQTT) {
            display.drawBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
        } else {
            display.drawInvertedBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
        }
    }
    return true;
#endif
    return false;
}

bool showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_ESPNOW
    RTC_DATA_ATTR static uint16_t oldESPState = -999;
    uint16_t newESPState;
    if (troubledESPNOW)
        newESPState = -1;
    else
        newESPState = activeESPNOW;

    if (oldESPState == newESPState)
        return false;
    else
        oldESPState = newESPState;
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    if (activeESPNOW) {
        if (troubledESPNOW) {
            // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
            display.drawBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
            return;
        } else {
            display.drawInvertedBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
        }
        // // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        // if (!activeESPNOW) {
        //     // when is disabled I think is better show nothing but for debug purposes show it in inverse mode
        //     display.drawBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
        // } else {
        //     display.drawInvertedBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
        // }
    }
    return true;
#endif
    return false;
}

void testRedrawValues(bool randomNumbers = false) {
    static String textToDraw = "1234";
    static String textDrawn = "1234";
    static uint64_t lastTimeDrawn = 0;

    if (millis() - lastTimeDrawn < 5000) {
        return;
    }

    display.setPartialWindow(0, 0, display.width(), display.height());

    display.setTextColor(GxEPD_WHITE);
    display.setFont(&BigFont);
    display.setTextSize(1);
    display.setCursor(10, 100);
    display.print(textDrawn);
    display.setFont(&SmallFont);

    if (randomNumbers) textToDraw = String(random(400, 2000));
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&BigFont);
    display.setTextSize(1);
    display.setCursor(0, 100);
    display.print(textToDraw);
    display.setFont(&SmallFont);
    textDrawn = textToDraw;
    lastTimeDrawn = millis();
}

void displayShowValues(bool forceRedraw = false) {
    static uint16_t cyclesLeftToRedrawDisplay = 0;  // Cycles left to redraw display
    static uint32_t lastDisplayUpdate = 0;
    bool changeCo2 = false, changeTemp = false, changeHumidity = false, changeBatt = false, changeWiFi = false, changeMQTT = false, changeBLE = false, changeESP = false;
    // Return if last update less than timeBetweenEinkUpdates seconds ago
    if (!forceRedraw && ((millis() - lastDisplayUpdate) < (timeBetweenEinkUpdates * 1000))) {
        return;
    }
    lastDisplayUpdate = millis();

#ifdef TIMEDEBUG
    timer.start();
#endif

    if (cyclesLeftToRedrawDisplay > 0) {
        cyclesLeftToRedrawDisplay--;
        Serial.println("-->[EINK] Cycles left to full refresh of display: " + String(cyclesLeftToRedrawDisplay));
    } else {
        cyclesLeftToRedrawDisplay = redrawDisplayEveryCycles;
        Serial.println("-->[EINK] Forcing full refresh of display.");
        forceRedraw = true;
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);  // CLEAR ALL DISPLAY
    }

    // testRedrawValues(true);
    changeCo2 = showCO2(co2, elementPosition.co2X, elementPosition.co2Y, forceRedraw);
    changeTemp = showTemperature(temp, elementPosition.tempXValue, elementPosition.tempYValue, forceRedraw);
    changeHumidity = showHumidity(hum, elementPosition.humidityXValue, elementPosition.humidityYValue, forceRedraw);
    changeBatt = showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, forceRedraw);
    // showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    changeWiFi = showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    changeMQTT = showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    changeBLE = showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    changeESP = showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    // display.hibernate();

    if (!changeCo2 && !changeTemp && !changeHumidity && !changeBatt && !changeWiFi && !changeMQTT && !changeBLE && !changeESP) {
        // IS NOT NECESSARY TO REFRESH DISPLAY
        cyclesLeftToRedrawDisplay++;
        Serial.println("-->[EINK] No changes -> no refresh");
    } else {
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] changeCo2 = " + String(changeCo2));
        Serial.print(" - changeTemp = " + String(changeTemp));
        Serial.print(" - changeHumidity = " + String(changeHumidity));
        Serial.print(" - changeBatt = " + String(changeBatt));
        Serial.print(" - changeWiFi = " + String(changeWiFi));
        Serial.print(" - changeMQTT = " + String(changeMQTT));
        Serial.print(" - changeBLE = " + String(changeBLE));
        Serial.println(" - changeESP = " + String(changeESP));
#endif

        if (forceRedraw) {  // when CO2 changes for the first time update full refresh
#ifdef DEBUG_EINK
            Serial.println("-->[EINK] Full refresh");
#endif
            display.setFullWindow();  // Enable full refresh
            display.display();
        } else {
#ifdef DEBUG_EINK
            Serial.println("-->[EINK] Partial refresh. Full in : " + String(cyclesLeftToRedrawDisplay));
#endif

            if (!changeCo2 && !changeBatt && !changeBLE && !changeWiFi && !changeMQTT && !changeESP) {
                // ONLY CHANGE TEMP OR HUM.
                if (!changeTemp) {  // only change hum
#ifdef DEBUG_EINK
                    Serial.println("-->[EINK] Refresh Humidity box: x1: " + String(elementPosition.humidityXValue) + ", y1: " + String(elementPosition.humidityYValue) + ", x2: " + String(elementPosition.humidityXValue + elementPosition.humidityWValue) + ", y2: " + String(elementPosition.humidityYValue + elementPosition.humidityHValue));
#endif
                    display.setPartialWindow(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityXValue + elementPosition.humidityWValue, elementPosition.humidityYValue + elementPosition.humidityHValue);
                    display.displayWindow(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityXValue + elementPosition.humidityWValue, elementPosition.humidityYValue + elementPosition.humidityHValue);
                } else if (!changeHumidity) {  // only change temp
#ifdef DEBUG_EINK
                    Serial.println("-->[EINK] Refresh Temp box: x1: " + String(elementPosition.tempXValue) + ", y1: " + String(elementPosition.tempYValue) + ", x2: " + String(elementPosition.tempXValue + elementPosition.tempWValue) + ", y2: " + String(elementPosition.tempYValue + elementPosition.tempHValue));
#endif
                    display.setPartialWindow(elementPosition.tempXValue, elementPosition.tempYValue, elementPosition.tempXValue + elementPosition.tempWValue, elementPosition.tempYValue + elementPosition.tempHValue);
                    display.displayWindow(elementPosition.tempXValue, elementPosition.tempYValue, elementPosition.tempXValue + elementPosition.tempWValue, elementPosition.tempYValue + elementPosition.tempHValue);
                } else {  // only change temp & hum
#ifdef DEBUG_EINK
                    Serial.println("-->[EINK] Refresh Temp + Humidity box: x1: 0, y1: " + String(elementPosition.tempYValue) + ", x2: " + String(display.width()) + ", y2: " + String(display.height()));
#endif
                    display.setPartialWindow(0, elementPosition.tempYValue, display.width(), display.height());
                    display.displayWindow(0, elementPosition.tempYValue, display.width(), display.height());
                }
            } else {
                if (changeCo2 || ((changeBLE || changeWiFi || changeMQTT || changeESP || changeBatt) && (changeTemp || changeHumidity))) {
#ifdef DEBUG_EINK
                    Serial.println("-->[EINK] Refresh CO2 - Full display");
#endif
                    display.setPartialWindow(0, 0, display.width(), display.height());
                    display.displayWindow(0, 0, display.width(), display.height());
                } else {
                    if (changeBLE || changeWiFi || changeMQTT || changeESP || changeBatt) {
#ifdef DEBUG_EINK
                        Serial.println("-->[EINK] Refresh Icons + Battery box: x1: 0, y1: 0, x2: " + String(display.width()) + ", y2: 16");
#endif
                        display.setPartialWindow(0, 0, display.width(), 16);
                        display.displayWindow(0, 0, display.width(), 16);
                    } else {
#ifdef DEBUG_EINK
                        Serial.println("-->[EINK] Refresh Temp + Humidity box: x1: 0, y1: " + String(elementPosition.tempYValue) + ", x2: " + String(display.width()) + ", y2: " + String(display.height()));
#endif
                        display.setPartialWindow(0, elementPosition.tempYValue, display.width(), display.height());
                        display.displayWindow(0, elementPosition.tempYValue, display.width(), display.height());
                    }
                }
            }
        }
        // display.hibernate();
    }
#ifdef TIMEDEBUG
    uint32_t elapsed = timer.read();
    if (elapsed > 10) {
        Serial.println("-->[EINK] Time used to showValues: " + String(elapsed));
    }
#endif
}

#endif  // SUPPORT_EINK
#endif  // CO2_Gadget_EINK_h
