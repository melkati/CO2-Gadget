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

uint16_t deepSleepReadrawEach = 5;

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEW0213Z16)
#include <Fonts/FreeMonoBold9pt7b.h>
#include <NotoSans_Bold48pt7b.h>
int displayWidth = 212;
int displayHeight = 104;

#include "bootlogo.h"
#include "icons.h"
const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;
#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17      // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4

#ifdef EINKBOARDDEPG0213BN
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif
#ifdef EINKBOARDGDEM0213B74
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEM0213B74
#endif
#ifdef EINKBOARDGDEW0213Z16
GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex ::HEIGHT> display(GxEPD2_213_flex(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEM0213B74
#endif
#endif

#ifdef EINKBOARDGDEM029T94
#include <Fonts/FreeMonoBold9pt7b.h>
#include <NotoSans_Bold48pt7b.h>

#include "bootlogo.h"  // Made with https://javl.github.io/image2cpp/
#include "icons.h"
int displayWidth = 296;
int displayHeight = 128;

const GFXfont SmallFont = FreeMonoBold9pt7b;
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
// GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=*/5, /*DC=*/27, /*RST=*/25, /*BUSY=*/32));  // GDEM029T94
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=5*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RST, /*BUSY=*/EPD_BUSY));  // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)

#endif

