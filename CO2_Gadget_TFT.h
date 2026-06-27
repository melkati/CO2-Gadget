

#ifndef CO2_Gadget_TFT_h
#define CO2_Gadget_TFT_h

#ifdef SUPPORT_TFT

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP TFT DISPLAY FUNCTIONALITY                          *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#include <SPI.h>
#include <TFT_eSPI.h>
#include <esp_task_wdt.h>

// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

// Load fonts for TTGO T-Display and others with 240x135 resolution
#if defined(TFT_WIDTH) && defined(TFT_HEIGHT)
#if TFT_WIDTH == 135 && TFT_HEIGHT == 240
#include "FontNotoSansBold15pt_mp.h"
#include "FontNotoSansBold20.h"
#include "FontNotoSansBold90ptDigits.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansBold20
#define BIG_FONT FontNotoSansBold90ptDigits
#define FONTS_LOADED
#endif
#endif

// Load fonts for T-Display-S3 and others with 320x170 resolution
#if defined(TFT_WIDTH) && defined(TFT_HEIGHT)
#if TFT_WIDTH == 170 && TFT_HEIGHT == 320
#include "FontNotoSansBold120ptDigits.h"
#include "FontNotoSansBold15pt_mp.h"
#include "FontNotoSansBold20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansBold20
#define BIG_FONT FontNotoSansBold120ptDigits
#define FONTS_LOADED
#endif
#endif

// Load fonts for ST7789_240x320 and others with 320x240 resolution
#if defined(TFT_WIDTH) && defined(TFT_HEIGHT)
#if TFT_WIDTH == 240 && TFT_HEIGHT == 320
#include "FontNotoSansBold120ptDigits.h"
#include "FontNotoSansBold15pt_mp.h"
#include "FontNotoSansBold20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansBold20
#define BIG_FONT FontNotoSansBold120ptDigits
#define FONTS_LOADED
#endif
#endif

// Default fonts
#ifndef FONTS_LOADED
#include "FontNotoSansBold15pt_mp.h"
#include "FontNotoSansBold90ptDigits.h"
#include "FontNotoSansRegular20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansRegular20
#define BIG_FONT FontNotoSansBold90ptDigits
#endif

#include "bootlogo.h"
#include "icons.h"

uint16_t iconDefaultColor = TFT_CYAN;
uint16_t displayWidth;
uint16_t displayHeight;

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);  // Invoke library, pins defined in platformio.ini
TFT_eSprite spr = TFT_eSprite(&tft);             // Sprite object "spr" with pointer to "tft" object

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
    int32_t bthomeIconX;
    int32_t bthomeIconY;
    int32_t espNowIconX;
    int32_t espNowIconY;
};

// Define an instance of the structure for the chosen resolution
ElementLocations elementPosition;

// Function to set element locations based on screen resolution
void setElementLocations() {
    if (displayWidth == 240 && displayHeight == 135) {  // TTGO T-Display and similar
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
#ifdef SUPPORT_BTHOME_BLE
        elementPosition.bthomeIconX = 26;
        elementPosition.bthomeIconY = 2;
        elementPosition.wifiIconX = 50;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 74;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 98;
        elementPosition.espNowIconY = 1;
#else
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
#endif
    }

    if (displayWidth == 320 && displayHeight == 170) {  // T-Display-S3 and similar
        elementPosition.co2X = displayWidth - 33;
        elementPosition.co2Y = displayHeight - 33;
        elementPosition.co2FontDigitsHeight = 100;
        elementPosition.pixelsToBaseline = 20;
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
#ifdef SUPPORT_BTHOME_BLE
        elementPosition.bthomeIconX = 26;
        elementPosition.bthomeIconY = 2;
        elementPosition.wifiIconX = 50;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 74;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 98;
        elementPosition.espNowIconY = 2;
#else
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 2;
#endif
    }

    if (displayWidth == 320 && displayHeight == 240) {  // ST7789_240x320 and similar
        elementPosition.co2X = displayWidth - 33;
        elementPosition.co2Y = displayHeight - 108;
        elementPosition.co2FontDigitsHeight = 100;
        elementPosition.pixelsToBaseline = 20;
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 130;
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
#ifdef SUPPORT_BTHOME_BLE
        elementPosition.bthomeIconX = 26;
        elementPosition.bthomeIconY = 2;
        elementPosition.wifiIconX = 50;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 74;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 98;
        elementPosition.espNowIconY = 2;
#else
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 2;
#endif
    }
}

