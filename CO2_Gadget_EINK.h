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

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEW0213M21)

#include "bootlogo.h"
#include "icons.h"
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17      // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4

#ifdef EINKBOARDDEPG0213BN
#include <Fonts/FreeMonoBold9pt7b.h>
#include <NotoSans_Bold48pt7b.h>
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif
#ifdef EINKBOARDGDEM0213B74
#include <Fonts/FreeMonoBold9pt7b.h>
#include <NotoSans_Bold48pt7b.h>
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
int displayWidth = 250;
int displayHeight = 122;
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEM0213B74
#endif
#ifdef EINKBOARDGDEW0213M21
#include <NotoSans_Bold42pt7b.h>
#include <NotoSans_Bold6pt7b.h>
const GFXfont SmallFont = NotoSans_Bold6pt7b;
const GFXfont BigFont = NotoSans_Bold42pt7b;
int displayWidth = 212;
int displayHeight = 104;
// GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex ::HEIGHT> display(GxEPD2_213_flex(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));
// GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D ::HEIGHT> display(GxEPD2_213_T5D(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));
GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21 ::HEIGHT> display(GxEPD2_213_M21(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEW0213M21 104x212, SSD1608 (GDEW0213Z16LW)
#endif
#endif

#ifdef EINKBOARDGDEM029T94
#include <NotoSans_Bold48pt7b.h>
#include <NotoSans_Bold6pt7b.h>

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"
int displayWidth = 296;
int displayHeight = 128;

const GFXfont SmallFont = NotoSans_Bold6pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
#define EPD_SCLK SCK
#define EPD_MISO 17
#define EPD_MOSI MOSI
// #define EPD_CS SS
// #define EPD_RST 16
// #define EPD_DC 17
// #define EPD_BUSY 4

#define EPD_CS SS
#define EPD_DC 27
#define EPD_RST 25
#define EPD_BUSY 32
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=5*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RST, /*BUSY=*/EPD_BUSY));  // GDEM029T94
// GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/*CS=5*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RST, /*BUSY=*/EPD_BUSY));  // GDEM029T94 V2 GDEM029T94  128x296, SSD1680, (FPC-7519 rev.b), Waveshare 2.9" V2 variant
// GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=*/5, /*DC=*/27, /*RST=*/25, /*BUSY=*/32));  // GDEM029T94 BS
// GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=5*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RST, /*BUSY=*/EPD_BUSY));  // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/* EPD_CS */ 5, /* EPD_MISO */ 17, /* EPD_RST */ 16, /* EPD_BUSY */ 4));  // GDEW029T5
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=5*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RST, /*BUSY=*/EPD_BUSY));  // GDEW029T5 with special pinout
#endif

