// https://rop.nl/truetype2gfx/ to convert fonts to .h files
// https://oleddisplay.squix.ch/#/home
// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

#ifndef CO2_Gadget_EINK_h
#define CO2_Gadget_EINK_h

#ifdef SUPPORT_EINK
// #define DEBUG_EINK

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP EINK DISPLAY FUNCTIONALITY                         *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <GxEPD2_BW.h>

#ifndef RESETDURATION
#define RESETDURATION 2 // Default reset duration in seconds for WaveShare displays with clever reset circuit
#endif

uint16_t redrawDisplayEveryCycles = 10;  // Redraw display every X partial updates
uint16_t cyclesLeftToRedrawDisplay = 0;  // Cycles left to redraw display

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
#include <Digits_NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#ifdef EINKBOARDGDEM0213B74
#include <Digits_NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM0213B74 https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#ifdef EINKBOARDGDEW0213M21
#include <Digits_NotoSans_Bold38pt7b.h>
#include <NotoSans_Bold6pt7b.h>
const GFXfont SmallFont = NotoSans_Bold6pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold38pt7b;
int displayWidth = 212;
int displayHeight = 104;
// GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex ::HEIGHT> display(GxEPD2_213_flex(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
// GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D ::HEIGHT> display(GxEPD2_213_T5D(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21 ::HEIGHT> display(GxEPD2_213_M21(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW0213M21 104x212, SSD1608 (GDEW0213Z16LW) https://s.click.aliexpress.com/e/_DDFb2gl
#endif
#endif

#ifdef EINKBOARDGDEM029T94
#include <Digits_NotoSans_Bold48pt7b.h>
#include <NotoSans_Bold6pt7b.h>
const GFXfont SmallFont = NotoSans_Bold6pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold48pt7b;
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

#ifdef EINKBOARD_WEACT_DEPG0213BN
#include <Digits_NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_DDvVZ4N
#endif

#ifdef EINKBOARD_WEACT_GDEH0154D67
#include <Digits_NotoSans_Bold38pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold38pt7b;
int displayWidth = 200;
int displayHeight = 200;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEH0154D67 200x200, SSD1681 https://s.click.aliexpress.com/e/_DDvVZ4N
#endif

#ifdef EINKBOARD_WEACT_GxEPD2_290_BS
#include <Digits_NotoSans_Bold48pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = Digits_NotoSans_Bold48pt7b;
int displayWidth = 200;
int displayHeight = 200;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029C90 As WeAct Studio 2.9" 296x128 E-Ink Display
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

    // Get font sizes for text bounds
    display.setFont(&BigFont);
    display.getTextBounds("0000", 0, 0, &tbx, &tby, &tbw, &tbh);  // Set elementPosition.bifFontDigitsHeight to tbh and elementPosition.co2W to tbw for static assignment
    elementPosition.bifFontDigitsHeight = tbh;
    display.setFont(&SmallFont);
    display.getTextBounds("0000", 0, 0, &tbx, &tby, &tbw, &tbh);
    elementPosition.smallFontDigitsHeight = tbh;

    // Common positions for most displays
    elementPosition.co2X = 2;       // Left screen
    elementPosition.co2Y = 16 + 8;  // Center text in screen. 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2W = display.width() - 16;
    elementPosition.co2H = (display.height() - 32);  // 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2XUnits = 16;                  // Display is rotated so vertical orientation (swaped width & height)
    elementPosition.co2YUnits = display.width() - elementPosition.smallFontDigitsHeight - 4;

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
#if defined(EINKBOARD_WEACT_GDEH0154D67)
    elementPosition.co2Y = 16 + 4;                                                                        // Center text in screen. 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2XUnits = ((display.width() / 4) + (elementPosition.bifFontDigitsHeight / 2) - 8);  // Display is rotated so vertical orientation (swaped width & height)
#endif
#if defined(EINKBOARD_WEACT_GxEPD2_290_BS)
    ;
#endif
}

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

// Function to set the display rotation
void setDisplayRotation(int rotation) {
    if (rotation == 1) {
        display.setRotation(displayReverse ? 3 : 1);
    } else if (rotation == 4) {
        display.setRotation(displayReverse ? 2 : 4);
    } else {
        display.setRotation(rotation); // Default behavior for other values
    }
    // shouldRedrawDisplay = true;
}