// Save display content as BMP file in SPIFFS (/screenshot.bmp)
File bmpFile;
uint32_t bytesSaved = 0;
volatile bool screenshotCaptureInProgress = false;
volatile bool screenshotCaptureReady = false;
volatile bool screenshotCaptureError = false;
volatile uint8_t screenshotCaptureProgress = 0;
TaskHandle_t screenshotTaskHandle = nullptr;
volatile uint32_t screenshotCaptureStartMs = 0;
// TTGO: readPixel() from a background task conflicts with the main loop's SPI
// write transactions (xQueueGenericSend assertion failure). Use this flag to
// defer the capture to the main-loop context (after displayShowValues finishes).
volatile bool screenshotPendingOnMainLoop = false;
char screenshotCaptureMessage[64] = "idle";
constexpr uint32_t SCREENSHOT_CAPTURE_TIMEOUT_MS = 45000;

void writeTwo(uint16_t word) {
    bmpFile.write(word & 0xFF);
    bmpFile.write((word >> 8) & 0xFF);
    bytesSaved += 2;
}

void writeFour(uint32_t word) {
    bmpFile.write(word & 0xFF);
    bmpFile.write((word >> 8) & 0xFF);
    bmpFile.write((word >> 16) & 0xFF);
    bmpFile.write((word >> 24) & 0xFF);
    bytesSaved += 4;
}

static inline uint16_t swapRedBlue565(uint16_t color) {
    return (color & 0x07E0) | ((color & 0xF800) >> 11) | ((color & 0x001F) << 11);
}

static inline void setScreenshotMessage(const char *msg) {
    if (msg == nullptr) return;
    strncpy(screenshotCaptureMessage, msg, sizeof(screenshotCaptureMessage) - 1);
    screenshotCaptureMessage[sizeof(screenshotCaptureMessage) - 1] = '\0';
}

static inline void updateScreenshotProgress(uint16_t rowsDone, uint16_t totalRows) {
    if (totalRows == 0) {
        screenshotCaptureProgress = 0;
        return;
    }
    uint8_t progress = (uint8_t)((uint32_t)rowsDone * 100UL / (uint32_t)totalRows);
    if (progress > 99) progress = 99;
    screenshotCaptureProgress = progress;
}

static inline void keepScreenshotTaskAlive() {
#if defined(ARDUINO_LILYGO_T_DISPLAY_S3) || defined(TDISPLAY_S3) || defined(LILYGO_T_DISPLAY_S3)
    esp_task_wdt_reset();
#endif
    delay(0);
}

void readDisplayTTGO(uint16_t height, uint16_t width, uint16_t *rowBuffer, uint16_t &rowsDone, uint16_t totalRows) {
#ifdef TTGO_TDISPLAY
    Serial.printf("-->[TFT ] Reading display for screenshot. Width: %d, Height: %d\n", width, height);
    delay(100);  // ST7789V needs settling time after write operations before RAMRD is reliable
    for (int32_t y = (int32_t)height - 1; y >= 0; y--) {
        for (uint16_t x = 0; x < width; x++) {
            rowBuffer[x] = tft.readPixel(x, y);
        }
        bmpFile.write((uint8_t *)rowBuffer, width * sizeof(uint16_t));
        bytesSaved += width * sizeof(uint16_t);
        rowsDone++;
        updateScreenshotProgress(rowsDone, totalRows);
        keepScreenshotTaskAlive();
    }
#endif
}

void readDisplayTDisplayS3(uint16_t height, uint16_t width, uint16_t *rowBuffer, uint16_t &rowsDone, uint16_t totalRows) {
#if defined(ARDUINO_LILYGO_T_DISPLAY_S3) || defined(TDISPLAY_S3) || defined(LILYGO_T_DISPLAY_S3)
    Serial.printf("-->[TFT ] Reading display for screenshot. Width: %d, Height: %d\n", width, height);
    delay(100);  // Allow parallel bus to settle before RAMRD
    for (int32_t y = (int32_t)height - 1; y >= 0; y--) {
        for (uint16_t x = 0; x < width; x++) {
              uint16_t pixel = tft.readPixel(x, y);
            // TFT_eSPI currently reports swapped red/blue channels on T-Display S3 readback.
            rowBuffer[x] = swapRedBlue565(pixel);
        }
        bmpFile.write((uint8_t *)rowBuffer, width * sizeof(uint16_t));
        bytesSaved += width * sizeof(uint16_t);
        rowsDone++;
        updateScreenshotProgress(rowsDone, totalRows);
        keepScreenshotTaskAlive();
    }
#endif
}

void readDisplayTDisplayST7789(uint16_t height, uint16_t width, uint16_t *rowBuffer, uint16_t &rowsDone, uint16_t totalRows) {
#ifdef ST7789_240x320
    Serial.printf("-->[TFT ] Reading display for screenshot. Width: %d, Height: %d\n", width, height);
    for (int32_t y = (int32_t)height - 1; y >= 0; y--) {
        for (uint16_t x = 0; x < width; x++) {
            rowBuffer[x] = tft.readPixel(x, y);
        }
        bmpFile.write((uint8_t *)rowBuffer, width * sizeof(uint16_t));
        bytesSaved += width * sizeof(uint16_t);
        rowsDone++;
        updateScreenshotProgress(rowsDone, totalRows);
        keepScreenshotTaskAlive();
    }
#endif
}

