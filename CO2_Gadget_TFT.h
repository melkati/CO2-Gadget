

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

#include "FontArchivoNarrow_Regular10pt7b.h"
#include "FontCO2Gadget50ptDigits.h"
#include "bootlogo.h"
#include "icons.h"

// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/
#define GFXFF 1
#define FF90 &ArchivoNarrow_Regular10pt7b
#define FF95 &FontCO2GadgetDigits50pt7b

// RAM:   [===       ]  29.6% (used 97060 bytes from 327680 bytes)
// Flash: [=======   ]  71.8% (used 1411157 bytes from 1966080 bytes)

uint16_t iconDefaultColor = TFT_CYAN;

TFT_eSPI tft = TFT_eSPI(SCREEN_HEIGHT, SCREEN_WIDTH);  // Invoke library, pins defined in platformio.ini

// Define a structure for the locations of elements
struct ElementLocations {
    int32_t co2X;
    int32_t co2Y;
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
#if SCREEN_WIDTH == 240 && SCREEN_HEIGHT == 135
    elementPosition.co2X = SCREEN_WIDTH - 39;
    elementPosition.co2Y = 112;
    elementPosition.co2UnitsX = 4;
    elementPosition.co2UnitsY = 102;
    elementPosition.tempX = 22;
    elementPosition.tempY = SCREEN_HEIGHT - 2;
    elementPosition.humidityX = SCREEN_WIDTH - 6;
    elementPosition.humidityY = SCREEN_HEIGHT - 2;
    elementPosition.batteryIconX = SCREEN_WIDTH - 36;
    elementPosition.batteryIconY = 4;
    elementPosition.batteryVoltageX = SCREEN_WIDTH - 45;
    elementPosition.batteryVoltageY = 3;
    elementPosition.bleIconX = 2;
    elementPosition.bleIconY = 2;
    elementPosition.wifiIconX = 26;
    elementPosition.wifiIconY = 2;
    elementPosition.mqttIconX = 50;
    elementPosition.mqttIconY = 2;
    elementPosition.espNowIconX = 74;
    elementPosition.espNowIconY = 2;
#endif
}

void setDisplayBrightness(uint32_t newBrightness) {
    Serial.printf("-->[TFT ] Actual display brightness value at %d\n", actualDisplayBrightness);
    Serial.printf("-->[TFT ] Setting display brightness value at %d\n", newBrightness);
    ledcWrite(BACKLIGHT_PWM_CHANNEL, newBrightness);  // 0-15, 0-255 (with 8 bit resolution); 0=totally
                                                      // dark;255=totally shiny
    Serial.printf("-->[TFT ] Actual display brightness value (ledcRead) at %d\n", ledcRead(BACKLIGHT_PWM_CHANNEL));
    actualDisplayBrightness = DisplayBrightness;
}

void turnOffDisplay() {
    setDisplayBrightness(0);  // Turn off the display
    actualDisplayBrightness = 0;
}

void displaySplashScreen() {
    tft.fillScreen(TFT_WHITE);
    tft.setSwapBytes(true);
    tft.pushImage(60, 12, 118, 40, eMarieteLogo);
    tft.pushImage(10, 50, 92, 72, CO2Logo);
    tft.pushImage(112, 67, 122, 46, GadgetLogo);
}

void initDisplay() {
    Serial.printf("-->[TFT ] Initializing display\n");
    pinMode(BACKLIGHT_PIN, OUTPUT);
    ledcSetup(BACKLIGHT_PWM_CHANNEL, BACKLIGHT_PWM_FREQUENCY, 8);  // 0-15, 5000, 8
    ledcAttachPin(BACKLIGHT_PIN, BACKLIGHT_PWM_CHANNEL);           // TFT_BL, 0 - 15
    setDisplayBrightness(DisplayBrightness);
    tft.init();
    if (displayReverse) {
        tft.setRotation(3);
    } else {
        tft.setRotation(1);
    }

    setElementLocations();

    displaySplashScreen();  // Display init and splash screen
    delay(2000);            // Enjoy the splash screen for 2 seconds
    tft.setTextSize(2);
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
    tft.drawString(notificationText, tft.width() / 2, tft.height() / 2);
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
    tft.drawString(notificationText, tft.width() / 2 - textWidth / 2, tft.height() / 5 * 1 + boxMarging);
    tft.drawString(notificationText2, tft.width() / 2 - textWidth2 / 2, tft.height() / 5 * 3 - boxMarging);
    tft.resetViewport();
    return true;
}

void showBatteryVoltage(int32_t posX, int32_t posY) {
    tft.setTextDatum(TR_DATUM);
    tft.setTextColor(TFT_SILVER, TFT_BLACK);
    tft.drawString(String(battery_voltage, 1) + "V", posX, posY);
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
    tft.drawLine(posX + 36, posY + 4, posX + 33, posY + 10, color);

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

void showTemperature(float temp, int32_t posX, int32_t posY) {
    if (!displayShowTemperature) return;
    if (temp >= 30)
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    else if (temp >= 10)
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    else
        tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.setTextDatum(BL_DATUM);
    tft.setSwapBytes(true);
    tft.pushImage(2, tft.height() - 22, 16, 16, iconTemperature);
    if (showFahrenheit) {
        tft.drawString(String(tempFahrenheit, 1) + "~", posX, posY);  // The "~" symbol has been redefined in custom font as the degree symbol
    } else {
        tft.drawString(String(temp, 1) + "~", posX, posY);
    }
}

void showHumidity(float hum, int32_t posX, int32_t posY) {
    if (!displayShowHumidity) return;
    if (hum <= 25)
        tft.setTextColor(TFT_WHITE, TFT_RED);
    else if (hum < 40)
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    else if (hum <= 60)
        tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    else if (hum < 75)
        tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    else
        tft.setTextColor(TFT_RED, TFT_SKYBLUE);
    tft.setTextDatum(BR_DATUM);
    tft.setSwapBytes(true);
    tft.pushImage(tft.width() - 60, tft.height() - 22, 16, 16, iconHumidity);
    tft.drawString(String(hum, 0) + "%", posX, posY);
}

void showVoltage(int32_t posX, int32_t posY) {
    // Draw Voltaje number
    uint16_t battery_voltage = battery.voltage();
    if (battery_voltage <= 3.6) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    } else if (battery_voltage <= 3.8) {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    } else if (battery_voltage <= 4.5) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
        tft.setTextColor(TFT_BLUE, TFT_BLACK);
    }
    tft.setTextDatum(TR_DATUM);
    tft.drawString(String(battery_voltage, 1) + "V", posX, posY);
}

void showCO2(uint16_t co2, int32_t posX, int32_t posY) {
    if (co2 > 9999) {
        co2 = 9999;
    }
    if (co2 >= co2RedRange) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    } else if (co2 >= co2OrangeRange) {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    } else {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    tft.setTextSize(1);
    tft.setTextDatum(BR_DATUM);  // bottom right
    tft.setFreeFont(FF95);
    tft.drawString(String(co2), posX, posY);
}

void showCO2units(int32_t posX, int32_t posY) {
    tft.setTextDatum(BR_DATUM);  // bottom right
    tft.setFreeFont(FF90);
    tft.drawString("ppm", tft.width() - posX, posY);
}

void displayShowValues(uint16_t co2) {
    tft.fillScreen(TFT_BLACK);
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

    // Set default font for the menu
    tft.setFreeFont(NULL);
    tft.setTextSize(2);
}

#endif  // SUPPORT_TFT
#endif  // CO2_Gadget_TFT_h