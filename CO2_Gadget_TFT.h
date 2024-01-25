

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
#include "FontNotoSansBold90ptDigits.h"
#include "FontNotoSansRegular15pt_mp.h"
#include "FontNotoSansRegular20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansRegular15pt_mp
#define SMALL_FONT FontNotoSansRegular20
#define BIG_FONT FontNotoSansBold90ptDigits
#define FONTS_LOADED
#endif
#endif

// Load fonts for T-Display-S3 and others with 320x170 resolution
#if defined(TFT_WIDTH) && defined(TFT_HEIGHT)
#if TFT_WIDTH == 170 && TFT_HEIGHT == 320
#include "FontNotoSansBold120ptDigits.h"
#include "FontNotoSansRegular15pt_mp.h"
#include "FontNotoSansRegular20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansRegular15pt_mp
#define SMALL_FONT FontNotoSansRegular20
#define BIG_FONT FontNotoSansBold120ptDigits
#define FONTS_LOADED
#endif
#endif

// Default fonts
#ifndef FONTS_LOADED
#include "FontNotoSansBold90ptDigits.h"
#include "FontNotoSansRegular15pt_mp.h"
#include "FontNotoSansRegular20.h"
#define GFXFF 1
#define MINI_FONT FontNotoSansRegular15pt_mp
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
        elementPosition.co2Y = displayHeight - 38;
        elementPosition.co2FontDigitsHeight = 70;  // Digits height for the font used (not the same as whole font height)
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempX = 1;
        elementPosition.tempY = displayHeight - 25;
        elementPosition.humidityX = displayWidth - 60;
        elementPosition.humidityY = displayHeight - 25;
        elementPosition.batteryIconX = displayWidth - 36;
        elementPosition.batteryIconY = 4;
        elementPosition.batteryVoltageX = displayWidth - 92;
        elementPosition.batteryVoltageY = 2;
        elementPosition.bleIconX = 2;
        elementPosition.bleIconY = 1;
        elementPosition.wifiIconX = 26;
        elementPosition.wifiIconY = 1;
        elementPosition.mqttIconX = 50;
        elementPosition.mqttIconY = 1;
        elementPosition.espNowIconX = 74;
        elementPosition.espNowIconY = 1;
    }

    if (displayWidth == 320 && displayHeight == 170) {  // T-Display-S3 and similar
        elementPosition.co2X = displayWidth - 33;
        elementPosition.co2Y = displayHeight - 15;
        elementPosition.co2FontDigitsHeight = 100;      // Digits height for the font used (not the same as whole font height)
        elementPosition.co2UnitsX = displayWidth - 33;
        elementPosition.co2UnitsY = displayHeight - 50;
        elementPosition.tempX = 1;
        elementPosition.tempY = displayHeight - 25;
        elementPosition.humidityX = displayWidth - 60;
        elementPosition.humidityY = displayHeight - 25;
        elementPosition.batteryIconX = displayWidth - 36;
        elementPosition.batteryIconY = 4;
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

// void setDisplayBrightness(uint32_t newBrightness) {
//     Serial.printf("-->[TFT ] Actual display brightness value at %d\n", actualDisplayBrightness);
//     Serial.printf("-->[TFT ] Setting display brightness value at %d\n", newBrightness);
//     ledcWrite(BACKLIGHT_PWM_CHANNEL, newBrightness);  // 0-15, 0-255 (with 8 bit resolution); 0=totally
//                                                       // dark;255=totally shiny
//     actualDisplayBrightness = newBrightness;
// }

void setDisplayBrightness(uint32_t newBrightness) {
// TO-DO: Fix this
#ifdef TTGO_TDISPLAY
    Serial.printf("-->[TFT ] Actual display brightness value at %d\n", actualDisplayBrightness);
    Serial.printf("-->[TFT ] Setting display brightness value at %d\n", newBrightness);
    ledcWrite(BACKLIGHT_PWM_CHANNEL, newBrightness);  // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=max brightness
    Serial.printf("-->[TFT ] Actual display brightness value (ledcRead) at %d\n", ledcRead(BACKLIGHT_PWM_CHANNEL));
    Serial.printf("-->[TFT ] newBrightness value at %d\n", newBrightness);
    actualDisplayBrightness = newBrightness;
    Serial.printf("-->[TFT ] Actual display brightness value at %d\n", actualDisplayBrightness);
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

    tft.fillScreen(TFT_WHITE);
    tft.setSwapBytes(true);
    tft.pushImage(eMarieteLogoX, eMarieteLogoY, eMarieteLogoWidth, eMarieteLogoHeight, eMarieteLogo);
    tft.pushImage(CO2LogoX, CO2LogoY, CO2LogoWidth, CO2LogoHeight, CO2Logo);
    tft.pushImage(GadgetLogoX, GadgetLogoY, GadgetLogoWidth, GadgetLogoHeight, GadgetLogo);
}

// void displaySplashScreen() {
//     tft.fillScreen(TFT_WHITE);
//     tft.setSwapBytes(true);
//     tft.pushImage(60, 12, 118, 40, eMarieteLogo);
//     tft.pushImage(10, 50, 92, 72, CO2Logo);
//     tft.pushImage(112, 67, 122, 46, GadgetLogo);
// }

void initBacklight() {
#ifdef TTGO_TDISPLAY
    pinMode(TFT_BL, OUTPUT);
    ledcSetup(BACKLIGHT_PWM_CHANNEL, BACKLIGHT_PWM_FREQUENCY, 8);  // 0-15, 5000, 8
    ledcAttachPin(TFT_BL, BACKLIGHT_PWM_CHANNEL);                  // TFT_BL, 0 - 15
    setDisplayBrightness(DisplayBrightness);
#endif
#ifdef TDISPLAY_S3
    pinMode(TFT_BL, OUTPUT);
    pinMode(TFT_BACKLIGHT_ON, OUTPUT);
    delay(50);
    digitalWrite(TFT_BL, HIGH);
    digitalWrite(TFT_BACKLIGHT_ON, HIGH);
#endif
}

void initDisplay() {
    Serial.printf("-->[TFT ] Initializing display\n");
    initBacklight();
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

uint16_t getBatteryColor(uint16_t battery_voltage) {
    uint16_t color;
    if (battery_voltage <= 3.6) {
        color = TFT_RED;
    } else if (battery_voltage <= 3.8) {
        color = TFT_ORANGE;
    } else if (battery_voltage <= 4.5) {
        color = TFT_GREEN;
    } else {
        color = TFT_SKYBLUE;
    }
    return color;
}

void showBatteryVoltage(int32_t posX, int32_t posY) {
    String batteryVoltageString = " " + String(battery_voltage, 1) + "V ";
    tft.setTextDatum(TL_DATUM);
    tft.setCursor(posX, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getBatteryColor(battery.voltage()), TFT_BLACK);
    spr.printToSprite(batteryVoltageString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void showBatteryIcon(int32_t posX, int32_t posY) {  // For TTGO T-Display posX=tft.width() - 32, posY=4
    if (!displayShowBattery) return;
    uint8_t batteryLevel = battery.level();
    uint16_t color;
    if (batteryLevel < 20) {
        color = TFT_RED;
    } else {
        color = TFT_SILVER;
    }

    if (battery_voltage > 4.5) {  // Charging...
        color = iconDefaultColor;
    }

    tft.drawRoundRect(posX, posY, 32, 14, 2, color);  // Battery outter rectangle
    tft.drawLine(posX + 33, posY + 4, posX + 33, posY + 10, color);

    if (batteryLevel > 20) tft.fillRect(posX + 4, posY + 2, 4, 10, color);
    if (batteryLevel > 40) tft.fillRect(posX + 11, posY + 2, 4, 10, color);
    if (batteryLevel > 60) tft.fillRect(posX + 18, posY + 2, 4, 10, color);
    if (batteryLevel > 80) tft.fillRect(posX + 25, posY + 2, 4, 10, color);
}

void showWiFiIcon(int32_t posX, int32_t posY) {
    int8_t rssi = WiFi.RSSI();
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeWIFI) {
        tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        if (WiFi.status() == WL_CONNECTED) {
            if (rssi < 60)
                tft.drawBitmap(posX, posY, iconWiFi, 16, 16, TFT_BLACK, iconDefaultColor);
            else if (rssi < 70)
                tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_ORANGE);
            else if (rssi < 90)
                tft.drawBitmap(posX, posY, iconWiFiMed, 16, 16, TFT_BLACK, TFT_YELLOW);
        } else {
            tft.drawBitmap(posX, posY, iconWiFiLow, 16, 16, TFT_BLACK, TFT_RED);
        }
    }
}

void showBLEIcon(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeBLE) {
        tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconBLE, 16, 16, TFT_BLACK, iconDefaultColor);
    }
}

