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

uint16_t redrawDisplayEveryCycles = 10;  // Redraw display every X partial updates
uint16_t cyclesLeftToRedrawDisplay = 0;  // Cycles left to redraw display

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEW0213M21)

#include "bootlogo.h"
#include "icons.h"
#define EPD_SCLK SCK
#define EPD_MISO 17
#define EPD_DC 17
#define EPD_MOSI MOSI
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4

#ifdef EINKBOARDDEPG0213BN
#include <NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif
#ifdef EINKBOARDGDEM0213B74
#include <NotoSans_Bold46pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold46pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM0213B74
#endif
#ifdef EINKBOARDGDEW0213M21
#include <NotoSans_Bold42pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold42pt7b;
int displayWidth = 212;
int displayHeight = 104;
// GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex ::HEIGHT> display(GxEPD2_213_flex(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
// GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D ::HEIGHT> display(GxEPD2_213_T5D(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21 ::HEIGHT> display(GxEPD2_213_M21(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW0213M21 104x212, SSD1608 (GDEW0213Z16LW)
#endif
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

#define EPD_SCLK SCK
#define EPD_MISO 17
#define EPD_MOSI MOSI
#define EPD_CS SS
#define EPD_DC 27
#define EPD_RST 25
#define EPD_BUSY 32
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94
// GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94 V2 GDEM029T94  128x296, SSD1680, (FPC-7519 rev.b), Waveshare 2.9" V2 variant
// GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEM029T94 BS
// GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEW029T5 with special pinout
#endif

#ifdef EINKBOARDGDEH0154D67
#include <NotoSans_Bold42pt7b.h>
#include <NotoSans_SemiCondensed_Bold10pt7b.h>
const GFXfont SmallFont = NotoSans_SemiCondensed_Bold10pt7b;
const GFXfont BigFont = NotoSans_Bold42pt7b;
int displayWidth = 200;
int displayHeight = 200;

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"

#define EPD_SCLK SCK
#define EPD_MISO 17
#define EPD_MOSI MOSI
#define EPD_CS SS
#define EPD_DC 27
#define EPD_RST 25
#define EPD_BUSY 32
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));  // GDEH0154D67 200x200, SSD1681
#endif

// Define a structure for the locations of elements
struct ElementLocations {
    int32_t co2X;
    int32_t co2Y;
    int32_t co2W;
    int32_t co2H;
    u_int16_t co2FontDigitsHeight;
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
    // Common positions for most displays
    elementPosition.co2X = 0;                                                       // Left screen
    elementPosition.co2Y = (display.height() / 2) - ((display.height() - 32) / 2);  // Center text in screen. 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2W = display.width() - 16;
    elementPosition.co2H = (display.height() - 32);  // 32 is the size of 16 + 16 pixels of upper and down icon lines
    elementPosition.co2XUnits = 16;                  // Display is rotated so vertical orientation (swaped width & height)
    elementPosition.co2YUnits = display.width() - 16;

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
    elementPosition.co2FontDigitsHeight = 48;
#endif
#if defined(EINKBOARDGDEW0213M21) || defined(EINKBOARDGDEH0154D67)
    elementPosition.co2FontDigitsHeight = 42;
#endif
#if defined(EINKBOARDGDEH0154D67)
    elementPosition.co2FontDigitsHeight = 42;
    elementPosition.co2Y = ((display.height() / 2) - ((display.height() - 32) / 2)) - (display.height() / 4);  // Center in upper half of display.
    elementPosition.co2XUnits = ((display.width() / 4) + (elementPosition.co2FontDigitsHeight / 2) - 8);       // Display is rotated so vertical orientation (swaped width & height)
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
    // Serial.println("eink busyCallback light sleep");
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
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    // display.init(115200, true, 2, false);
    display.init(115200, !fastMode, 2, false);

    //    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.setPartialWindow(0, 0, display.width(), display.height());
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Display hasPartialUpdate " + String(display.epd2.hasPartialUpdate));
    Serial.println("-->[EINK] Display hasFastPartialUpdate " + String(display.epd2.hasFastPartialUpdate));
#endif

    setElementLocations();

    displaySplashScreen();
    delay(2000);

    // DisplayInititialized = true;
    // display.hibernate();
}

void showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    publishMQTTLogData("-->[EINK] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V");
    // Serial.println("-->[EINK] Drawn battery icon at " + String(posX) + ", " + String(posY) + " with level " + String(batteryLevel) + "% and voltage " + String(batteryVoltage) + "V");
    if (workingOnExternalPower) {
        // display.drawRoundRect(posX + 8, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        // Serial.println("-->[EINK] Drawn round rect at " + String(posX + 8) + ", " + String(posY) + " with size 16x16");
        //    display.setSwapBytes(true);
        display.drawBitmap(posX + 16, posY, iconUSB, 16, 16, GxEPD_WHITE, GxEPD_BLACK);
        // Serial.println("-->[EINK] Drawn USB icon at " + String(posX + 12) + ", " + String(posY) + " with size 16x16");
    } else {
        display.fillRect(posX, posY + 4, 2, 6, GxEPD_BLACK);
        display.drawRoundRect(posX + 2, posY, 27, 14, 3, GxEPD_BLACK);  // Battery outter rectangle
        // Serial.println("-->[EINK] Drawn round rect at " + String(posX + 2) + ", " + String(posY) + " with size 27x14");
        if (batteryLevel > 20) {
            display.fillRect(posX + 6, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 8) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 40) {
            display.fillRect(posX + 11, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 14) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 60) {
            display.fillRect(posX + 16, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 20) + ", " + String(posY + 2) + " with size 4x10");
        }
        if (batteryLevel > 80) {
            display.fillRect(posX + 21, posY + 2, 4, 10, GxEPD_BLACK);
            // Serial.println("-->[EINK] Drawn rect at " + String(posX + 26) + ", " + String(posY + 2) + " with size 4x10");
        }
    }
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    if (!forceRedraw && (co2 == oldCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;

    display.setRotation(4);
    display.setFont(&SmallFont);
    drawTextAligned(elementPosition.co2XUnits, elementPosition.co2YUnits, elementPosition.co2H, 16, "PPM", 'c', 'c');

    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    // Erase old CO2 value
    display.fillRect(elementPosition.co2X, elementPosition.co2Y, elementPosition.co2W, elementPosition.co2H, GxEPD_WHITE);  // Clear previous co2 value

    // Show new CO2 value
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);
    drawTextAligned(elementPosition.co2X, elementPosition.co2Y, elementPosition.co2W, elementPosition.co2H, String(co2), 'c', 'c');

#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn CO2 value: " + String(co2) + " at: " + String(posX) + ", " + String(posY) + " in: " + __func__);
#endif
    oldCO2Value = co2;
}

void showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldHumiValue = -200;
    if (!displayShowHumidity) return;
    String humidityStr;
    if (!forceRedraw && (hum == oldHumiValue)) return;
    if ((hum == 0) && (temp == 0)) return;
    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.drawBitmap(elementPosition.humidityXIcon, elementPosition.humidityYUnits, iconHumidityBW, 16, 16, GxEPD_BLACK);
    display.fillRect(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, GxEPD_WHITE);  // Clear previous humidity value
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    humidityStr = String(int(hum)) + "%";
    drawTextAligned(elementPosition.humidityXValue, elementPosition.humidityYValue, elementPosition.humidityWValue, elementPosition.humidityHValue, humidityStr, 'l', 'c');
    Serial.println("-->[EINK] Drawn humidity value: " + humidityStr + " at: " + String(posX) + ", " + String(posY) + " with width: " + String(elementPosition.humidityWValue) + " and height: " + String(elementPosition.humidityHValue) + " in: " + __func__);
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
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    if (troubledMQTT) {
        // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
        return;
    }
    // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeMQTT) {
        // when is disabled I think is better show nothing but for debug purposes show it in inverse mode
        display.drawBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
    } else {
        display.drawInvertedBitmap(posX, posY, iconMQTT, 16, 16, GxEPD_BLACK);
    }
}

void showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_ESPNOW
    display.fillRect(posX, posY, 16, 16, GxEPD_WHITE);
    if (troubledESPNOW) {
        // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
        return;
    }
    // display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeESPNOW) {
        // when is disabled I think is better show nothing but for debug purposes show it in inverse mode
        display.drawBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
    } else {
        display.drawInvertedBitmap(posX, posY, iconEspNow, 16, 16, GxEPD_BLACK);
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
    display.setCursor(0, 100);
    display.print(textToDraw);
    display.setFont(&SmallFont);
    textDrawn = textToDraw;
    lastTimeDrawn = millis();
}

void displayShowValues(bool forceRedraw = false) {
    static uint32_t lastDisplayUpdate = 0;
    // Resurn if last update less than 15 seconds ago
    if (!forceRedraw && (millis() - lastDisplayUpdate < 15000)) {
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
        forceRedraw = true;
        Serial.println("-->[EINK] Forcing full refresh of display");
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
    // showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
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

#endif  // SUPPORT_EINK
#endif  // CO2_Gadget_EINK_h