/**
 * Draws text on the display with alignment options.
 *
 * @param x The x-coordinate of the top-left corner of the bounding box.
 * @param y The y-coordinate of the top-left corner of the bounding box.
 * @param w The width of the bounding box.
 * @param h The height of the bounding box.
 * @param text The text to be displayed.
 * @param h_align The horizontal alignment of the text. Options: 'l' (left), 'c' (centered), 'r' (right). Default: 'c'.
 * @param v_align The vertical alignment of the text. Options: 'u' (upper), 'c' (centered), 'd' (down). Default: 'c'.
 */
void drawTextAligned(int16_t x, int16_t y, int16_t w, int16_t h, const String text, char h_align = 'c', char v_align = 'c') {
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

void displaySplashScreen(bool fullRefresh = false) {
    uint16_t LogoWidth = 180;
    uint16_t LogoHeight = 45;
    uint16_t LogoX = (display.width() - LogoWidth) / 2;
    uint16_t LogoY = (display.height() - LogoHeight) / 2;
    if (fullRefresh) {
        display.setFullWindow();  // Activate full screen refresh
        display.firstPage();      // Clear screen
        do {
            display.fillScreen(GxEPD_WHITE);
            display.drawInvertedBitmap(LogoX + 4, LogoY, Logo180x45, LogoWidth, LogoHeight, GxEPD_BLACK);
        } while (display.nextPage());  // Do full refresh
    } else {
        display.setPartialWindow(0, 0, display.width(), display.height());  // Enable partial refresh
        display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
        display.drawInvertedBitmap(LogoX + 4, LogoY, Logo180x45, LogoWidth, LogoHeight, GxEPD_BLACK);
#ifdef DEBUG_EINK
        drawTextAligned(0, 0, display.width(), elementPosition.smallFontDigitsHeight + 8, "v" + String(CO2_GADGET_VERSION) + "." + String(CO2_GADGET_REV), 'c', 'u');
        Serial.println("-->[EINK] Text drawn at: " + String(0) + ", " + String(LogoY + LogoHeight / 2 + 8) + " with width: " + String(display.width()) + " and height: " + String(elementPosition.smallFontDigitsHeight + 8) + " in: " + __func__ + "()");
        drawTextAligned(0, elementPosition.smallFontDigitsHeight + 8, display.width(), elementPosition.smallFontDigitsHeight + 8, String(FLAVOUR), 'c', 'u');
        Serial.println("-->[EINK] Text drawn at: " + String(0) + ", " + String(LogoY - LogoHeight / 2 - 8) + " with width: " + String(display.width()) + " and height: " + String(elementPosition.smallFontDigitsHeight + 8) + " in: " + __func__ + "()");
#endif
        drawTextAligned(0, display.height() - elementPosition.smallFontDigitsHeight - 8, display.width(), elementPosition.smallFontDigitsHeight + 8, String("https://emariete.com/co2"), 'c', 'u');
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

// busyCallbackDeepSleep function called during waiting for BUSY to end, to light sleep or service other tasks
void busyCallbackDeepSleep(const void* p) {
#ifdef DEBUG_EINK
    // Serial.println("Serial.println("[EINK] busyCallbackDeepSleep light sleep");
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

void busyCallbackHighPerformance(const void* p) {
#ifdef DEBUG_EINK
    // Serial.println("[EINK] busyCallbackHighPerformance light sleep");
#endif
    menuLoop();
}

void setDisplayReverse(bool reverse) {
    if (reverse) {
        Serial.printf("-->[EINK] Set display reverse\n");
        setDisplayRotation(3);
    } else {
        Serial.printf("-->[EINK] Set display normal\n");
        setDisplayRotation(1);
    }
    shouldRedrawDisplay = true;
}

void initDisplayFromDeepSleep(bool forceRedraw = false) {
    RTC_DATA_ATTR static bool firstBoot = true;
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.epd2.setBusyCallback(busyCallbackDeepSleep);  // register callback to be called during BUSY active time
    setElementLocations();
    if (firstBoot) {
        forceRedraw = true;
        display.init(115200, true, RESETDURATION, false);
        firstBoot = false;
    } else {
        display.init(115200, false, RESETDURATION, false);
    }

    // Set default options to draw
    setDisplayRotation(1);
    // display.setRotation(1);
    display.setFont(&SmallFont);
    // display.setTextColor(GxEPD_BLACK);
    // display.setFullWindow();
    // display.setPartialWindow(0, 0, display.width(), display.height());
#ifdef DEBUG_EINK
    // Serial.println("-->[EINK] Width: " + String(display.width()) + ", Height: " + String(display.height()));
#endif

    // Each deepSleepData.redrawDisplayEveryCycles boots do a full screen refresh
    if (forceRedraw) {
        // display.fillScreen(GxEPD_WHITE);
        // display.display();
        // drawMainScreen(false);
        // deepSleepData.cyclesLeftToRedrawDisplay = deepSleepData.redrawDisplayEveryCycles;
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] Initializing display from deep sleep with full refresh from: ");
        Serial.println(__func__);
#endif
    } else {
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] Initializing display from deep sleep with partial refresh from: ");
        Serial.println(__func__);
#endif
        // display.setPartialWindow(0, 0, display.width(), display.height());
        // display.fillRect(20, 45, display.width() - 40, display.height() - 40, GxEPD_WHITE);
        // display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
        // display.displayWindow(0, 0, display.width(), display.height());
        // drawMainScreen(false);
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
    display.epd2.setBusyCallback(busyCallbackHighPerformance);  // register callback to be called during BUSY active time (attend menu)
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);

    // display.init(115200, true, RESETDURATION, false);
    display.init(115200, !fastMode, RESETDURATION, false);

#ifdef DEBUG_EINK
    int a = SCK, b = EPD_MISO, c = MOSI, d = SS, e = EPD_DC, f = EPD_RST, g = EPD_BUSY;
    Serial.println("-->[EINK] SPI pins: SCK: " + String(a) + ", MISO: " + String(b) + ", MOSI: " + String(c) + ", SS: " + String(d) + ", DC: " + String(e) + ", RST: " + String(f) + ", BUSY: " + String(g));
    Serial.println("-->[EINK] Display hasPartialUpdate " + String(display.epd2.hasPartialUpdate));
    Serial.println("-->[EINK] Display hasFastPartialUpdate " + String(display.epd2.hasFastPartialUpdate));
#endif

    // Set default options to draw
    setDisplayRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    // display.setFullWindow();
    // display.setPartialWindow(0, 0, display.width(), display.height());
    setElementLocations();
    displaySplashScreen();
}

void showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    publishMQTTLogData("-->[EINK] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V");
    display.fillRect(posX, posY, display.width() - posX, 16, GxEPD_WHITE);
    if (!displayShowBattery) return;
    if (workingOnExternalPower) {
        // display.drawRoundRect(posX + 8, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX + 16, posY, iconUSB, 16, 16, GxEPD_WHITE, GxEPD_BLACK);
    } else {
        display.fillRect(posX, posY + 4, 2, 6, GxEPD_BLACK);
        display.drawRoundRect(posX + 2, posY, 27, 14, 3, GxEPD_BLACK);  // Battery outter rectangle
        if (batteryLevel > 80) {
            display.fillRect(posX + 6, posY + 2, 4, 10, GxEPD_BLACK);
        }
        if (batteryLevel > 60) {
            display.fillRect(posX + 11, posY + 2, 4, 10, GxEPD_BLACK);
        }
        if (batteryLevel > 40) {
            display.fillRect(posX + 16, posY + 2, 4, 10, GxEPD_BLACK);
        }
        if (batteryLevel > 20) {
            display.fillRect(posX + 21, posY + 2, 4, 10, GxEPD_BLACK);
        }
    }
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    if (!forceRedraw && (co2 == oldCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;

    setDisplayRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());

    display.fillRoundRect(0, elementPosition.co2Y, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_WHITE);  // 10 = 2px for top and bottom rectangle borders + 8px for top and bottom margin
    display.drawRoundRect(0, elementPosition.co2Y, display.width(), elementPosition.bifFontDigitsHeight + 10, 6, GxEPD_BLACK);

    setDisplayRotation(4);
    display.setFont(&SmallFont);
    drawTextAligned(elementPosition.co2XUnits, elementPosition.co2YUnits, elementPosition.co2H - 5, elementPosition.smallFontDigitsHeight + 3, "PPM", 'c', 'b');

    setDisplayRotation(1);
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);
    drawTextAligned(elementPosition.co2X, elementPosition.co2Y + 2, elementPosition.co2W, elementPosition.bifFontDigitsHeight, String(co2), 'c', 'b');
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] CO2 value width: " + String(elementPosition.co2W) + " and height: " + String(elementPosition.co2H) + " in: " + __func__);
#endif
    oldCO2Value = co2;
}

void showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldHumiValue = -200;
    if (!displayShowHumidity) return;
    String humidityStr;
    if (!forceRedraw && (hum == oldHumiValue)) return;
    if ((hum == 0) && (temp == 0)) return;
    setDisplayRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.drawBitmap(elementPosition.humidityXIcon, elementPosition.humidityYUnits, iconHumidityBW, 16, 16, GxEPD_BLACK);
    display.fillRect(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, GxEPD_WHITE);  // Clear previous humidity value
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    humidityStr = String(int(hum)) + "%";
    drawTextAligned(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, humidityStr, 'l', 'c');
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn humidity value: " + humidityStr + " at: " + String(posX) + ", " + String(posY) + " with width: " + String(elementPosition.humidityWValue) + " and height: " + String(elementPosition.humidityHValue) + " in: " + __func__);
#endif
    oldHumiValue = hum;
}

void showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldTempValue = -200;
    if (!displayShowTemperature) return;
    String tempStr;
    if (!forceRedraw && (temp == oldTempValue)) return;
    if ((temp == 0) && (hum == 0)) return;
    setDisplayRotation(1);
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
    oldTempValue = temp;
}

void showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    //    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
    //    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (activeBLE) {
        display.drawBitmap(posX, posY, iconBluetoothBW, 16, 16, GxEPD_BLACK);
    } else {
        // if it's not active I think is better not to display it.
        display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
        // display.drawInvertedBitmap(posX, posY, iconBluetoothBW, 16, 16, GxEPD_BLACK);
    }
}

void showWiFiIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
#ifdef SUPPORT_CAPTIVE_PORTAL
    // If captivePortalActive = true; draw a filled circle instead of the WiFi icon.  If forceCaptivePortalActive is also true, draw a non filled circle
    if (captivePortalActive) {
        if (forceCaptivePortalActive) {
            display.drawCircle(posX + 8, posY + 8, 6, GxEPD_BLACK);
        } else {
            display.fillCircle(posX + 8, posY + 8, 6, GxEPD_BLACK);
        }
        return;
    }
#endif
    int8_t rssi = WiFi.RSSI();
    if (troubledWIFI) {
        display.drawBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
        return;
    }
    // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeWIFI) {
        // when is disabled I think is better show nothing but for debug purposes show it in inverse mode
        display.drawBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
    } else {
        if (WiFi.status() == WL_CONNECTED) {
            if (rssi < 60)
                display.drawInvertedBitmap(posX, posY, iconWiFi, 16, 16, GxEPD_BLACK);
            else if (rssi < 70)
                display.drawInvertedBitmap(posX, posY, iconWiFiMed, 16, 16, GxEPD_BLACK);
            else if (rssi < 80)
                display.drawInvertedBitmap(posX, posY, iconWiFiMed, 16, 16, GxEPD_BLACK);
        } else {
            display.drawInvertedBitmap(posX, posY, iconWiFiLow, 16, 16, GxEPD_BLACK);
        }
    }
}

void showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_MQTT
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    if (activeMQTT) {
        if (troubledMQTT) {
            display.drawBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
        } else {
            display.drawInvertedBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
        }
    }
#endif
}

void showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_ESPNOW
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
#endif
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
    lastTimeDrawn = millis();
}

