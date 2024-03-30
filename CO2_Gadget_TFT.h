

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
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
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
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 2;
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
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 2;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 2;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 2;
    }
}

void setDisplayBrightness(uint16_t newBrightness) {
#ifdef TTGO_TDISPLAY
    if (actualDisplayBrightness != newBrightness) {
        Serial.printf("\n-->[TFT ] DisplayBrightness value at %d\n", DisplayBrightness);
        Serial.printf("-->[TFT ] actualDisplayBrightness value at %d\n", actualDisplayBrightness);
        Serial.printf("-->[TFT ] New display brightness value at %d\n", newBrightness);
        analogWrite(TFT_BACKLIGHT, newBrightness);
        actualDisplayBrightness = newBrightness;
    }
#endif
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
    if (actualDisplayBrightness != newBrightness) {
        uint16_t dif, i;
        if (newBrightness > actualDisplayBrightness)
            dif = 16 - (newBrightness - actualDisplayBrightness);
        else
            dif = actualDisplayBrightness - newBrightness;

        // Serial.printf("-->[TFT ] Change brightness %d levels", dif);
        for (i = 1; i <= dif; i++) {
            digitalWrite(TFT_BACKLIGHT, LOW);
            delayMicroseconds(20);
            digitalWrite(TFT_BACKLIGHT, HIGH);
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
    digitalWrite(TFT_BACKLIGHT, 1);
    setDisplayBrightness(DisplayBrightness);
#endif
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
    pinMode(TFT_BACKLIGHT, OUTPUT);
    pinMode(TFT_POWER_ON_BATTERY, OUTPUT);
    delay(20);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    digitalWrite(TFT_POWER_ON_BATTERY, HIGH);
    actualDisplayBrightness = 16;  // At the beginning brightness is at maximum level
    if (DisplayBrightness > 16)    // Prevent malfunction if upper values are stored in preferences
        DisplayBrightness = 16;
    setDisplayBrightness(DisplayBrightness);
#endif
}

void initDisplay() {
    Serial.printf("-->[TFT ] Initializing display\n");
    // Display is rotated 90 degrees vs phisical orientation
    displayWidth = TFT_HEIGHT;
    displayHeight = TFT_WIDTH;
    tft.init();
    if (displayReverse) {
        tft.setRotation(3);
    } else {
        tft.setRotation(1);
    }
    setElementLocations();
    tft.setTextSize(2);
    initBacklight();
    displaySplashScreen();  // Display init and splash screen
    delay(2000);            // Enjoy the splash screen for 2 seconds
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

    publishMQTTLogData("-->[TFT ] Battery Level: " + String(batteryLevel) + "%   Battery voltage: " + String(batteryVoltage) + "V");

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
    int8_t rssi = WiFi.RSSI();
    if (troubledWIFI) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeWIFI) {
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        if (WiFi.status() == WL_CONNECTED) {
            if (rssi < 60)
                tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
            else if (rssi < 70)
                tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_ORANGE);
            else if (rssi < 80)
                tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_YELLOW);
        } else {
            tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_BLUE);
        }
    }
}

void showBLEIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeBLE) {
        tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, iconDefaultColor);
    }
}

void showMQTTIcon(int32_t posX, int32_t posY, bool forceRedraw) {
    if (troubledMQTT) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeMQTT) {
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
    }
}

void showEspNowIcon(int32_t posX, int32_t posY, bool forceRedraw) {
#ifdef SUPPORT_ESPNOW
    if (troubledESPNOW) {
        tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_RED);
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, iconDefaultColor);
        return;
    }
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeESPNOW) {
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, iconDefaultColor);
    }
#endif
}

void showTemperatureIcon(int32_t posX, int32_t posY, bool forceRedraw) {
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
    String temperatureString = (showFahrenheit ? String(tempFahrenheit, 1) : String(temp, 1)) + "° ";
    tft.setCursor(posX + 18, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getTemperatureColor(temp), TFT_BLACK);
    spr.printToSprite(temperatureString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void showHumidityIcon(int32_t posX, int32_t posY, bool forceRedraw) {
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
    uint8_t currentDatum = tft.getTextDatum();
    tft.unloadFont();
    if (forceRedraw) {
        // tft.fillScreen(TFT_BLACK);
    }
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y, elementPosition.pixelsToBaseline, forceRedraw);
    showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY, forceRedraw);
    showTemperature(temp, elementPosition.tempX, elementPosition.tempY, forceRedraw);
    showHumidity(hum, elementPosition.humidityX, elementPosition.humidityY, forceRedraw);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY, forceRedraw);
    showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY, forceRedraw);
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY, forceRedraw);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY, forceRedraw);
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY, forceRedraw);
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY, forceRedraw);
    forceRedraw = false;

    // Revert the datum setting
    tft.loadFont(SMALL_FONT);

    tft.setTextDatum(currentDatum);
    tft.setTextSize(2);
}

#endif  // SUPPORT_TFT
#endif  // CO2_Gadget_TFT_h