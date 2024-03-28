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

#define ENABLE_GxEPD2_GFX 1
// #include <GFX.h>
#include <GxEPD2_BW.h>

uint16_t deepSleepReadrawEach = 5;

#include <Fonts/FreeMonoBold9pt7b.h>
#include <NotoSans_Bold48pt7b.h>

#include "bootlogo.h"
#include "icons.h"

// 'iconTemperature', 16x16px
const unsigned char iconTemp[256] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xf7, 0xe7, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xef, 0xce, 0xdf, 0xf7, 0x00, 0x6c, 0x6c, 0x6c, 0x64, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xce, 0x85, 0xf7, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xc6, 0x85, 0xf7, 0xf7, 0x00, 0x6c, 0x64, 0x64, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xc6, 0x85, 0xf7, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xe4, 0xf1, 0xf7, 0xf7, 0x00, 0x64, 0x6c, 0x6c, 0x64, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xe3, 0xf9, 0xf7, 0xf7, 0x00, 0x07, 0x6c, 0x54, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xe3, 0xf9, 0xf7, 0xf7, 0x00, 0x6c, 0x6c, 0x64, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf7, 0xe3, 0xf9, 0xf7, 0xf7, 0x00, 0x84, 0x6c, 0x6c, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xf7, 0xe3, 0xf9, 0xf7, 0xf7, 0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf7, 0xef, 0xe3, 0xf9, 0xe4, 0xdf, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf7, 0xf7, 0xdc, 0xfa, 0xfa, 0xf9, 0xe5, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xef, 0xf7, 0xe3, 0xfa, 0xfa, 0xf9, 0xe3, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xe7, 0xf7, 0xdc, 0xfa, 0xfa, 0xf9, 0xe6, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf7, 0xf7, 0xdd, 0xe3, 0xe5, 0xdf, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xf7, 0xf7, 0xef, 0xf7, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const GFXfont SmallFont = FreeMonoBold9pt7b;
const GFXfont BigFont = NotoSans_Bold48pt7b;

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
int displayWidth = 212;
int displayHeight = 104;

#define EPD_SCLK SCK  // 18
#define EPD_MISO 17
#define EPD_DC 17      // MISO
#define EPD_MOSI MOSI  // 23
#define EPD_CS SS
#define EPD_RST 16
#define EPD_BUSY 4