bool takeScreenshot() {
    uint16_t width = tft.width();
    uint16_t height = tft.height();
    uint32_t imageBytes = (uint32_t)width * (uint32_t)height * 2U;
    uint16_t rowsDone = 0;
    uint16_t totalRows = height;

    uint16_t *rowBuffer = (uint16_t *)malloc(width * sizeof(uint16_t));
    if (rowBuffer == nullptr) {
        setScreenshotMessage("no memory");
        Serial.println("-->[TFT ] Error: no memory for screenshot row buffer");
        return false;
    }

    bytesSaved = 0;
    SPIFFS.remove("/screenshot.bmp");
    bmpFile = SPIFFS.open("/screenshot.bmp", FILE_WRITE);
    if (!bmpFile) {
        free(rowBuffer);
        setScreenshotMessage("open file failed");
        Serial.println("-->[TFT ] Error opening /screenshot.bmp for writing");
        return false;
    }

    // BMP file header (14 bytes)
    bmpFile.write('B');
    bmpFile.write('M');
    bytesSaved += 2;
    writeFour(14 + 40 + 12 + imageBytes);  // file size
    writeFour(0);
    writeFour(14 + 40 + 12);  // pixel data offset

    // DIB header (BITMAPINFOHEADER, 40 bytes)
    writeFour(40);
    writeFour(width);
    writeFour(height);
    writeTwo(1);
    writeTwo(16);  // RGB565
    writeFour(3);  // BI_BITFIELDS
    writeFour(imageBytes);
    writeFour(0);
    writeFour(0);
    writeFour(0);
    writeFour(0);

    // Color masks for RGB565
    writeFour(0xF800);
    writeFour(0x07E0);
    writeFour(0x001F);

    readDisplayTTGO(height, width, rowBuffer, rowsDone, totalRows);
    readDisplayTDisplayS3(height, width, rowBuffer, rowsDone, totalRows);
    readDisplayTDisplayST7789(height, width, rowBuffer, rowsDone, totalRows);

    bmpFile.flush();
    uint32_t finalSize = bmpFile.size();
    bmpFile.close();
    free(rowBuffer);

    screenshotCaptureProgress = 100;

    if (finalSize == 0) setScreenshotMessage("empty file");

    Serial.printf("-->[TFT ] Screenshot saved (%dx%d), bytes written: %lu, file size: %lu\n", width, height, (unsigned long)bytesSaved, (unsigned long)finalSize);
    return finalSize > 0;
}

void screenshotCaptureTask(void *parameter) {
    bool ok = takeScreenshot();
    screenshotCaptureReady = ok;
    screenshotCaptureError = !ok;
    screenshotCaptureInProgress = false;
    screenshotCaptureStartMs = 0;
    if (ok) setScreenshotMessage("ready");
    screenshotTaskHandle = nullptr;
    vTaskDelete(nullptr);
}

bool startScreenshotCaptureAsync() {
    if (screenshotCaptureInProgress) return false;
    screenshotCaptureInProgress = true;
    screenshotCaptureReady = false;
    screenshotCaptureError = false;
    screenshotCaptureProgress = 0;
    screenshotCaptureStartMs = millis();
    setScreenshotMessage("capturing");
#ifdef TTGO_TDISPLAY
    // TTGO: readPixel() from a background task triggers an xQueueGenericSend
    // assertion because the main loop may hold the TFT SPI mutex mid-update.
    // Defer the actual capture to displayShowValues() (main-loop context) where
    // the SPI bus is known to be idle after the display update completes.
    screenshotPendingOnMainLoop = true;
    return true;
#else
    BaseType_t taskCreated = xTaskCreatePinnedToCore(
        screenshotCaptureTask,
        "ScreenshotTask",
        8192,
        nullptr,
        1,
        &screenshotTaskHandle,
        1);
    if (taskCreated != pdPASS) {
        screenshotCaptureInProgress = false;
        screenshotCaptureError = true;
        screenshotCaptureStartMs = 0;
        setScreenshotMessage("task create failed");
        return false;
    }
    return true;
#endif
}

void refreshScreenshotCaptureState() {
    if (!screenshotCaptureInProgress) return;
    if (screenshotCaptureStartMs == 0) return;
    if ((millis() - screenshotCaptureStartMs) < SCREENSHOT_CAPTURE_TIMEOUT_MS) return;

    if (screenshotTaskHandle != nullptr) {
        vTaskDelete(screenshotTaskHandle);
        screenshotTaskHandle = nullptr;
    }
    screenshotCaptureInProgress = false;
    screenshotCaptureReady = false;
    screenshotCaptureError = true;
    screenshotCaptureProgress = 0;
    screenshotCaptureStartMs = 0;
    setScreenshotMessage("timeout");
}