#ifdef EINKBOARDGDEM0213B74
void displayShowValues(bool forceRedraw = false) {
    static uint32_t lastDisplayUpdate = 0;
    if (!thresholdsManager.evaluateThresholds(DISPLAY_SHOW, co2, temp, hum)) return;
    if (isDownloadingBLE) return;  // Do not update display while downloading BLE data to MyAmbiance
    if (redrawDisplayOnNextLoop) {
        shouldRedrawDisplay = true;
        redrawDisplayOnNextLoop = false;
    }
    if (shouldRedrawDisplay) {
        forceRedraw = true;
        shouldRedrawDisplay = false;
    }
    // Return if last update less than 15 seconds ago
    if (!forceRedraw && (millis() - lastDisplayUpdate < 10000)) {
        return;
    }

    if ((!forceRedraw) && (co2 == 0)) {
        return;
    }

    lastDisplayUpdate = millis();

#ifdef TIMEDEBUG
    timer.start();
#endif

    if (cyclesLeftToRedrawDisplay > 0) {
        cyclesLeftToRedrawDisplay--;
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Cycles left to full refresh of display: " + String(cyclesLeftToRedrawDisplay));
#endif
    } else {
        cyclesLeftToRedrawDisplay = redrawDisplayEveryCycles;
        forceRedraw = true;
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Forcing full refresh of display");
#endif
    }

    if (forceRedraw) {
        display.fillScreen(GxEPD_WHITE);
        display.clearScreen(GxEPD_WHITE);
    }
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, forceRedraw);
    showTemperature(temp, elementPosition.tempXValue, elementPosition.tempYValue, forceRedraw);
    showHumidity(hum, elementPosition.humidityXValue, elementPosition.humidityYValue, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, true);
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);

    display.display(true);  // Partial update

#ifdef TIMEDEBUG
    uint32_t elapsed = timer.read();
    if (elapsed > 10) {
        Serial.println("-->[EINK] Time used to showValues: " + String(elapsed));
    }
#endif
}

#else  // ALL OTHER NOT EINKBOARDGDEM0213B74 BOARDS

void displayShowValues(bool forceRedraw = false) {
    static uint32_t lastDisplayUpdate = 0;
    if (forceRedraw) {
        thresholdsManager.updatePreviousValues(DISPLAY_SHOW, co2, temp, hum);
    } else {
        if (!thresholdsManager.evaluateThresholds(DISPLAY_SHOW, co2, temp, hum)) return;
    }
    if (isDownloadingBLE) return;  // Do not update display while downloading BLE data to MyAmbiance
    if (redrawDisplayOnNextLoop) {
        shouldRedrawDisplay = true;
        redrawDisplayOnNextLoop = false;
    }
    if (shouldRedrawDisplay) {
        forceRedraw = true;
        shouldRedrawDisplay = false;
    }
    // Return if last update less than 15 seconds ago
    if (!forceRedraw && (millis() - lastDisplayUpdate < 15000)) {
        return;
    }

    if ((!forceRedraw) && (co2 == 0)) {
        return;
    }

    lastDisplayUpdate = millis();

#ifdef TIMEDEBUG
    timer.start();
#endif

    if (cyclesLeftToRedrawDisplay > 0) {
        cyclesLeftToRedrawDisplay--;
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Cycles left to full refresh of display: " + String(cyclesLeftToRedrawDisplay));
#endif
    } else {
        cyclesLeftToRedrawDisplay = redrawDisplayEveryCycles;
        forceRedraw = true;
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Forcing full refresh of display");
#endif
    }

    if (forceRedraw) {
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
    }

    // testRedrawValues(true);
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, forceRedraw);
    showTemperature(temp, elementPosition.tempXValue, elementPosition.tempYValue, forceRedraw);
    showHumidity(hum, elementPosition.humidityXValue, elementPosition.humidityYValue, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, true);
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    // display.hibernate();

    if (forceRedraw) {
        display.display();  // Full update
    } else {
        display.displayWindow(0, 0, display.width(), display.height());  // Refresh screen in partial mode
    }

#ifdef TIMEDEBUG
    uint32_t elapsed = timer.read();
    if (elapsed > 10) {
        Serial.println("-->[EINK] Time used to showValues: " + String(elapsed));
    }
#endif
}
#endif

#endif  // SUPPORT_EINK
#endif  // CO2_Gadget_EINK_h