#ifdef EINKBOARDDEPG0213BN
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT>
    display(GxEPD2_213_BN(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // DEPG0213BN https://s.click.aliexpress.com/e/_Aadykl
#endif
#ifdef EINKBOARDGDEM0213B74
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT>
    display(GxEPD2_213_B74(/* EPD_CS */ EPD_CS, /* EPD_MISO */ EPD_DC, /* EPD_RST */ EPD_RST, /* EPD_BUSY */ EPD_BUSY));  // GDEM0213B74
#endif

#include "bitmaps/Bitmaps104x212.h"  // 2.13" b/w flexible GDEW0213I5F

#endif

#ifdef EINKBOARDGDEM029T94
int displayWidth = 296;
int displayHeight = 128;

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

#include "bitmaps/Bitmaps128x296.h"  // 2.9"  b/w

#endif

int32_t iconState_wifi = -9999;
int32_t iconState_BLE = -9999;
int32_t iconState_Battery = -9999;
int32_t iconState_mqtt = -9999;
int32_t iconState_esp32 = -9999;

// Define a structure for the locations of elements
struct ElementLocations {
    int32_t co2X;
    int32_t co2Y;
    int32_t co2W;
    int32_t co2H;
    u_int16_t co2FontDigitsHeight;
    u_int16_t pixelsToBaseline;
    int32_t co2UnitsX;
    int32_t co2UnitsY;
    int32_t co2UnitsW;
    int32_t co2UnitsH;
    int32_t tempX;
    int32_t tempY;
    int32_t tempW;
    int32_t tempH;
    int32_t humidityX;
    int32_t humidityY;
    int32_t humidityW;
    int32_t humidityH;
    int32_t batteryIconX;
    int32_t batteryIconY;
    int32_t batteryIconW;
    int32_t batteryIconH;
    int32_t batteryVoltageX;
    int32_t batteryVoltageY;
    int32_t batteryVoltageW;
    int32_t batteryVoltageH;
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

#if defined(EINKBOARDDEPG0213BN) || defined(EINKBOARDGDEM0213B74)
    if (displayWidth == 212 && displayHeight == 104) {  // 212x104 DEPG0213BN, GDEM0213B74 and similar
                                                        // GDEM0213B74 250x122 pixels
        elementPosition.co2X = 0;
        elementPosition.co2Y = 24;
        elementPosition.co2W = display.width() - 24;
        elementPosition.co2H = display.height() - 48;
        elementPosition.co2FontDigitsHeight = 70;  // Digits (0..9) height for the font used (not the same as whole font height)
        elementPosition.pixelsToBaseline = 18;     // Pixels bellow baseline (p.ej "y" in "y" or "g" in "g" they draw bellow the baseline))
        elementPosition.co2UnitsX = display.width() - 24;
        elementPosition.co2UnitsY = 24;
        elementPosition.co2UnitsW = 24;
        elementPosition.co2UnitsH = display.height() - 48;
        elementPosition.tempX = 0;
        elementPosition.tempY = display.height() - 24;
        elementPosition.tempW = 72;
        elementPosition.tempH = 24;
        elementPosition.humidityX = display.width() - 72;
        elementPosition.humidityY = display.height() - 24;
        elementPosition.humidityW = 72;
        elementPosition.humidityH = 24;
        elementPosition.batteryIconX = display.width() - 48;
        elementPosition.batteryIconY = 0;
        elementPosition.batteryIconW = 48;
        elementPosition.batteryIconH = 24;
        elementPosition.batteryVoltageX = display.width() - 96;
        elementPosition.batteryVoltageY = 0;
        elementPosition.batteryVoltageW = 48;
        elementPosition.batteryVoltageH = 24;
        elementPosition.bleIconX = 0;
        elementPosition.bleIconY = 0;
        elementPosition.wifiIconX = 24;
        elementPosition.wifiIconY = 0;
        elementPosition.mqttIconX = 48;
        elementPosition.mqttIconY = 0;
        elementPosition.espNowIconX = 72;
        elementPosition.espNowIconY = 0;
    }
#endif
}
void displayShowValues(bool forceRedraw = false);  // Forward declaration
void drawMainScreen(bool force = false);           // Forward declaration

void setDisplayBrightness(uint32_t newBrightness) {
    // Serial.println("-->[EINK] Setting display brightness (ignore for e-Ink) to: " + String(newBrightness));
    // display.setContrast(newBrightness);
    // actualDisplayBrightness = newBrightness;
}

void turnOffDisplay() {
    //    setDisplayBrightness(0);  // Turn off the display
}

void displaySleep(bool value)  // https://github.com/Bodmer/TFT_eSPI/issues/715
{
    // return;  // e-Ink don't need hibernate display
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

void drawHorizontalCenterText(int16_t y, const String text) {
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
    //    String salida = "-->[PRNT] Text: " + text + " - x: " + String(x) + " y: " + String(y) + " w: " + String(w) + " h: " + String(h) + " tbx: " + String(tbx) + " tby: " + String(tby) + " tbw: " + String(tbw) + " tbh: " + String(tbh) + " valign: " + String(v_align) + " halign: " + String(h_align) + " posX: " + String(pos_x) + " posY: " + String(pos_y);
    //    Serial.println(salida);

    // Display text
    display.setCursor(pos_x, pos_y);
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
    display.init(115200, true, 2, true);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);

    // Each deepSleepReadrawEach boots do a full screen refresh
    if (forceRedraw) {
        Serial.println("-->[EINK] Initializing display from deep sleep with force redraw");
        display.setFullWindow();
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);
            //            drawMainScreen(true);
        } while (display.nextPage());
        deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;
    } else {
        Serial.println("-->[EINK] Initializing display from deep sleep with force redraw");
        display.setPartialWindow(0, 0, display.width(), display.height());
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);
            displayShowValues();
        } while (display.nextPage());
        // TO-DO: Optimize this to avoid full screen refresh and save display update time
        //        display.fillRect(20, 45, display.width() - 40, display.height() - 40, GxEPD_WHITE);
        //        display.displayWindow(0, 0, display.width(), display.height());
        //        drawMainScreen(true);
    }
    display.hibernate();
}