bool resetScreenshotCaptureState() {
    if (screenshotTaskHandle != nullptr) {
        vTaskDelete(screenshotTaskHandle);
        screenshotTaskHandle = nullptr;
    }
    screenshotCaptureInProgress = false;
    screenshotCaptureReady = false;
    screenshotCaptureError = false;
    screenshotCaptureProgress = 0;
    screenshotCaptureStartMs = 0;
    setScreenshotMessage("idle");
    return true;
}

bool isScreenshotCaptureInProgress() {
    refreshScreenshotCaptureState();
    return screenshotCaptureInProgress;
}

bool isScreenshotCaptureReady() {
    refreshScreenshotCaptureState();
    return screenshotCaptureReady;
}

bool hasScreenshotCaptureError() {
    refreshScreenshotCaptureState();
    return screenshotCaptureError;
}

uint8_t getScreenshotCaptureProgress() {
    return screenshotCaptureProgress;
}

const char *getScreenshotCaptureMessage() {
    return screenshotCaptureMessage;
}

void setDisplayBrightness(uint16_t newBrightness) {
#ifdef TTGO_TDISPLAY
    if (actualDisplayBrightness != newBrightness) {
        // Serial.println("\n-->[TFT ] DisplayBrightness (user setting) value at: " + String(DisplayBrightness));
        // Serial.println("-->[TFT ] actualDisplayBrightness value at: " + String(actualDisplayBrightness));
        // Serial.println("-->[TFT ] New display brightness value at: " + String(newBrightness));
        // delay(20);
        analogWrite(TFT_BACKLIGHT, newBrightness);
        actualDisplayBrightness = newBrightness;
    }
#endif
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
    if (actualDisplayBrightness != newBrightness) {
        if (newBrightness == 0) {
            // Hold the backlight pin LOW for >3ms to force the backlight IC into complete shutdown.
            // Simple pulse-down would cycle the IC back to max brightness rather than turning it off.
            // Fixes #218 / #223: display fails to power off on T-Display S3.
            digitalWrite(TFT_BACKLIGHT, LOW);
            delay(5);  // 5ms >> 3ms threshold; IC shuts down completely
            // Pin remains LOW; IC stays off until next wake-up sequence
        } else {
            if (actualDisplayBrightness == 0) {
                // Waking from full shutdown: bring pin HIGH so the IC resets and powers up at
                // maximum brightness (level 16), then step down to the desired level.
                digitalWrite(TFT_BACKLIGHT, HIGH);
                delay(5);  // Allow IC to stabilize at level 16
                actualDisplayBrightness = 16;
            }
            uint16_t dif, i;
            if (newBrightness > actualDisplayBrightness)
                dif = 16 - (newBrightness - actualDisplayBrightness);
            else
                dif = actualDisplayBrightness - newBrightness;
            // Serial.printf("-->[TFT ] Change brightness %d levels\n", dif);
            for (i = 1; i <= dif; i++) {
                digitalWrite(TFT_BACKLIGHT, LOW);
                delayMicroseconds(20);
                digitalWrite(TFT_BACKLIGHT, HIGH);
            }
        }
        actualDisplayBrightness = newBrightness;
    }
#endif
#ifdef ST7789_240x320
    if (actualDisplayBrightness != newBrightness) {
        analogWrite(TFT_BACKLIGHT, newBrightness);
        actualDisplayBrightness = newBrightness;
    }
#endif
}

void displaySleep(bool value)  // https://github.com/Bodmer/TFT_eSPI/issues/715
{
    if (value) {
        tft.writecommand(0x10);  // Send command to put the display to sleep.
        delay(150);              // Delay for shutdown time before another command can be sent.
    } else {
        tft.init();  // This sends the wake up command and initialises the display
    }
}

void turnOffDisplay() {
    setDisplayBrightness(0);  // Turn off the display
}

void displaySplashScreen() {
    uint16_t eMarieteLogoWidth = 118;
    uint16_t eMarieteLogoHeight = 40;
    uint16_t CO2LogoWidth = 92;
    uint16_t CO2LogoHeight = 72;
    uint16_t GadgetLogoWidth = 122;
    uint16_t GadgetLogoHeight = 46;
#if TFT_WIDTH == 135 && TFT_HEIGHT == 240
    uint16_t eMarieteLogoX = 60;
    uint16_t eMarieteLogoY = 12;
    uint16_t CO2LogoX = 10;
    uint16_t CO2LogoY = 50;
    uint16_t GadgetLogoX = 112;
    uint16_t GadgetLogoY = 67;
#endif
#if TFT_WIDTH == 170 && TFT_HEIGHT == 320
    uint16_t eMarieteLogoX = 100;
    uint16_t eMarieteLogoY = 40;
    uint16_t CO2LogoX = 50;
    uint16_t CO2LogoY = 78;
    uint16_t GadgetLogoX = 152;
    uint16_t GadgetLogoY = 95;
#endif
#if TFT_WIDTH == 240 && TFT_HEIGHT == 320
    uint16_t eMarieteLogoX = 100;
    uint16_t eMarieteLogoY = 150;
    uint16_t CO2LogoX = 50;
    uint16_t CO2LogoY = 78;
    uint16_t GadgetLogoX = 152;
    uint16_t GadgetLogoY = 95;
#endif

    tft.fillScreen(TFT_WHITE);
    tft.setSwapBytes(true);
    tft.pushImage(eMarieteLogoX, eMarieteLogoY, eMarieteLogoWidth, eMarieteLogoHeight, eMarieteLogo);
    tft.pushImage(CO2LogoX, CO2LogoY, CO2LogoWidth, CO2LogoHeight, CO2Logo);
    tft.pushImage(GadgetLogoX, GadgetLogoY, GadgetLogoWidth, GadgetLogoHeight, GadgetLogo);
}