void showMQTTIcon(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeMQTT) {
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconMQTT, 16, 16, TFT_BLACK, iconDefaultColor);
    }
}

void showEspNowIcon(int32_t posX, int32_t posY) {
    tft.drawRoundRect(posX - 2, posY - 2, 16 + 4, 16 + 4, 2, TFT_DARKGREY);
    if (!activeESPNOW) {
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, TFT_DARKGREY);
    } else {
        tft.drawBitmap(posX, posY, iconEspNow, 16, 16, TFT_BLACK, iconDefaultColor);
    }
}

void showTemperatureIcon(int32_t posX, int32_t posY) {
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

void showTemperature(float temp, int32_t posX, int32_t posY) {
    if (!displayShowTemperature) return;
    showTemperatureIcon(posX, posY);
    String temperatureString = (showFahrenheit ? String(tempFahrenheit, 1) : String(temp, 1)) + "° ";
    tft.setCursor(posX + 18, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getTemperatureColor(temp), TFT_BLACK);
    spr.printToSprite(temperatureString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void showHumidityIcon(int32_t posX, int32_t posY) {
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

void showHumidity(float hum, int32_t posX, int32_t posY) {
    if (!displayShowHumidity) return;
    showHumidityIcon(posX, posY);
    String humidityString = String(hum, 0) + "% ";
    tft.setCursor(posX + 18, posY);
    spr.loadFont(SMALL_FONT);
    spr.setTextColor(getHumidityColor(hum), TFT_BLACK);
    spr.printToSprite(humidityString);  // Space padding helps over-write old numbers
    spr.unloadFont();
}

void OLDshowHumidity(float hum, int32_t posX, int32_t posY) {
    if (!displayShowHumidity) return;
    showHumidityIcon(posX - 20, posY - 2);
    tft.setTextColor(getHumidityColor(hum), TFT_BLACK);
    tft.setTextDatum(BR_DATUM);
    showHumidityIcon(tft.width() - 60, tft.height() - 22);
    tft.loadFont(SMALL_FONT);
    tft.drawString(String(hum, 0) + "%", posX, posY);
    tft.unloadFont();
}

uint16_t getCO2Color(uint16_t co2) {
    uint16_t color;
    if (co2 < 800) {
        color = TFT_GREEN;
    } else if (co2 < 1000) {
        color = TFT_YELLOW;
    } else if (co2 < 1200) {
        color = TFT_ORANGE;
    } else {
        color = TFT_RED;
    }
    return color;
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY) {
    spr.loadFont(BIG_FONT);
    uint16_t width = spr.textWidth("0000")+2;
    uint16_t height = elementPosition.co2FontDigitsHeight;
    uint16_t posSpriteX = posX - width;
    uint16_t posSpriteY = posY - height;
    if (posSpriteX < 0) posSpriteX = 0;
    if (posSpriteY < 0) posSpriteY = 0;
    spr.createSprite(width, height);
    spr.drawRect(0, 0, width, height, TFT_WHITE);
    spr.setTextColor(getCO2Color(co2), TFT_BLACK);
    // spr.drawString(String(co2), width, height);
    // spr.printToSprite( (String) co2);
    spr.drawNumber(co2, 0, 0);
    spr.pushSprite(posSpriteX, posSpriteY);
    spr.unloadFont();
    spr.deleteSprite();
}

void showCO2units(int32_t posX, int32_t posY) {
    tft.setTextDatum(BL_DATUM);
    spr.loadFont(MINI_FONT);
    spr.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(posX, posY);
    spr.printToSprite("ppm");
    spr.unloadFont();
}

void displayShowValues() {
    uint8_t currenttDatum = tft.getTextDatum();
    showCO2(co2, elementPosition.co2X, elementPosition.co2Y);
    showCO2units(elementPosition.co2UnitsX, elementPosition.co2UnitsY);
    showTemperature(temp, elementPosition.tempX, elementPosition.tempY);
    showHumidity(hum, elementPosition.humidityX, elementPosition.humidityY);
    showBatteryIcon(elementPosition.batteryIconX, elementPosition.batteryIconY);
    showBatteryVoltage(elementPosition.batteryVoltageX, elementPosition.batteryVoltageY);
    showWiFiIcon(elementPosition.wifiIconX, elementPosition.wifiIconY);
    showMQTTIcon(elementPosition.mqttIconX, elementPosition.mqttIconY);
    showBLEIcon(elementPosition.bleIconX, elementPosition.bleIconY);
    showEspNowIcon(elementPosition.espNowIconX, elementPosition.espNowIconY);

    // Revert the datum setting
    tft.setTextDatum(currenttDatum);
    tft.setTextSize(2);
}

#endif  // SUPPORT_TFT
#endif  // CO2_Gadget_TFT_h