void initDisplay(bool fastMode = false) {
    if (!fastMode) Serial.println("-->[TFT ] Initializing display");
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(115200, true, 2, true);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

    deepSleepData.cyclesToRedrawDisplay = cyclesToRedrawDisplay;

    // Set default options to draw
    display.setRotation(1);
    display.setFont(&SmallFont);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();

    setElementLocations();

    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
        //        drawMainScreen(true);
    } while (display.nextPage());

    /*
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
    */
    // Show splash screen the first time
    // if (bootCount == 1) {
    // displaySplashScreenLOGO();
    // delay(1000);
    // displaySplashScreen();
    // delay(1000);
    // drawMainScreen();
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
    display.setCursor(0, 12);
    display.print("TEMP: ");
    display.setCursor((display.width()) - 5 * 9 * 2 - 35, 12);
    display.print("HUM: ");
    display.setRotation(4);
    display.setCursor((display.width() / 2) - 20, display.height() - 3);
    display.print("PPM");

#ifdef TIMEDEBUG
    Serial.print("-->[EINK] Time used to drawMainScreen: \t");
    Serial.println(timer.read());
#endif
}

void showWiFiIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    //    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
    int8_t rssi = WiFi.RSSI();
    if (troubledWIFI) {
        display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX + 3, posY + 3, iconWiFi, 16, 16, GxEPD_BLACK);
        return;
    }

    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeWIFI) {
        display.drawInvertedBitmap(posX + 3, posY + 3, iconWiFi, 16, 16, GxEPD_BLACK);
    } else {
        if (WiFi.status() == WL_CONNECTED) {
            if (rssi < 60)
                display.drawBitmap(posX + 3, posY + 3, iconWiFi, 16, 16, GxEPD_BLACK);
            else if (rssi < 70)
                display.drawBitmap(posX + 3, posY + 3, iconWiFiMed, 16, 16, GxEPD_BLACK);
            else if (rssi < 80)
                display.drawBitmap(posX + 3, posY + 3, iconWiFiMed, 16, 16, GxEPD_BLACK);
        } else {
            display.drawBitmap(posX + 3, posY + 3, iconWiFiLow, 16, 16, GxEPD_BLACK);
        }
    }
}

void showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    //    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeBLE) {
        display.drawInvertedBitmap(posX + 3, posY + 3, iconBLE, 16, 16, GxEPD_BLACK);
    } else {
        display.drawBitmap(posX + 3, posY + 3, iconBLE, 16, 16, GxEPD_BLACK);
    }
}

void showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    //    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
    if (troubledMQTT) {
        display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX + 3, posY + 3, iconMQTT, 16, 16, GxEPD_BLACK);
        return;
    }
    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeMQTT) {
        display.drawBitmap(posX + 3, posY + 3, iconMQTT, 16, 16, GxEPD_BLACK);
    } else {
        display.drawInvertedBitmap(posX + 3, posY + 3, iconMQTT, 16, 16, GxEPD_BLACK);
    }
}

void showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
//    display.fillRect(posX, posY, 16+6, 16+6, GxEPD_WHITE);
#ifdef SUPPORT_ESPNOW
    if (troubledESPNOW) {
        display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        display.drawBitmap(posX + 3, posY + 3, iconEspNow, 16, 16, GxEPD_BLACK);
        return;
    }
    display.drawRoundRect(posX, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
    if (!activeESPNOW) {
        display.drawInvertedBitmap(posX + 3, posY + 3, iconEspNow, 16, 16, GxEPD_BLACK);
    } else {
        display.drawBitmap(posX + 3, posY + 3, iconEspNow, 16, 16, GxEPD_BLACK);
    }
#endif
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY, int32_t W, int32_t H, uint16_t pixelsToBaseline, bool forceRedraw) {
    RTC_DATA_ATTR static uint16_t oldCO2Value = 0;
    //  display.fillRect(posX, posY, W, H, GxEPD_WHITE);
    //    if (!forceRedraw && (co2 == oldCO2Value || (co2 == 0) || (co2 > 9999))) return;

    display.setFont(&BigFont);
    display.setTextSize(1);
    display.setRotation(1);

    drawTextAligned(posX, posY, W, H, String(co2), 'r', 'c');
    oldCO2Value = co2;
}

void showCO2units(int32_t posX, int32_t posY, bool forceRedraw) {
    display.setRotation(4);
    display.setPartialWindow(posY, posX, display.height() - 24, display.width());
    display.setFont(&SmallFont);
    display.setCursor(0, 24);
    //    display.setRotation(4);
    display.print("ppm");
    display.setRotation(1);
    display.setPartialWindow(0, 0, display.width(), display.height());
}

void showTemperatureIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    display.drawBitmap(posX, posY, iconTemp, 16, 16, GxEPD_BLACK);
}

void showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw) {
    //    display.fillRect(posX + 24, posY, 48, 24, GxEPD_WHITE);
    //    if (!displayShowTemperature) return;
    display.setRotation(1);
    showTemperatureIcon(posX + 3, posY + 3, forceRedraw);
    display.setFont(&SmallFont);
    String temperatureString = (showFahrenheit ? String(tempFahrenheit, 1) : String(temp, 1)) + "° ";
    drawTextAligned(posX + 24, posY, 48, 24, temperatureString, 'l', 'c');
}

void showHumidityIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    //    if (!forceRedraw) return;
    //    display.drawImage(iconHumidity, posX, posY, 16, 16, GxEPD_BLACK);
    //    display.setPartialWindow(posX, posY, 16, 16);
}

void showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    //    if (!displayShowHumidity) return;
    showHumidityIcon(posX + 3, posY + 3, forceRedraw);
    display.setFont(&SmallFont);
    String humidityString = String(hum, 0) + "% ";
    //    display.fillRect(posX + 24, posY, 48, 24, GxEPD_WHITE);
    drawTextAligned(posX + 24, posY, 48, 24, humidityString, 'l', 'c');
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

void showBatteryVoltage(int32_t posX, int32_t posY, bool forceRedraw) {
    //    if ((!displayShowBatteryVoltage) || (!displayShowBattery) || (batteryVoltage < 1)) return;
    String batteryVoltageString = " " + String(batteryVoltage, 1) + "V ";
    display.setFont(&SmallFont);
    display.fillRect(posX, posY, 48, 24, GxEPD_WHITE);
    drawTextAligned(posX, posY, 48, 24, batteryVoltageString, 'r', 'u');
}

void showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw) {  // For TTGO T-Display posX=tft.width() - 32, posY=4
                                                                      //    if (!forceRedraw) return;
    publishMQTTLogData("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V");
    if (workingOnExternalPower) {
        display.drawRoundRect(posX + 8, posY, 16 + 6, 16 + 6, 2, GxEPD_BLACK);
        //        display.setSwapBytes(true);
        display.drawBitmap(posX + 8, posY, iconUSB, 16, 16, GxEPD_BLACK);
    } else {
        uint32_t upperLine = 4;
        display.drawRoundRect(posX + 8, posY, 34, 16, 3, GxEPD_BLACK);  // Battery outter rectangle
        display.fillRect(posX + 6, posY + 4, 2, 6, GxEPD_BLACK);
        if (batteryLevel > 20) display.fillRect(posX + 12, posY + 2, 4, 10, GxEPD_BLACK);
        if (batteryLevel > 40) display.fillRect(posX + 19, posY + 2, 4, 10, GxEPD_BLACK);
        if (batteryLevel > 60) display.fillRect(posX + 26, posY + 2, 4, 10, GxEPD_BLACK);
        if (batteryLevel > 80) display.fillRect(posX + 33, posY + 2, 4, 10, GxEPD_BLACK);
    }
}

void displayShowValues(bool forceRedraw) {
#ifdef TIMEDEBUG
    timer.start();
#endif
    /*    if (forceRedraw) {
            display.setFullWindow();
            display.fillScreen(GxEPD_WHITE);
        }
    */
    display.setPartialWindow(0, 0, display.width(), display.height());
    display.fillScreen(GxEPD_WHITE);
    display.setRotation(1);

    // display.firstPage();
    //     do {
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    // drawMainScreen();
    // showValues();
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, elementPosition.co2W, elementPosition.co2H, elementPosition.pixelsToBaseline, forceRedraw);
    showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY, forceRedraw);
    showTemperature(temp, elementPosition.tempX, elementPosition.tempY, forceRedraw);
    showHumidity(hum, elementPosition.humidityX, elementPosition.humidityY, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, forceRedraw);
    showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    //    display.displayWindow(0, 0, display.width(), display.height());
    //    display.drawRoundRect(0, 0, display.width(), display.height(), 2, GxEPD_BLACK);
    //    } while (display.nextPage());
    display.displayWindow(0, 0, display.width(), display.height());
    // display.displayWindow(0, 0, display.width(), display.height());

    //    display.hibernate();

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