void initBacklight() {
#if defined(TTGO_TDISPLAY) || defined(ST7789_240x320)
    pinMode(TFT_BACKLIGHT, OUTPUT);
    // digitalWrite(TFT_BACKLIGHT, 1); Removed to revert as v.0.12.000 to try to fix #192
    setDisplayBrightness(DisplayBrightness);
#endif
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
    pinMode(TFT_BACKLIGHT, OUTPUT);
    // TFT_POWER_ON_BATTERY was already set HIGH in initDisplay() before tft.init();
    // assert it here too so initBacklight() is safe if called independently.
    pinMode(TFT_POWER_ON_BATTERY, OUTPUT);
    digitalWrite(TFT_POWER_ON_BATTERY, HIGH);
    delay(20);  // Pin starts LOW → >3 ms keeps IC in shutdown; then we wake it below
    digitalWrite(TFT_BACKLIGHT, HIGH);
    delay(5);   // Allow backlight IC (DW8904-compatible) to stabilize at level 16
    actualDisplayBrightness = 16;  // IC powers up at maximum level after wakeup from shutdown
    if (DisplayBrightness > 16)    // Prevent malfunction if upper values are stored in preferences
        DisplayBrightness = 16;
    if (DisplayBrightness == 0)    // Prevent permanent black screen: 0 shuts down the IC immediately
        DisplayBrightness = 1;
    setDisplayBrightness(DisplayBrightness);
#endif
}

void setDisplayReverse(bool reverse) {
    if (reverse) {
        Serial.printf("-->[TFT ] Set display reversed\n");
        tft.setRotation(3);
    } else {
        Serial.printf("-->[TFT ] Set display normal\n");
        tft.setRotation(1);
    }
    tft.fillScreen(TFT_BLACK);
    shouldRedrawDisplay = true;
}

void initDisplay(bool fastMode = false) {
    if (!fastMode) Serial.printf("-->[TFT ] Initializing display\n");
    // Display is rotated 90 degrees vs phisical orientation
    displayWidth = TFT_HEIGHT;
    displayHeight = TFT_WIDTH;
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
    // Power on display BEFORE tft.init() so the panel is powered during initialization.
    // On battery-powered devices GPIO15 enables the boost converter; without it the
    // init commands reach an unpowered panel and the display stays blank.
    pinMode(TFT_POWER_ON_BATTERY, OUTPUT);
    digitalWrite(TFT_POWER_ON_BATTERY, HIGH);
#endif
    tft.init();
    setDisplayReverse(displayReverse);
    setElementLocations();
    tft.setTextSize(2);
    if (!fastMode) {
        initBacklight();
        displaySplashScreen();  // Display init and splash screen
        delay(2000);            // Enjoy the splash screen for 2 seconds
    } else {
        tft.fillScreen(TFT_BLACK);
        initBacklight();
    }
    spr.setColorDepth(16);
    spr.setTextWrap(false);
}

// Display a boxed  notification in the display
// parameters:
//      notificationText = string to display.
//      notificationTypes one of enum notificationTypes notifyNothing, notifyInfo, notifyWarning, notifyError
bool displayNotification(String notificationText, notificationTypes notificationType) {
    uint16_t textColor, boxColor, backgroundColor, boxMarging = 15;
    // if (notificationType=1) {
    textColor = TFT_RED;
    boxColor = TFT_SILVER;
    backgroundColor = TFT_BLUE;
    // }

    // TO-DO: Size rectangle to text size
    // int16_t textWidth = tft.textWidth(notificationText);
    // int16_t textHeight = tft.fontHeight();

    tft.setViewport(boxMarging, boxMarging, tft.width() - boxMarging * 2, tft.height() - boxMarging * 2);
    tft.fillRect(0, 0, tft.width(), tft.height(), backgroundColor);
    tft.drawRect(0, 0, tft.width(), tft.height(), boxColor);

    tft.setTextDatum(CC_DATUM);
    tft.setTextColor(textColor, backgroundColor);
    tft.loadFont(SMALL_FONT);
    tft.drawString(notificationText, tft.width() / 2, tft.height() / 2);
    tft.unloadFont();
    tft.resetViewport();
    shouldRedrawDisplay = true;  // Must redraw display to clear the notification
    return true;
}