// Define a structure for the locations of elements
struct ElementLocations {
    int32_t co2X;
    int32_t co2Y;
    u_int16_t co2FontDigitsHeight;
    int32_t co2UnitsX;
    int32_t co2UnitsY;
    int32_t tempXUnits;
    int32_t tempYUnits;
    int32_t tempXValue;
    int32_t tempYValue;
    int32_t humidityXUnits;
    int32_t humidityYUnits;
    int32_t humidityXValue;
    int32_t humidityYValue;
    int32_t ppmXUnits;
    int32_t ppmYUnits;

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
#ifdef EINKBOARDGDEM029T94
    if (displayWidth == 296 && displayHeight == 128) {  // 296x128 GDEM029T94 and similar
        elementPosition.co2X = -1;
        elementPosition.co2Y = displayHeight - 25;
        elementPosition.co2FontDigitsHeight = 48;
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempXUnits = 2;
        elementPosition.tempYUnits = 12;
        elementPosition.tempXValue = 14;
        elementPosition.tempYValue = 12;
        elementPosition.humidityXUnits = 60;
        elementPosition.humidityYUnits = 12;
        elementPosition.humidityXValue = 82;
        elementPosition.humidityYValue = 12;
        elementPosition.ppmXUnits = displayHeight / 2 - 21;  // Display is rotated so vertical orientation (swaped width & height)
        elementPosition.ppmYUnits = displayWidth - 8;
        elementPosition.batteryIconX = displayWidth - 32;
        elementPosition.batteryIconY = 2;
        elementPosition.batteryVoltageX = displayWidth - 92;
        elementPosition.batteryVoltageY = 2;
        elementPosition.bleIconX = 2;
        elementPosition.bleIconY = 2;
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
    }
#endif
#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    if (displayWidth == 250 && displayHeight == 122) {  // 250x122 DEPG0213BN, GDEM0213B74 and similar
        elementPosition.co2X = -1;
        elementPosition.co2Y = displayHeight - 33;
        elementPosition.co2FontDigitsHeight = 48;  // Digits (0..9) height for the font used (not the same as whole font height)
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempXUnits = 2;
        elementPosition.tempYUnits = 12;
        elementPosition.tempXValue = 14;
        elementPosition.tempYValue = 12;
        elementPosition.humidityXUnits = 60;
        elementPosition.humidityYUnits = 12;
        elementPosition.humidityXValue = 82;
        elementPosition.humidityYValue = 12;
        elementPosition.ppmXUnits = displayHeight / 2 - 21;  // Display is rotated so vertical orientation (swaped width & height)
        elementPosition.ppmYUnits = displayWidth - 3;
        elementPosition.batteryIconX = displayWidth - 32;
        elementPosition.batteryIconY = 2;
        elementPosition.batteryVoltageX = displayWidth - 92;
        elementPosition.batteryVoltageY = 2;
        elementPosition.bleIconX = 2;
        elementPosition.bleIconY = 2;
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
    }
#endif
#if defined(EINKBOARDGDEW0213M21)
    if (displayWidth == 212 && displayHeight == 104) {  // 212x104 GDEW0213M21 and similar
        elementPosition.co2X = -1;
        elementPosition.co2Y = displayHeight - 14;
        elementPosition.co2FontDigitsHeight = 42;  // Digits (0..9) height for the font used (not the same as whole font height)
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempXUnits = 2;
        elementPosition.tempYUnits = 12;
        elementPosition.tempXValue = 14;
        elementPosition.tempYValue = 12;
        elementPosition.humidityXUnits = 60;
        elementPosition.humidityYUnits = 12;
        elementPosition.humidityXValue = 82;
        elementPosition.humidityYValue = 12;
        elementPosition.ppmXUnits = displayHeight / 2 - 21;  // Display is rotated so vertical orientation (swaped width & height)
        elementPosition.ppmYUnits = displayWidth - 3;
        elementPosition.batteryIconX = displayWidth - 32;
        elementPosition.batteryIconY = 2;
        elementPosition.batteryVoltageX = displayWidth - 92;
        elementPosition.batteryVoltageY = 2;
        elementPosition.bleIconX = 2;
        elementPosition.bleIconY = 2;
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
    }
#endif
}

void drawMainScreen(bool force = false);  // Forward declaration

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
#ifdef TIMEDEBUG
    timer.start();
#endif
    display.setFullWindow();
    display.firstPage();
    do {
        // Draw bitmap
        display.fillScreen(GxEPD_WHITE);
        display.drawInvertedBitmap((display.width() - 250) / 2, (display.height() - 128) / 2, Logo250x128, 250, 128, GxEPD_BLACK);
    } while (display.nextPage());
#ifdef TIMEDEBUG
    Serial.print("time used to displaySplashScreenLOGO: ");
    Serial.println(timer.read());
#endif
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