// Define a structure for the locations of elements
struct ElementLocations {
    int32_t co2X;
    int32_t co2Y;
    u_int16_t co2FontDigitsHeight;
    u_int16_t pixelsToBaseline;
    int32_t co2UnitsX;
    int32_t co2UnitsY;
    int32_t tempX;
    int32_t tempY;
    int32_t humidityX;
    int32_t humidityY;
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
        elementPosition.co2X = displayWidth - 32;
        elementPosition.co2Y = displayHeight - 33;
        elementPosition.co2FontDigitsHeight = 70;  // Digits (0..9) height for the font used (not the same as whole font height)
        elementPosition.pixelsToBaseline = 18;     // Pixels bellow baseline (p.ej "y" in "y" or "g" in "g" they draw bellow the baseline))
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempX = 1;
        elementPosition.tempY = displayHeight - 25;
        elementPosition.humidityX = displayWidth - 60;
        elementPosition.humidityY = displayHeight - 25;
        elementPosition.batteryIconX = displayWidth - 34;
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
#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74) || defined(EINKBOARDGDEW0213Z16)
    if (displayWidth == 212 && displayHeight == 104) {  // 212x104 DEPG0213BN, GDEM0213B74 and similar
        elementPosition.co2X = displayWidth - 32;
        elementPosition.co2Y = displayHeight - 33;
        elementPosition.co2FontDigitsHeight = 70;  // Digits (0..9) height for the font used (not the same as whole font height)
        elementPosition.pixelsToBaseline = 18;     // Pixels bellow baseline (p.ej "y" in "y" or "g" in "g" they draw bellow the baseline))
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempX = 1;
        elementPosition.tempY = displayHeight - 25;
        elementPosition.humidityX = displayWidth - 60;
        elementPosition.humidityY = displayHeight - 25;
        elementPosition.batteryIconX = displayWidth - 34;
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

void setDisplayBrightness(uint32_t newBrightness) {
    // Serial.println("-->[EINK] Setting display brightness (ignore for e-Ink) to: " + String(newBrightness));
    // display.setContrast(newBrightness);
    // actualDisplayBrightness = newBrightness;
}

void turnOffDisplay() {
    setDisplayBrightness(0);  // Turn off the display
}

void displaySleep(bool value)  // https://github.com/Bodmer/TFT_eSPI/issues/715
{
    display.hibernate();
    if (value) {
        // display.powerOff();  // Send command to put the display to sleep.
        // delay(150);              // Delay for shutdown time before another command can be sent.
    } else {
        ;  // This sends the wake up command and initialises the display
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

// void drawLogoScreen() {
//     display.setFullWindow();
//     display.firstPage();
//     do {
//         // Draw bitmap
//         display.fillScreen(GxEPD_WHITE);
//         display.drawInvertedBitmap((display.width() - 128) / 2, (display.height() - 64) / 2, splash, 128, 64, GxEPD_BLACK);
//     } while (display.nextPage());
// }

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

void showPages() {
    display.setRotation(0);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.clearScreen(0);  // black
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0, 10);
    if (display.width() >= 300) {
        display.print("would need ");
        display.print(display.pages());
        display.println(" pages of height ");
        display.print(display.pageHeight());
    } else {
        display.println("would need");
        display.print(display.pages());
        display.println(" pages of");
        display.print("height ");
        display.print(display.pageHeight());
    }
    display.display(false);  // full update
}

void initDisplayFromDeepSleep(bool forceRedraw = false) {
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(115200, false, 2, false);

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    // display.setFullWindow();
    // display.setPartialWindow(0, 0, display.width(), display.height());

    // Each deepSleepReadrawEach boots do a full screen refresh
    if (forceRedraw) {
        Serial.println("-->[EINK] Initializing display from deep sleep with force redraw");
        display.fillScreen(GxEPD_WHITE);
        display.display();
        drawMainScreen(true);
        deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;
    } else {
        // TO-DO: Optimize this to avoid full screen refresh and save display update time
        display.setPartialWindow(0, 0, display.width(), display.height());
        display.fillRect(20, 45, display.width() - 40, display.height() - 40, GxEPD_WHITE);
        display.displayWindow(0, 0, display.width(), display.height());
        drawMainScreen(true);
    }
}

void initDisplay(bool fastMode = false) {
    if (!fastMode) Serial.println("-->[TFT ] Initializing display");
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(115200, true, 2, false);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.setPartialWindow(0, 0, display.width(), display.height());

    Serial.println("-->[EINK] Display hasPartialUpdate " + String(display.epd2.hasPartialUpdate));
    Serial.println("-->[EINK] Display hasFastPartialUpdate " + String(display.epd2.hasFastPartialUpdate));

    if (display.pages() > 1) {
        delay(100);
        Serial.print("pages = ");
        Serial.print(display.pages());
        Serial.print(" page height = ");
        Serial.println(display.pageHeight());
        delay(1000);
        showPages();
        display.hibernate();
    }

    // setElementLocations();

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

void showBattery() {
    if (workingOnExternalPower) {
        display.fillRect(display.width() - 27, 2, 26, 12, GxEPD_WHITE);  // Delete battery icon
        return;
    }
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

void drawMainScreen(bool force) {
    RTC_DATA_ATTR static uint16_t drawTimes = 0;
    if ((!force) && (drawTimes > 0)) {
        return;
    }
    #ifdef TIMEDEBUG
    timer.start();
    #endif
    drawTimes++;

    // Enable partial refresh
    display.setPartialWindow(0, 0, display.width(), display.height());
    Serial.println("-->[EINK] Drawing main screen at " + String(display.width()) + "x" + String(display.height()));

    // Clear screen
    display.fillScreen(GxEPD_WHITE);

    // Draw labels and field rectangles
    display.drawRoundRect(0, 20, display.width(), display.height() - 30, 6, GxEPD_BLACK);
    display.setRotation(1);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(0, 12);
    display.print("TEMP: ");
    display.setCursor((display.width()) - 5 * 9 * 2 - 35, 12);
    display.print("HUM: ");
    display.setRotation(4);
    display.setCursor((display.width() / 2) - 20, display.height() - 3);
    display.print("PPM");
    display.setRotation(1);

    // Refresh screen in partial mode
    display.displayWindow(0, 0, display.width(), display.height());

    #ifdef TIMEDEBUG
    Serial.print("-->[EINK] Time used to drawMainScreen: \t");
    Serial.println(timer.read());
    #endif
}

void showValues() {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    RTC_DATA_ATTR static float oldTempValue = 0;
    RTC_DATA_ATTR static float oldHumiValue = 0;

    if (oldCO2Value == co2 && oldTempValue == temp && oldHumiValue == hum) {
        return;
    }

    // Erase old values
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&BigFont);
    display.setTextSize(1);
    drawHoritzontalCenterText((display.height() / 2) + 40, String(oldCO2Value));
    display.setTextSize(1);
    display.setFont(&SmallFont);
    display.setCursor(55, 12);
    display.printf("%.1fºC", oldTempValue);
    display.setCursor((display.width()) - 5 * 9 - 35, 12);
    display.printf("%.0f%%", oldHumiValue);

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

    oldCO2Value = co2;
    oldTempValue = temp;
    oldHumiValue = hum;
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY, uint16_t pixelsToBaseline, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    if (!forceRedraw && (co2 == oldCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // full window mode is the initial mode, set it anyway
    display.setFullWindow();
    display.setRotation(1);
    display.setFont(&BigFont);
    display.setTextColor(GxEPD_BLACK);

    display.getTextBounds(String(oldCO2Value), 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("-->[EINK] CO2 OLD text bounds: value=" + String(oldCO2Value) + ", tbx=" + String(tbx) + ", tby=" + String(tby) + ", tbw=" + String(tbw) + ", tbh=" + String(tbh));

    uint16_t utx = ((display.width() - tbw) / 2) - tbx;
    uint16_t uty = ((display.height() / 4) - tbh / 2) - tby - 30;
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(utx, uty);
    display.print(String(oldCO2Value));

    display.getTextBounds(String(co2), 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("-->[EINK] CO2 NEW text bounds: value=" + String(co2) + ", tbx=" + String(tbx) + ", tby=" + String(tby) + ", tbw=" + String(tbw) + ", tbh=" + String(tbh));

    uint16_t umx = ((display.width() - tbw) / 2) - tbx;
    uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby - 30;
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(umx, umy);
    display.print(String(co2));

    display.display(true);

    oldCO2Value = co2;
}

void displayShowValues(bool forceRedraw = false) {
    #ifdef TIMEDEBUG
    timer.start();
    #endif
    if (forceRedraw) {
        // tft.fillScreen(TFT_BLACK);
    }
    // drawMainScreen();
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
    // display.hibernate();
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