// Display a boxed two line notification in the display
// parameters:
//      notificationText = string to display.
//      notificationTypes one of enum notificationTypes notifyNothing, notifyInfo, notifyWarning, notifyError
bool displayNotification(String notificationText, String notificationText2, notificationTypes notificationType) {
    uint16_t textColor, boxColor, backgroundColor, boxMarging = 10;
    // if (notificationType=1) {
    textColor = TFT_RED;
    boxColor = TFT_SILVER;
    backgroundColor = TFT_BLUE;
    // }

    // TO-DO: Size rectangle to text size
    int16_t textWidth = tft.textWidth(notificationText);
    int16_t textWidth2 = tft.textWidth(notificationText2);
    // int16_t textHeight = tft.fontHeight();

    tft.setViewport(boxMarging, boxMarging, tft.width() - boxMarging * 2, tft.height() - boxMarging * 2);
    tft.fillRect(0, 0, tft.width(), tft.height(), backgroundColor);
    tft.drawRect(0, 0, tft.width(), tft.height(), boxColor);

    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(textColor, backgroundColor);
    tft.loadFont(SMALL_FONT);
    tft.drawString(notificationText, tft.width() / 2 - textWidth / 2, tft.height() / 5 * 1 + boxMarging);
    tft.drawString(notificationText2, tft.width() / 2 - textWidth2 / 2, tft.height() / 5 * 3 - boxMarging);
    tft.unloadFont();
    tft.resetViewport();
    shouldRedrawDisplay = true;  // Must redraw display to clear the notification
    return true;
}

uint16_t getBatteryColor(float batteryVoltage) {
    uint16_t color;
    if (batteryVoltage <= 3.6) {
        color = TFT_RED;
    } else if (batteryVoltage <= 3.8) {
        color = TFT_ORANGE;
    } else if (!workingOnExternalPower) {
        color = TFT_GREEN;
    } else {
        color = TFT_SKYBLUE;
    }
    return color;
}

void showBatteryVoltage(int32_t posX, int32_t posY, bool forceRedraw) {
    if ((!displayShowBatteryVoltage) || (!displayShowBattery) || (batteryVoltage < 1)) return;
    String batteryVoltageString = " " + String(batteryVoltage, 1) + "V ";
    tft.setTextDatum(TL_DATUM);
    tft.setCursor(posX, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getBatteryColor(batteryVoltage), TFT_BLACK);
    spr.printToSprite(batteryVoltageString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void showBatteryIcon(int32_t posX, int32_t posY, bool forceRedraw) {  // For TTGO T-Display posX=tft.width() - 32, posY=4
    uint16_t color;
    if ((!displayShowBattery) || (batteryVoltage < 1)) return;

    if (batteryLevel < 20) {
        color = TFT_RED;
    } else {
        color = TFT_SILVER;
    }

    if (batteryVoltage > 4.5) {  // Charging...
        color = iconDefaultColor;
    }

    if (spr.createSprite(34, 20) == nullptr) {
        Serial.printf("-->[TFT ] Error: sprite not created, not enough free RAM! Free RAM: %d\n", ESP.getFreeHeap());
        spr.deleteSprite();
        return;
    }

    // publishMQTTLogData("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltageNow) + "V  External power: " + String(workingOnExternalPower));

    spr.fillSprite(TFT_BLACK);

    if (workingOnExternalPower) {
        spr.drawRoundRect(12, 0, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
        spr.setSwapBytes(true);
        spr.drawBitmap(14, 2, iconUSB, 16, 16, TFT_BLACK, iconDefaultColor);
    } else {
        uint32_t upperLine = 4;
        spr.drawRoundRect(0, upperLine, 32, 14, 2, color);  // Battery outter rectangle
        spr.drawLine(33, upperLine + 4, 33, upperLine + 10, color);
        if (batteryLevel > 20) spr.fillRect(4, upperLine + 2, 4, 10, color);
        if (batteryLevel > 40) spr.fillRect(11, upperLine + 2, 4, 10, color);
        if (batteryLevel > 60) spr.fillRect(18, upperLine + 2, 4, 10, color);
        if (batteryLevel > 80) spr.fillRect(25, upperLine + 2, 4, 10, color);
    }

    spr.pushSprite(posX, posY);
    spr.deleteSprite();
}

void showWiFiIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowStatusIcons) return;
    bool wifiStatusActive = activeWIFI;
#ifdef SUPPORT_LOW_POWER
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode) {
        wifiStatusActive = deepSleepData.activeWifiOnWake;
    }
#endif
    if (!wifiStatusActive) return;

    // If captivePortalActive = true; draw a white circle instead of the WiFi icon. If forceCaptivePortalActive is also true, draw it blue

    if (captivePortalActive) {
        tft.drawRect(posX - 2, posY - 2, 16 + 4, 16 + 4, TFT_BLACK);
        if (forceCaptivePortalActive) {
            tft.fillCircle(posX + 8, posY + 8, 6, TFT_BLUE);
        } else {
            tft.fillCircle(posX + 8, posY + 8, 6, TFT_WHITE);
        }
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    int16_t rssi = getWiFiRSSIForStatus();
    if (troubledWIFI) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (deepSleepData.lastWifiRSSIValid) {
        int16_t signalStrength = abs(rssi);
        if (signalStrength < 60)
            tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
        else if (signalStrength < 70)
            tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_ORANGE);
        else if (signalStrength < 80)
            tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_YELLOW);
        else
            tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_BLUE);
    } else {
        tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_BLUE);
    }
}

void showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowStatusIcons) return;
    bool bleStatusActive = enableBLE && activeBLE;
#ifdef SUPPORT_LOW_POWER
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode) {
        bleStatusActive = deepSleepData.activeBLEOnWake && enableBLE && activeBLE;
    }
#endif
    if (!bleStatusActive) return;

    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, iconDefaultColor);
}

void showBTHomeIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowStatusIcons) return;
#ifdef SUPPORT_BTHOME_BLE
    bool bthomeStatusActive = enableBLE && activeBTHome;
#ifdef SUPPORT_LOW_POWER
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode) {
        bthomeStatusActive = deepSleepData.activeBLEOnWake && enableBLE && activeBTHome;
    }
#endif
    if (!bthomeStatusActive) return;

    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    tft.drawBitmap(posX, posY, iconBTHome, 16, 16, TFT_BLACK, iconDefaultColor);
#endif
}

void showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowStatusIcons) return;
    bool mqttStatusActive = activeMQTT;
#ifdef SUPPORT_LOW_POWER
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode) {
        mqttStatusActive = deepSleepData.sendMQTTOnWake;
    }
#endif
    if (!mqttStatusActive) return;

    if (troubledMQTT) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
}

void showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowStatusIcons) return;
#ifdef SUPPORT_ESPNOW
    bool espNowStatusActive = activeESPNOW;
#ifdef SUPPORT_LOW_POWER
    if ((esp_reset_reason() == ESP_RST_DEEPSLEEP) && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) && !interactiveMode) {
        espNowStatusActive = deepSleepData.sendESPNowOnWake;
    }
#endif
    if (!espNowStatusActive) return;

    if (troubledESPNOW) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, iconDefaultColor);
#endif
}

void clearStatusIconArea() {
    int32_t clearWidth = elementPosition.espNowIconX + 22;
    tft.fillRect(0, 0, clearWidth, 22, TFT_BLACK);
}

void showTemperatureIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowTemperature) return;
    tft.setSwapBytes(true);
    tft.pushImage(posX, posY, 16, 16, iconTemperature);
}

uint16_t getTemperatureColor(float temp) {
    uint16_t color;
    if (temp >= 30) {
        color = TFT_ORANGE;
    } else if (temp >= 10) {
        color = TFT_LIGHTGREY;
    } else {
        color = TFT_SKYBLUE;
    }
    return color;
}

void showTemperature(float temp, int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowTemperature) return;
    showTemperatureIcon(posX, posY, forceRedraw);
    String temperatureString = (showFahrenheit ? String(tempFahrenheit, 1) + "°F" : String(temp, 1)) + "°C";
    tft.setCursor(posX + 18, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getTemperatureColor(temp), TFT_BLACK);
    spr.printToSprite(temperatureString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void showHumidityIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowHumidity) return;
    tft.setSwapBytes(true);
    tft.pushImage(posX, posY, 16, 16, iconHumidity);
}

uint16_t getHumidityColor(float hum) {
    uint16_t color;
    if (hum <= 25) {
        color = TFT_RED;
    } else if (hum < 40) {
        color = TFT_ORANGE;
    } else if (hum <= 60) {
        color = TFT_LIGHTGREY;
    } else if (hum < 75) {
        color = TFT_SKYBLUE;
    } else {
        color = TFT_BLUE;
    }
    return color;
}