// busyCallback function called during waiting for BUSY to end, to light sleep or service other tasks
void busyCallback(const void* p) {
#ifdef DEBUG_EINK
    // Serial.println("eink busyCallback light sleep");
#endif
    esp_sleep_enable_timer_wakeup(0.2 * 1000000);  // 0.2 seconds
    Serial.flush();
    timerLightSleep.resume();
    esp_light_sleep_start();
    timerLightSleep.pause();
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
        drawMainScreen(false);
        deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;
    } else {
#ifdef DEBUG_EINK
        Serial.print("-->[EINK] Initializing display from deep sleep with partial refresh from: ");
        Serial.println(__func__);
#endif
        // display.setPartialWindow(0, 0, display.width(), display.height());
        // display.fillRect(20, 45, display.width() - 40, display.height() - 40, GxEPD_WHITE);
        // display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
        // display.displayWindow(0, 0, display.width(), display.height());
        drawMainScreen(false);
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
    // display.init(115200, true, 2, false);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    display.init(115200, !fastMode, 2, false);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;

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

    // Show splash screen the first time
    // if (bootCount == 1) {
    // displaySplashScreenLOGO();
    // delay(1000);
    // displaySplashScreen();
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

void drawMainScreen(bool fullRefresh) {
    RTC_DATA_ATTR static uint16_t drawTimes = 0;
    if ((!fullRefresh) && (drawTimes > 0)) {
        return;
    }
#ifdef TIMEDEBUG
    timer.start();
#endif
    // drawTimes++;

    if (fullRefresh) {
        display.setFullWindow();  // Enable full refresh
        display.fillScreen(GxEPD_WHITE);
    } else {
        display.setPartialWindow(0, 0, display.width(), display.height());  // Enable partial refresh
        display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE);
    }

    // Enable partial refresh
    // display.setPartialWindow(0, 0, display.width(), display.height());
#ifdef DEBUG_EINK
    Serial.print("-->[EINK] Drawing main screen at " + String(display.width()) + "x" + String(display.height()) + " in " + (fullRefresh ? "full" : "partial") + " mode from: ");
    Serial.println(__func__);
#endif

// Draw labels and field rectangles
#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    display.drawRoundRect(0, 20, display.width(), display.height() - 30, 6, GxEPD_BLACK);
#endif
#if defined(EINKBOARDGDEW0213M21)
    display.drawRoundRect(0, 23, display.width(), display.height() - 28, 6, GxEPD_BLACK);
#endif
#if defined(EINKBOARDGDEM029T94)
    display.drawRoundRect(0, 23, display.width(), display.height() - 30, 6, GxEPD_BLACK);
#endif
    display.setRotation(1);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(elementPosition.tempXUnits, elementPosition.tempYUnits);
    display.print("T:");
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn T label at " + String(elementPosition.tempXUnits) + ", " + String(elementPosition.tempYUnits));
#endif
    // #if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    //     display.setCursor((display.width()) - 5 * 9 * 2 - 35, 12);
    // #endif
    // #if defined(EINKBOARDGDEW0213M21)
    //     display.setCursor((display.width()) - 5 * 6 * 2 - 55, 12);
    // #endif
    // #if defined(EINKBOARDGDEM029T94)
    //     display.setCursor((display.width()) - 5 * 9 * 2 - 65, 12);
    // #endif
    display.setCursor(elementPosition.humidityXUnits, elementPosition.humidityYUnits);
    display.print("HR:");
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn HR label at " + String(elementPosition.humidityXUnits) + ", " + String(elementPosition.humidityYUnits));
#endif
    display.setRotation(4);
    // display.setCursor((display.width() / 2) - 15, display.height() - 3);
    display.setCursor(elementPosition.ppmXUnits, elementPosition.ppmYUnits);
    display.print("PPM");
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn PPM label at " + String(elementPosition.ppmXUnits) + ", " + String(elementPosition.ppmYUnits));
#endif
    display.setRotation(1);

    if (fullRefresh) {
        display.display();  // Full update
    } else {
        display.displayWindow(0, 0, display.width(), display.height());  // Partial update
    }

#ifdef TIMEDEBUG
    Serial.print("-->[EINK] Time used to drawMainScreen: \t");
    Serial.println(timer.read());
#endif
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
    RTC_DATA_ATTR static int32_t oldPosX = -1;
    RTC_DATA_ATTR static int32_t oldPosY = -1;
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    if (!forceRedraw && (co2 == oldCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;

    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    // Erase old CO2 value
    if (oldCO2Value != 0) {
        display.setFont(&BigFont);
        display.setTextColor(GxEPD_WHITE);

        display.setCursor(oldPosX, oldPosY);
        display.print(String(oldCO2Value));
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Erased old CO2 value: " + String(oldCO2Value) + " at: " + String(oldPosX) + ", " + String(oldPosY) + " in: " + __func__);
#endif
    }

    // Show new CO2 value
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);
    if (posX == -1) {
        display.getTextBounds(String(co2), posX, 0, &tbx, &tby, &tbw, &tbh);
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Text Bounds for CO2: " + String(tbx) + ", " + String(tby) + ", " + String(tbw) + ", " + String(tbh) + " in: " + __func__);
#endif
        // posX equals to the center of the screen minus half of the text width
        posX = ((display.width() - tbw) / 2) - tbx - 5;

        // posX = ((display.width() - tbw) / 2) + 40 - tbx;
        display.setCursor(posX, posY);
        display.print(String(co2));
    } else {
        display.setCursor(posX, posY);
        display.print(String(co2));
    }
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn CO2 value: " + String(co2) + " at: " + String(posX) + ", " + String(posY) + " in: " + __func__);
#endif
    oldPosX = posX;
    oldPosY = posY;
    oldCO2Value = co2;
}

void showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldHumiValue = -200;
    String newHumidityStr, oldHumidityStr;

    if (!forceRedraw && (hum == oldHumiValue)) return;
    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
    // Erase old humidity value
    if (oldHumiValue != -200) {
        display.setFont(&SmallFont);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(posX, posY);
        oldHumidityStr = String(int(oldHumiValue)) + "%";
        display.print(oldHumidityStr);
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Erased old humidity value: " + String(oldHumiValue) + " at: " + String(posX) + ", " + String(posY) + " in: " + __func__);
#endif
    }

    // Show new humidity value
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(posX, posY);
    newHumidityStr = String(int(hum)) + "%";
    display.print(newHumidityStr);
#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn humidity value: " + String(hum) + " at: " + String(posX) + ", " + String(posY) + " in: " + __func__);
#endif
    oldHumiValue = hum;
}

void showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw) {
    RTC_DATA_ATTR static float oldTempValue = -200;
    if (!forceRedraw && (temp == oldTempValue)) return;

    display.setPartialWindow(0, 0, display.width(), display.height());
    // Erase old temperature value
    if (oldTempValue != -200) {
        display.setRotation(1);
        display.setFont(&SmallFont);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(posX, posY);
        display.print(String(oldTempValue, 1) + "C");
#ifdef DEBUG_EINK
        Serial.println("-->[EINK] Erased old temperature value: " + String(oldTempValue) + " in: " + __func__);
#endif
    }

    display.setTextColor(GxEPD_BLACK);
    display.setCursor(posX, posY);
    display.print(String(temp, 1) + "C");

#ifdef DEBUG_EINK
    Serial.println("-->[EINK] Drawn temperature value: " + String(temp) + " in: " + __func__);
#endif
    oldTempValue = temp;
}

void showValues() {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    RTC_DATA_ATTR static float oldTempValue = 0;
    RTC_DATA_ATTR static float oldHumiValue = 0;
    RTC_DATA_ATTR static bool firstTime = true;

    if (oldCO2Value == co2 && oldTempValue == temp && oldHumiValue == hum) {
        return;
    }

#ifdef DEBUG_EINK
    Serial.print("-->[EINK] Erasing values: CO2=" + String(oldCO2Value) + ", Temp=" + String(oldTempValue) + ", Hum=" + String(oldHumiValue) + "in: ");
    Serial.println(__func__);
    Serial.print("-->[EINK] Showing values: CO2=" + String(co2) + ", Temp=" + String(temp) + ", Hum=" + String(hum) + "in: ");
    Serial.println(__func__);
#endif

    display.displayWindow(0, 0, display.width(), display.height());

    // Erase old values
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&BigFont);
    display.setTextSize(1);
    drawHoritzontalCenterText((display.height() / 2) + 40, String(oldCO2Value));
    display.setFont(&SmallFont);
    // #if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    //     display.setCursor(55, 12);
    //     display.printf("%.1fºC", oldTempValue);
    //     display.setCursor((display.width()) - 5 * 9 - 35, 12);
    // #endif
    // #if defined(EINKBOARDGDEW0213M21)
    //     display.setCursor(40, 12);
    //     display.printf("%.1fºC", oldTempValue);
    //     display.setCursor((display.width()) - 5 * 6 - 50, 12);
    // #endif
    // #if defined(EINKBOARDGDEM029T94)
    //     display.setCursor(55, 12);
    //     display.printf("%.1fºC", oldTempValue);
    //     display.setCursor((display.width()) - 5 * 9 - 65, 12);
    // #endif
    //     display.printf("%.0f%%", oldHumiValue);

    // if (firstTime) {
    //     display.displayWindow(0, 0, display.width(), display.height());
    //     firstTime = false;
    // }

    // Show values
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&BigFont);
    display.setTextSize(1);
    drawHoritzontalCenterText((display.height() / 2) + 40, String(co2));
    oldCO2Value = co2;
    display.setFont(&SmallFont);

    // #if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    //     display.setCursor(55, 12);
    //     display.printf("%.1fºC", temp);
    //     display.setCursor((display.width()) - 5 * 9 - 35, 12);
    // #endif
    // #if defined(EINKBOARDGDEW0213M21)
    //     display.setCursor(40, 12);
    //     display.printf("%.1fºC", temp);
    //     display.setCursor((display.width()) - 5 * 6 - 50, 12);
    // #endif
    // #if defined(EINKBOARDGDEM029T94)
    //     display.setCursor(55, 12);
    //     display.printf("%.1fºC", temp);
    //     display.setCursor((display.width()) - 5 * 9 - 65, 12);
    // #endif
    //     display.printf("%.0f%%", hum);
    //     oldTempValue = temp;
    //     oldHumiValue = hum;

    // Refresh screen in partial mode
    // display.displayWindow(0, 0, display.width(), display.height());

    // showBattery();

    // display.setTextColor(GxEPD_BLACK);
    // display.setCursor(240, 12);
    // display.print(readBattery());

    // if (firstTime) {
    //     display.display();
    //     firstTime = false;
    // } else {
    //     display.displayWindow(0, 0, display.width(), display.height());
    // }
    // display.displayWindow(0, 0, display.width(), display.height());
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
    // drawHoritzontalCenterText((display.height() / 2) + 40, textDrawn);
    display.setCursor(10, 100);
    display.print(textDrawn);
    display.setFont(&SmallFont);

    if (randomNumbers) textToDraw = String(random(400, 2000));
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&BigFont);
    display.setTextSize(1);
    // drawHoritzontalCenterText((display.height() / 2) + 40, textToDraw);
    display.setCursor(0, 100);
    display.print(textToDraw);
    display.setFont(&SmallFont);
    textDrawn = textToDraw;
    lastTimeDrawn = millis();
}

void displayShowValues(bool forceRedraw = false) {
#ifdef TIMEDEBUG
    timer.start();
#endif
    if (forceRedraw) {
        // tft.fillScreen(TFT_BLACK);
    }
    // drawMainScreen();

    // showValues();
    // testRedrawValues(true);
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, forceRedraw);
    // showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY, forceRedraw);
    showTemperature(temp, elementPosition.tempXValue, elementPosition.tempYValue, forceRedraw);
    showHumidity(hum, elementPosition.humidityXValue, elementPosition.humidityYValue, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, true);
    // showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    // showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    // showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    // showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    // showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    // display.hibernate();
    // Refresh screen in partial mode
    display.displayWindow(0, 0, display.width(), display.height());
#ifdef TIMEDEBUG
    uint32_t elapsed = timer.read();
    if (elapsed > 10) {
        Serial.println("-->[EINK] Time used to showValues: " + String(elapsed));
    }
#endif
    forceRedraw = false;
}

#endif  // SUPPORT_EINK
#endif  // CO2_Gadget_EINK_h