void showHumidity(float hum, int32_t posX, int32_t posY, bool forceRedraw) {
    if (!displayShowHumidity) return;
    showHumidityIcon(posX, posY, forceRedraw);
    String humidityString = String(hum, 0) + "% ";
    tft.setCursor(posX + 18, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getHumidityColor(hum), TFT_BLACK);
    spr.printToSprite(humidityString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

uint16_t getCO2Color(uint16_t co2) {
    uint16_t color;
    if (co2 < co2OrangeRange) {
        color = TFT_GREEN;
    } else if (co2 < co2RedRange) {
        color = TFT_ORANGE;
    } else {
        color = TFT_RED;
    }
    return color;
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY, uint16_t pixelsToBaseline, bool forceRedraw) {
    if (!forceRedraw && (co2 == previousCO2Value)) return;
    if ((co2 == 0) || (co2 > 9999)) return;

    spr.loadFont(BIG_FONT);
    uint16_t digitWidth = spr.textWidth("0");
    uint16_t height = spr.fontHeight() - pixelsToBaseline;
    uint16_t totalWidth = digitWidth * 4;  // Four digits
    uint16_t posSpriteY = posY - height;
    uint16_t color = getCO2Color(co2);
    if (posSpriteY < 0) posSpriteY = 0;
    // spr.createSprite(digitWidth, height);
    if (spr.createSprite(digitWidth, height) == nullptr) {
        // Serial.printf("-->[TFT ] Error: sprite not created, not enough free RAM! Free RAM: %d\n", ESP.getFreeHeap());
        spr.unloadFont();
        spr.deleteSprite();
        return;
    }
    spr.setTextColor(color, TFT_BLACK);
    spr.setTextDatum(TR_DATUM);

    // Store the last CO2 digits in an array
    uint8_t lastCO2ValueDigits[4];
    for (int i = 0; i < 4; ++i) {
        lastCO2ValueDigits[i] = previousCO2Value % 10;
        previousCO2Value /= 10;
    }

    for (int i = 0; i < 4; ++i) {
        uint16_t digit = co2 % 10;  // Get the rightmost digit
        co2 /= 10;                  // Move to the next digit

        // if (digit == lastCO2ValueDigits[i]) continue;  // Skip if the digit is equal to the corresponding digit of previousCO2Value
        spr.fillSprite(TFT_BLACK);
        if ((i == 3) && (digit == 0)) {  // Don't draw leading zero and fill black
            spr.pushSprite(posX - totalWidth + digitWidth * (3 - i), posSpriteY);
        } else {
            spr.drawNumber(digit, digitWidth, 0);
            uint16_t posSpriteX = posX - totalWidth + digitWidth * (3 - i);  // Calculate X position for the sprite
            // if (posSpriteX < 0) posSpriteX = 0;
            spr.pushSprite(posSpriteX, posSpriteY);
        }
    }

    spr.deleteSprite();  // Clear sprite memory
    spr.unloadFont();
}

void showCO2units(int32_t posX, int32_t posY, bool forceRedraw) {
    spr.loadFont(MINI_FONT);
    spr.setTextColor(getCO2Color(co2), TFT_BLACK);
    tft.setCursor(posX, posY);
    spr.printToSprite("ppm");
    spr.unloadFont();
}

void displayShowValues(bool forceRedraw = false) {
    if (forceRedraw) {
        thresholdsManager.updatePreviousValues(DISPLAY_SHOW, co2, temp, hum);
    } else {
        if (!thresholdsManager.evaluateThresholds(DISPLAY_SHOW, co2, temp, hum)) return;
    }
    uint8_t currentDatum = tft.getTextDatum();
    if (redrawDisplayOnNextLoop) {
        shouldRedrawDisplay = true;
        redrawDisplayOnNextLoop = false;
    }
    if (shouldRedrawDisplay) {
        forceRedraw = true;
        Serial.println("-->[TFT ] Forcing display redraw");        
    }
    tft.unloadFont();
    if (forceRedraw) {
        Serial.println("-->[TFT ] Displaying values. Force Redraw: " + String(forceRedraw ? "true" : "false"));
        tft.fillScreen(TFT_BLACK);  // Remove previous remains in the screen
    }
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, elementPosition.pixelsToBaseline, forceRedraw);
    showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY, forceRedraw);
    showTemperature(temp, elementPosition.tempX, elementPosition.tempY, forceRedraw);
    showHumidity(hum, elementPosition.humidityX, elementPosition.humidityY, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, forceRedraw);
    showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    clearStatusIconArea();
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
#ifdef SUPPORT_BTHOME_BLE
    showBTHomeIcon(elementPosition.bthomeIconX, elementPosition.bthomeIconY, forceRedraw);
#endif
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    forceRedraw = false;
    shouldRedrawDisplay = false;

    // Revert the datum setting
    tft.loadFont(SMALL_FONT);

    tft.setTextDatum(currentDatum);
    tft.setTextSize(2);

#ifdef TTGO_TDISPLAY
    // Run deferred screenshot here — SPI bus is idle after the display update.
    if (screenshotPendingOnMainLoop) {
        screenshotPendingOnMainLoop = false;
        bool ok = takeScreenshot();
        screenshotCaptureReady = ok;
        screenshotCaptureError = !ok;
        screenshotCaptureInProgress = false;
        screenshotCaptureStartMs = 0;
        if (ok) setScreenshotMessage("ready");
    }
#endif
}

#endif  // SUPPORT_TFT
#endif  // CO2_Gadget_TFT_h
