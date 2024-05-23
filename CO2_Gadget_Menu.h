#ifndef CO2_Gadget_Menu_h
#define CO2_Gadget_Menu_h

// Based on
// https://drive.google.com/file/d/1_qGqs3XpFQRoT-u5-GK8aJk6f0aI7EA3/view?usp=drive_web

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                SETUP ARDUINOMENU                                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <menu.h>
#include <menuIO/serialIO.h>

#ifdef SUPPORT_TFT
#include <menuIO/TFT_eSPIOut.h>
#endif

#ifdef SUPPORT_EINK
#include <GxEPD2Out.h>
#endif

#ifdef SUPPORT_OLED
#include <menuIO/chainStream.h>
#include <menuIO/u8g2Out.h>
#endif

#include <menuIO/esp8266Out.h>  //must include this even if not doing web output...

using namespace Menu;

String rightPad(const String &aString, uint8_t aLength) {
    String paddedString = aString;
    while (paddedString.length() < aLength) {
        paddedString += ' ';
    }

#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Original String: #%s#\n", aString.c_str());
    Serial.printf("-->[MENU] Padded String: #%s#\n", paddedString.c_str());
#endif

    return paddedString;
}

// customizing a menu prompt look
class confirmReboot : public menu {
   public:
    confirmReboot(constMEM menuNodeShadow &shadow) : menu(shadow) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t p) override {
        return idx < 0 ?  // idx will be -1 when printing a menu title or a valid index when printing as option
                   menu::printTo(root, sel, out, idx, len, p)
                       :                                                    // when printing title
                   out.printRaw((constText *)F("Reboot w/o saving"), len);  // when printing as regular option
    }
};

result systemReboot() {
    Serial.println();
    Serial.println("-->[MENU] Reboot CO2 Gadget at user request from menu...");
    // do some termination stuff here
    if (sensorsGetMainDeviceSelected().equals("SCD30")) {
        Serial.println("-->[MENU] Resetting SCD30 sensor...");
        sensors.scd30.reset();
        delay(100);
    }
    ESP.restart();
    return quit;
}

// using the customized menu class
// note that first parameter is the class name
altMENU(confirmReboot, rebootMenu, "Reboot?", doNothing, noEvent, wrapStyle, (Menu::_menuData | Menu::_canNav), OP("Yes", systemReboot, enterEvent), EXIT("Cancel"));

char tempIPAddress[16];

// list of allowed characters
const char *const hexChars[] MEMMODE = {"0123456789ABCDEF"};
const char *const alphaNum[] MEMMODE = {" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
const char *const allChars[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!#@$%&/()=+-*^~:.[]{}?¿"};
const char *const ssidChars[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!#@%&/()=-*^~:.{}¿"};
const char *const reducedSet[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyz.-_"};

// field will initialize its size by this string length
char tempMQTTTopic[] = "                              ";
char tempMQTTClientId[] = "                              ";
char tempMQTTBrokerIP[] = "                              ";
char tempMQTTUser[] = "                              ";
char tempMQTTPass[] = "                              ";
char tempWiFiSSID[] = "                              ";
char tempWiFiPasswrd[] = "                              ";
char tempHostName[] = "                              ";
char tempBLEDeviceId[] = "                              ";
char tempCO2Sensor[] = "                              ";
char tempESPNowAddress[] = "            ";

void setInMenu(bool isInMenu) {
    inMenu = isInMenu;
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] inMenu:\t %s\n", ((inMenu) ? "TRUE" : "FALSE"));
#endif
}

void fillTempIPAddress() {
    if ((activeWIFI) && (WiFi.isConnected())) {
        sprintf(tempIPAddress, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    } else {
        sprintf(tempIPAddress, ((activeWIFI) ? "Disconnected" : "Disabled"));
    }
}

void showPath(navRoot &root) {
    Serial.print("-->[MENU] nav level:");
    Serial.print(root.level);
    Serial.print(" path:[");
    for (int n = 0; n <= root.level; n++) {
        Serial.print(n ? "," : "");
        Serial.print(root.path[n].sel);
    }
    Serial.println("]");
}

result showEvent(eventMask e, navNode &nav, prompt &item) {
    Serial.println();
    Serial.println("========");
    Serial.print("-->[MENU] Event for target: 0x");
    Serial.println((long)nav.target, HEX);
    showPath(*nav.root);
    Serial.print("-->[MENU] ");
    Serial.print(e);
    switch (e) {
        case noEvent:  // just ignore all stuff
            Serial.println(" noEvent");
            break;
        case activateEvent:  // this item is about to be active (system event)
            Serial.println(" activateEvent");
            break;
        case enterEvent:  // entering navigation level (this menu is now active)
            Serial.println(" enterEvent");
            break;
        case exitEvent:  // leaving navigation level
            Serial.println(" exitEvent");
            break;
        case returnEvent:  // entering previous level (return)
            Serial.println(" returnEvent");
            break;
        case focusEvent:  // element just gained focus
            Serial.println(" focusEvent");
            break;
        case blurEvent:  // element about to lose focus
            Serial.println(" blurEvent");
            break;
        case selFocusEvent:  // child just gained focus
            Serial.println(" selFocusEvent");
            break;
        case selBlurEvent:  // child about to lose focus
            Serial.println(" selBlurEvent");
            break;
        case updateEvent:  // Field value has been updated
            Serial.println(" updateEvent");
            break;
        case anyEvent:
            Serial.println(" anyEvent");
            break;
    }
    return proceed;
}

result doCalibration400ppm(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Calibrating sensor at %d", 400);
    Serial.print(F("-->[MENU] action1 event:"));
    Serial.println(e);
    Serial.flush();
#endif
    calibrationValue = 400;
    pendingCalibration = true;
    return quit;
}

result doCalibrationCustom(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Calibrating sensor at %d\n", customCalibrationValue);
    Serial.print(F("-->[MENU] action1 event:"));
    Serial.println(e);
    Serial.flush();
#endif
    calibrationValue = customCalibrationValue;
    pendingCalibration = true;
    return quit;
}

result doSavePreferences(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("-->[MENU] Saving preferences to NVR");
    Serial.print(F("-->[MENU] action1 event:"));
    Serial.println(e);
    Serial.flush();
#endif
    putPreferences();
    return quit;
}

result doSetDisplayBrightness(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Setting TFT brightness at %d", DisplayBrightness);
    Serial.print(F("-->[MENU] action1 event:"));
    Serial.println(e);
    Serial.flush();
#endif
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
    setDisplayBrightness(DisplayBrightness);
#endif
    return proceed;
}

// clang-format off

TOGGLE(autoSelfCalibration, autoSelfCalibrationMenu, "Autom. Cal.: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doNothing, noEvent)
  ,VALUE("OFF", false, doNothing, noEvent));

MENU(calibrationMenu, "Calibration", doNothing, noEvent, wrapStyle
  ,SUBMENU(autoSelfCalibrationMenu)
  ,OP("Calibrate at 400ppm", doCalibration400ppm, enterEvent)
  ,FIELD(customCalibrationValue, "Custom Cal: ", "ppm", 400, 2000, 10, 10, showEvent, enterEvent, noStyle)
  ,OP("Calibrate at custom ppm", doCalibrationCustom, enterEvent)
  ,EXIT("<Back"));
  // ,OP("Test menu event", showEvent, anyEvent),

int8_t setCO2Sensor;
const uint8_t AutoSensor = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6;

void SetTempCO2Sensor(int8_t sensor) {
  String strSensor="", paddedString="";  
  
  if (sensor==AutoSensor)            {strSensor = "AutoSensor";}
  else if (sensor==MHZ19)      {strSensor = "MHZ19";}
  else if (sensor==CM1106)     {strSensor = "CM1106";}
  else if (sensor==SENSEAIRS8) {strSensor = "SENSEAIRS8";}
  else {strSensor = "Unknown";}
  paddedString = rightPad(strSensor, 30);  
  paddedString.toCharArray(tempMQTTTopic, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting selected CO2 sensor to: #%s#\n", paddedString.c_str());
  #endif
}

result doSetCO2Sensor(eventMask e, navNode &nav, prompt &item) {
  if (selectedCO2Sensor != setCO2Sensor) {
    Serial.printf("-->[MENU] New CO2 Sensor selected: %d\n", setCO2Sensor);
    selectedCO2Sensor = setCO2Sensor;
    SetTempCO2Sensor(selectedCO2Sensor);
    displayNotification("Init sensor", notifyInfo);
    initSensors();
  }  else {
    Serial.printf("-->[MENU] CO2 Sensor same as before (old:%d/selected:%d)\n", selectedCO2Sensor, setCO2Sensor);
  }
  return proceed;
}

CHOOSE(setCO2Sensor,CO2SensorChooseMenu,"Sensor ",doNothing,noEvent,wrapStyle
  ,VALUE("AutoSensor (I2C)",AutoSensor,doSetCO2Sensor, enterEvent)
  ,VALUE("MH-Z19 (A/B/C/D)",MHZ19,doSetCO2Sensor, enterEvent)
  ,VALUE("CM1106",CM1106,doSetCO2Sensor, enterEvent)
  ,VALUE("Senseair S8",SENSEAIRS8,doSetCO2Sensor, enterEvent)
);

result doSetDebugSensors(eventMask e, navNode &nav, prompt &item) {
  sensors.setDebugMode(debugSensors);
  return proceed;
}

TOGGLE(debugSensors, debugSensorsMenu, "Debug Sensors: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetDebugSensors, enterEvent)
  ,VALUE("OFF", false, doSetDebugSensors, enterEvent));

MENU(CO2SensorConfigMenu, "CO2 Sensor", doNothing, noEvent, wrapStyle
  ,SUBMENU(CO2SensorChooseMenu)
  ,SUBMENU(autoSelfCalibrationMenu)
  ,FIELD(ambientPressureValue, "Pres. Comp.", "mbar", 0, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(altitudeMeters, "Altitude", "mtrs", 0, 9999, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2OrangeRange, "Orange", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2RedRange, "Red", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,SUBMENU(debugSensorsMenu)
  ,EXIT("<Back"));

#ifdef SUPPORT_BLE
result doSetActiveBLE(eventMask e, navNode &nav, prompt &item) {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("activeBLE", activeBLE);
  preferences.end();
  return proceed;
}

TOGGLE(activeBLE, activeBLEMenu, "BLE Enable: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveBLE, exitEvent)
  ,VALUE("OFF", false, doSetActiveBLE, exitEvent));

MENU(bleConfigMenu, "BLE Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeBLEMenu)
  ,OP("You can't", doNothing, noEvent)
  ,OP("disable BLE.", doNothing, noEvent)
  ,EXIT("<Back"));
#endif

result doSetActiveWIFI(eventMask e, navNode &nav, prompt &item) {
  if (!activeWIFI) {
    activeMQTT = false;
    disableWiFi();
    #ifdef SUPPORT_ESPNOW
    if (activeESPNOW) {
      initESPNow();
    }
    #endif
  } else {
    initWifi();
    nav.target-> dirty = true;
    activeMQTT = preferences.getBool("activeMQTT", false); // TO-DO: Check if this is needed. It do not looks fine.
    if ((activeMQTT) && (WiFi.isConnected())) {
      initMQTT();
    }
  }
  fillTempIPAddress();
  return proceed;
}

result doSetWiFiSSID(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting WiFi SSID to #%s#\n", tempWiFiSSID);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  wifiSSID = String(tempWiFiSSID);
  wifiSSID.trim();
  return proceed;
}

result doSetWiFiPasswrd(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting WiFi Password to #%s#\n", tempWiFiPasswrd);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  wifiPass = String(tempWiFiPasswrd);
  wifiPass.trim();
  return proceed;
}

result doSetHostName(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting HostName to #%s#\n", tempHostName);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  hostName = String(tempHostName);
  hostName.trim();
  return proceed;
}

TOGGLE(activeWIFI, activeWIFIMenu, "WIFI Enable: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveWIFI, exitEvent)
  ,VALUE("OFF", false, doSetActiveWIFI, exitEvent));

#ifdef SUPPORT_OTA
result doSetActiveOTA(eventMask e, navNode &nav, prompt &item) {
  return proceed;
}

TOGGLE(activeOTA, activeOTAMenu, "OTA Enable: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveOTA, exitEvent)
  ,VALUE("OFF", false, doSetActiveOTA, exitEvent));
#endif

MENU(wifiConfigMenu, "WIFI Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeWIFIMenu)
  ,EDIT("SSID", tempWiFiSSID, ssidChars, doSetWiFiSSID, exitEvent, wrapStyle)
  ,EDIT("Pass:", tempWiFiPasswrd, allChars, doSetWiFiPasswrd, exitEvent, wrapStyle)
  ,EDIT("Host:", tempHostName, allChars, doSetHostName, exitEvent, wrapStyle)
#ifdef SUPPORT_OTA
  ,SUBMENU(activeOTAMenu)
#endif
  ,EXIT("<Back"));


result doSetMQTTTopic(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting MQTT Topic to #%s#\n", tempMQTTTopic);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempMQTTTopic, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  rootTopic = tempMQTTTopic;
  if ((activeMQTT) && (WiFi.isConnected())) {
    initMQTT();
  }
  return proceed;
}

result doSetMQTTClientId(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting MQTT Client Id to #%s#\n", tempMQTTClientId);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempMQTTClientId, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  mqttClientId = tempMQTTClientId;  
  if ((activeMQTT) && (WiFi.isConnected())) {
    initMQTT();
  }
  return proceed;
}

result doSetMQTTBrokerIP(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting MQTT Broker IP to: #%s#\n", tempMQTTBrokerIP);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempMQTTBrokerIP, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  mqttBroker = tempMQTTBrokerIP;  
  if ((activeMQTT) && (WiFi.isConnected())) {
    initMQTT();
  }
  return proceed;
}

result doSetMQTTUser(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting MQTT User to: #%s#\n", tempMQTTBrokerIP);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempMQTTUser, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  mqttUser = tempMQTTUser;  
  if ((activeMQTT) && (WiFi.isConnected())) {
    initMQTT();
  }
  return proceed;
}

result doSetMQTTPass(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting MQTT Pass to: #%s#\n", tempMQTTPass);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempMQTTPass, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  mqttPass = tempMQTTPass;  
  if ((activeMQTT) && (WiFi.isConnected())) {
    initMQTT();
  }
  return proceed;
}

result doSetActiveMQTT(eventMask e, navNode &nav, prompt &item) {
  if ((activeWIFI) && (activeMQTT)) {
    initMQTT();
  } else {
    activeMQTT = false;
  }
  return proceed;
}

TOGGLE(activeMQTT, activeMQTTMenu, "MQTT Enable: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveMQTT, exitEvent)
  ,VALUE("OFF", false, doSetActiveMQTT, exitEvent));

MENU(mqttConfigMenu, "MQTT Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeMQTTMenu)
  ,EDIT("Topic", tempMQTTTopic, alphaNum, doSetMQTTTopic, exitEvent, wrapStyle)
  ,EDIT("Id", tempMQTTClientId, alphaNum, doSetMQTTClientId, exitEvent, wrapStyle)
  ,EDIT("Broker IP", tempMQTTBrokerIP, reducedSet, doSetMQTTBrokerIP, exitEvent, wrapStyle)
  ,EDIT("User", tempMQTTUser, alphaNum, doSetMQTTUser, exitEvent, wrapStyle)
  ,EDIT("Pass", tempMQTTPass, alphaNum, doSetMQTTPass, exitEvent, wrapStyle)
  ,EXIT("<Back"));

#ifdef SUPPORT_ESPNOW
result doSetActiveESPNOW(eventMask e, navNode &nav, prompt &item) {
  if (!activeESPNOW) {
    disableESPNow();
  } else {
    initESPNow();
  }
  return proceed;
}

TOGGLE(activeESPNOW, activeESPNOWMenu, "ESP-NOW Enable: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveESPNOW, exitEvent)
  ,VALUE("OFF", false, doSetActiveESPNOW, exitEvent));


byte nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;  // Not a valid hexadecimal character
}

void hexCharacterStringToBytes(byte *byteArray, const char *hexString) // https://forum.arduino.cc/t/hex-string-to-byte-array/563827/4
{
  bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    bool oddCharIndex = charIndex & 1;

    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}

result doSetPeerESPNow(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting ESP-NOW Peer to: #%s#\n", tempESPNowAddress);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.printf("-->[MENU] peerESPNow: #%02X:%02X:%02X:%02X:%02X:%02X#\n", peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2], peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
  Serial.flush();
#endif
  esp_now_del_peer(peerESPNowAddress);
  hexCharacterStringToBytes(peerESPNowAddress, tempESPNowAddress);
  memcpy(peerInfo.peer_addr, peerESPNowAddress, 6);
  esp_now_add_peer(&peerInfo);
  return proceed;
}

MENU(espnowConfigMenu, "ESP-NOW Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeESPNOWMenu)
  ,FIELD(timeBetweenESPNowPublish, "TX Time: ", " Secs", 10, 360, 10, 100, doNothing, noEvent, noStyle)
  ,FIELD(boardIdESPNow, "Board ID: ", "", 0, 254, 1, 10, doNothing, noEvent, noStyle)
  ,EDIT("Peer MAC: ", tempESPNowAddress, hexChars, doSetPeerESPNow,  exitEvent, wrapStyle)
  ,EXIT("<Back"));
#endif // SUPPORT_ESPNOW

result doSetvRef(eventMask e, navNode &nav, prompt &item) {
  battery.begin(vRef, voltageDividerRatio, &asigmoidal);
  delay(10);
  batteryVoltage = (float)battery.voltage() / 1000;
  nav.target-> dirty = true;
  return proceed;
}

MENU(batteryConfigMenu, "Battery Config", doNothing, noEvent, wrapStyle
  ,FIELD(batteryVoltage, "Battery:", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,FIELD(vRef, "Voltage ref:", "", 0, 2000, 10, 10, doSetvRef, anyEvent, noStyle)
  ,FIELD(batteryFullyChargedMillivolts, "Bat Full (mV):", "", 0, 4200, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(batteryDischargedMillivolts, "Bat Empty (mV):", "", 2700, 3700, 10, 10, doNothing, noEvent, noStyle)
  ,EXIT("<Back"));

result doSetTempOffset(eventMask e, navNode &nav, prompt &item) {
  #ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Setting setTempOffset to %.2f\n",tempOffset);
  #endif
  sensors.setTempOffset(tempOffset);
  preferences.begin("CO2-Gadget", false);
  preferences.putFloat("tempOffset", tempOffset);
  preferences.end();
  nav.target-> dirty = true;
  return proceed;
}

TOGGLE(showFahrenheit, showFahrenheitMenu, "Units: ", doNothing,noEvent, wrapStyle
  ,VALUE("Celsius ", false, doNothing, noEvent)
  ,VALUE("Fahrenheit ", true, doNothing, noEvent));

MENU(temperatureConfigMenu, "Temp Config", doNothing, noEvent, wrapStyle
  ,FIELD(temp, "Temp", " deg C", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,altFIELD(decPlaces<1>::menuField,tempOffset,"Offset"," deg C",0,30,1,0.1,doSetTempOffset,(eventMask)(enterEvent | exitEvent | updateEvent),wrapStyle)
  ,SUBMENU(showFahrenheitMenu)
  ,EXIT("<Back"));

TOGGLE(displayOffOnExternalPower, activeDisplayOffMenuOnBattery, "Off on USB: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doNothing, noEvent)
  ,VALUE("OFF", false, doNothing, noEvent));

  
result doDisplayReverse(eventMask e, navNode &nav, prompt &item) {
  #ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Setting doDisplayReverse to %s\n", ((displayReverse) ? "TRUE" : "FALSE"));
  #endif  
  reverseButtons(displayReverse);  
  #ifdef SUPPORT_TFT
  if (displayReverse) {
    tft.setRotation(3);
  } else {
    tft.setRotation(1);
  }
  #endif
  #ifdef SUPPORT_OLED
  if (displayReverse) {
    u8g2.setDisplayRotation(U8G2_R2);
  } else {
    u8g2.setDisplayRotation(U8G2_R0);
  }
  #endif
  nav.target-> dirty = true;
  return proceed;
}

TOGGLE(displayReverse, activeDisplayReverse, "Orient: ", doNothing, noEvent, wrapStyle
  ,VALUE("Normal", false, doDisplayReverse, enterEvent)
  ,VALUE("Reversed", true, doDisplayReverse, enterEvent));


TOGGLE(displayShowTemperature, activeDisplayShowTemperature, "Temp: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doDisplayReverse, enterEvent)
  ,VALUE("Show", true, doDisplayReverse, enterEvent));

TOGGLE(displayShowHumidity, activeDisplayShowHumidity, "Humidity: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doDisplayReverse, enterEvent)
  ,VALUE("Show", true, doDisplayReverse, enterEvent));

TOGGLE(displayShowBattery, activeDisplayShowBattery, "Battery: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doDisplayReverse, enterEvent)
  ,VALUE("Show", true, doDisplayReverse, enterEvent));

TOGGLE(displayShowCO2, activeDisplayShowCO2, "CO2: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doDisplayReverse, enterEvent)
  ,VALUE("Show", true, doDisplayReverse, enterEvent));

TOGGLE(displayShowPM25, activeDisplayShowPM25, "PM2.5: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doDisplayReverse, enterEvent)
  ,VALUE("Show", true, doDisplayReverse, enterEvent));

MENU(displayConfigMenu, "Display Config", doNothing, noEvent, wrapStyle
#ifdef ARDUINO_LILYGO_T_DISPLAY_S3
  ,FIELD(DisplayBrightness, "Brightness:", "", 1, 16, 1, 1, doSetDisplayBrightness, anyEvent, wrapStyle)
#endif  
#if defined(TTGO_TDISPLAY) || defined(ST7789_240x320)
  ,FIELD(DisplayBrightness, "Brightness:", "", 10, 255, 10, 10, doSetDisplayBrightness, anyEvent, wrapStyle)
#endif  
  ,FIELD(timeToDisplayOff, "Time To Off:", "", 0, 900, 5, 5, doNothing, noEvent, wrapStyle)
  ,SUBMENU(activeDisplayOffMenuOnBattery)
  ,SUBMENU(activeDisplayReverse)
  ,SUBMENU(activeDisplayShowTemperature)
  ,SUBMENU(activeDisplayShowHumidity)
  ,SUBMENU(activeDisplayShowBattery)
  // ,SUBMENU(activeDisplayShowCO2)
  // ,SUBMENU(activeDisplayShowPM25)
  ,EXIT("<Back"));

result doSetActiveNeopixelType(eventMask e, navNode &nav, prompt &item) {
  #ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Setting selectedNeopixelType to %d\n",selectedNeopixelType);
  #endif
  setNeopixelType(selectedNeopixelType);
  strip.show();
  return proceed;
}

TOGGLE(selectedNeopixelType, activeNeopixelTypeMenu, "Neopixels: ", doNothing,noEvent, wrapStyle
  ,VALUE("NEO_GRB",  NEO_GRB  + NEO_KHZ800, doSetActiveNeopixelType, anyEvent)
  ,VALUE("NEO_RGB",  NEO_RGB  + NEO_KHZ800, doSetActiveNeopixelType, anyEvent)
  ,VALUE("NEO_RGBW", NEO_RGBW + NEO_KHZ800, doSetActiveNeopixelType, anyEvent));

  // Can add these really old Neopixel types if needed
  // ,VALUE("NEO_GRB v1",  NEO_GRB  + NEO_KHZ400, doSetActiveNeopixelType, anyEvent)
  // ,VALUE("NEO_RGB v1",  NEO_RGB  + NEO_KHZ400, doSetActiveNeopixelType, anyEvent)
  // ,VALUE("NEO_RGBW v1", NEO_RGBW + NEO_KHZ400, doSetActiveNeopixelType, anyEvent)

result doSetNeopixelBrightness(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting TFT brightness at %d", neopixelBrightness);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  setNeopixelBrightness(neopixelBrightness);
  strip.show();
  return proceed;
}

result doSetOuputsRelayMode(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("-->[MENU] Setting outputsModeRelay to %d", outputsModeRelay);
  Serial.print(F("-->[MENU] action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  outputsLoop();
  return proceed;
}

TOGGLE(outputsModeRelay, outputsModeMenu, "GPIO Outs: ", doNothing,noEvent, wrapStyle
  ,VALUE("RGB LED", false, doSetOuputsRelayMode, anyEvent)
  ,VALUE("Relays", true, doSetOuputsRelayMode, anyEvent));


  #ifdef SUPPORT_BUZZER
TOGGLE(timeBetweenBuzzerBeeps, timeBetweenBuzzerBeepMenu, "Buzzer: ", doNothing, noEvent, wrapStyle
  ,VALUE("OFF", -1,  doNothing, noEvent)
  ,VALUE("One time", 0,  doNothing, noEvent)
  ,VALUE("Every 5s", 5,  doNothing, noEvent)
  ,VALUE("Every 10s", 10,  doNothing, noEvent)
  ,VALUE("Every 15s", 15,  doNothing, noEvent)
  ,VALUE("Every 30s", 30,  doNothing, noEvent)
  ,VALUE("Every 1min", 60,  doNothing, noEvent)
  ,VALUE("Every 2min", 120,  doNothing, noEvent)
  ,VALUE("Every 5min", 300,  doNothing, noEvent));
  
TOGGLE(toneBuzzerBeep, toneBuzzerBeepMenu, "Tone: ", doNothing, noEvent, wrapStyle
  ,VALUE("HIGH", BUZZER_TONE_HIGH,  doNothing, noEvent)
  ,VALUE("MED", BUZZER_TONE_MED,  doNothing, noEvent)
  ,VALUE("LOW", BUZZER_TONE_LOW,  doNothing, noEvent));

TOGGLE(durationBuzzerBeep, durationBuzzerBeepMenu, "Span: ", doNothing, noEvent, wrapStyle
  ,VALUE("SHORT", DURATION_BEEP_SHORT,  doNothing, noEvent)
  ,VALUE("MED", DURATION_BEEP_MEDIUM,  doNothing, noEvent)
  ,VALUE("LONG", DURATION_BEEP_LONG,  doNothing, noEvent));

MENU(buzzerConfigMenu, "Buzzer Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(timeBetweenBuzzerBeepMenu)
  ,SUBMENU(toneBuzzerBeepMenu)
  ,SUBMENU(durationBuzzerBeepMenu)
  ,EXIT("<Back"));
#endif

MENU(outputsConfigMenu, "Outputs Config", doNothing, noEvent, wrapStyle
  #ifdef SUPPORT_BUZZER
  ,SUBMENU(buzzerConfigMenu)
  #endif  
  ,FIELD(neopixelBrightness, "Neopix Bright", "%", 0, 255, 5, 10, doSetNeopixelBrightness, anyEvent, noStyle)
  ,SUBMENU(activeNeopixelTypeMenu)
  ,SUBMENU(outputsModeMenu)
  ,EXIT("<Back"));

MENU(configMenu, "Configuration", doNothing, noEvent, wrapStyle
  ,SUBMENU(CO2SensorConfigMenu)
  #ifdef SUPPORT_BLE
  ,SUBMENU(bleConfigMenu)
  #endif
  ,SUBMENU(wifiConfigMenu)
  ,SUBMENU(mqttConfigMenu)
#ifdef SUPPORT_ESPNOW
  ,SUBMENU(espnowConfigMenu)
#endif  
  ,SUBMENU(batteryConfigMenu)
  ,SUBMENU(temperatureConfigMenu)
  ,SUBMENU(displayConfigMenu)
  ,SUBMENU(outputsConfigMenu)
  ,OP("Save preferences", doSavePreferences, enterEvent)
  ,EXIT("<Back"));

std::string getUptime() {
  unsigned long uptime = millis() / 1000; // Get the uptime in seconds

  unsigned long days = uptime / (24 * 60 * 60);
  unsigned long hours = (uptime % (24 * 60 * 60)) / (60 * 60);
  unsigned long minutes = (uptime % (60 * 60)) / 60;
  unsigned long seconds = uptime % 60;

  return "Uptime: " + std::to_string(days) + "D, " + std::to_string(hours) + "H, " +
         std::to_string(minutes) + "M, " + std::to_string(seconds) + "S";
}


// Customizing a prompt look by extending the prompt class
class altPromptUptime:public prompt {
public:
  altPromptUptime(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    return out.printRaw(getUptime().c_str(),len);
  }
};


MENU(informationMenu, "Information", doNothing, noEvent, wrapStyle
  ,FIELD(batteryVoltage, "Battery", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,OP("Comp " BUILD_GIT, doNothing, noEvent)
  ,OP("Version " CO2_GADGET_VERSION CO2_GADGET_REV, doNothing, noEvent)
  ,OP("" FLAVOUR, doNothing, noEvent)
  ,altOP(altPromptUptime, "", doNothing, noEvent)
  ,EDIT("IP", tempIPAddress, alphaNum, doNothing, noEvent, wrapStyle)
  ,EDIT("BLE Dev. Id", tempBLEDeviceId, alphaNum, doNothing, noEvent, wrapStyle)  
  ,EXIT("<Back"));

// when entering main menu
result enterMainMenu(menuOut &o, idleEvent e) {
#ifdef DEBUG_ARDUINOMENU
  Serial.println("-->[MENU] Enter main menu");
#endif
  return proceed;
}

// ,FIELD(batteryVoltage, "Battery", "Volts", 0, 9, 0, 0, doNothing, noEvent, noStyle) // It was removed from menu as updates avoids timeout to work
MENU(mainMenu, "CO2 Gadget", doNothing, noEvent, wrapStyle
  ,SUBMENU(informationMenu)
  ,SUBMENU(configMenu)
  ,SUBMENU(calibrationMenu)
  ,SUBMENU(rebootMenu)
  ,EXIT("<Exit"));

#define MAX_DEPTH 4

// define serial input device
serialIn serial(Serial);

// define serial output device
idx_t serialTops[MAX_DEPTH] = {0};
serialOut outSerial(Serial, serialTops);

#ifdef SUPPORT_TFT
// define menu colors --------------------------------------------------------
#define Black RGB565(0, 0, 0)
#define Red RGB565(255, 0, 0)
#define Green RGB565(0, 255, 0)
#define Blue RGB565(0, 0, 255)
#define Gray RGB565(128, 128, 128)
#define LighterRed RGB565(255, 150, 150)
#define LighterGreen RGB565(150, 255, 150)
#define LighterBlue RGB565(150, 150, 255)
#define LighterGray RGB565(211, 211, 211)
#define DarkerRed RGB565(150, 0, 0)
#define DarkerGreen RGB565(0, 150, 0)
#define DarkerBlue RGB565(0, 0, 150)
#define Cyan RGB565(0, 255, 255)
#define Magenta RGB565(255, 0, 255)
#define Yellow RGB565(255, 255, 0)
#define White RGB565(255, 255, 255)
#define DarkerOrange RGB565(255, 140, 0)

const colorDef<uint16_t> colors[6] MEMMODE = {
    //{{disabled normal,disabled selected}, {enabled normal,  enabled selected,       enabled editing}}
    {{(uint16_t)Black,  (uint16_t)Black},  {(uint16_t)Black,  (uint16_t)Blue,         (uint16_t)Blue}},   // bgColor
    {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,        (uint16_t)White}},  // fgColor
    {{(uint16_t)Green,  (uint16_t)Green},  {(uint16_t)Green,  (uint16_t)Green,        (uint16_t)Green}},  // valColor - Numbers
    {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,        (uint16_t)White}},  // unitColor - Numeric field unit color
    {{(uint16_t)White,  (uint16_t)Gray},   {(uint16_t)Black,  (uint16_t)Red,          (uint16_t)White}},  // cursorColor
    {{(uint16_t)White,  (uint16_t)Yellow}, {(uint16_t)Black,  (uint16_t)DarkerOrange, (uint16_t)Red}},    // titleColor - Menu title color
};

#define tft_WIDTH 240
#define tft_HEIGHT 135
#define fontW 10
#define fontH 20

#if defined(TFT_WIDTH) && defined(TFT_HEIGHT)
#if TFT_WIDTH == 170 && TFT_HEIGHT == 320  // Display is rotated 90 degrees
#undef tft_WIDTH
#undef tft_HEIGHT
#define tft_WIDTH 320
#define tft_HEIGHT 170
#endif
#if TFT_WIDTH == 240 && TFT_HEIGHT == 320  // Display is rotated 90 degrees
#undef tft_WIDTH
#undef tft_HEIGHT
#define tft_WIDTH 320
#define tft_HEIGHT 240
#endif
#endif

const panel panels[] MEMMODE = {{0, 0, tft_WIDTH / fontW, tft_HEIGHT / fontH}};
navNode *nodes[sizeof(panels) /
               sizeof(panel)];      // navNodes to store navigation status
panelsList pList(panels, nodes, 1); // a list of panels and nodes
idx_t eSpiTops[MAX_DEPTH] = {0};
TFT_eSPIOut eSpiOut(tft, colors, eSpiTops, pList, fontW, fontH + 1);
menuOut *constMEM outputs[] MEMMODE = {&outSerial, &eSpiOut}; // list of output devices
menuOut *constMEM outputsNoSerial[] MEMMODE = {&eSpiOut}; // list of output devices
#endif // SUPPORT_TFT

#ifdef SUPPORT_OLED
// define menu colors --------------------------------------------------------
//each color is in the format:
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
// this is a monochromatic color table
const colorDef<uint8_t> colors[6] MEMMODE={
  {{0,0},{0,1,1}},//bgColor
  {{1,1},{1,0,0}},//fgColor
  {{1,1},{1,0,0}},//valColor
  {{1,1},{1,0,0}},//unitColor
  {{0,1},{0,0,1}},//cursorColor
  {{1,1},{1,0,0}},//titleColor
};

#define fontX 5
#define fontY 10
// #define MENUFONT u8g2_font_7x13_mf
// #define fontX 7
// #define fontY 16
#define offsetX 1
#define offsetY 2
#define U8_Width 128
#define U8_Height 64
#define USE_HWI2C
#define fontMarginX 2
#define fontMarginY 2

//define output device oled
idx_t gfx_tops[MAX_DEPTH];
PANELS(gfxPanels,{0,0,U8_Width/fontX,U8_Height/fontY});
u8g2Out oledOut(u8g2,colors,gfx_tops,gfxPanels,fontX,fontY,offsetX,offsetY,fontMarginX,fontMarginY);

//define outputs controller
menuOut* outputs[]{&outSerial,&oledOut};//list of output devices

MENU_INPUTS(in,&serial);
#endif // SUPPORT_OLED

#ifdef SUPPORT_EINK
// define menu colors --------------------------------------------------------
const colorDef<uint8_t> colors[6] MEMMODE = {
    // {{disabled normal, disabled selected}, {enabled normal, enabled selected, enabled editing}}
    {{0, 1}, {1, 0, 0}}, // bgColor
    {{1, 0}, {0, 1, 1}}, // fgColor
    {{1, 0}, {0, 1, 1}}, // valColor
    {{1, 0}, {0, 1, 1}}, // unitColor
    {{0, 1}, {0, 0, 0}}, // cursorColor
    {{1, 0}, {1, 0, 0}}, // titleColor
};

#define fontW 10
#define fontH 20  
#define offsetX 1
#define offsetY -2
#define fontMarginX 2
#define fontMarginY 2

//define output device oled
idx_t gfx_tops[MAX_DEPTH];
PANELS(gfxPanels,{0, 0, displayWidth/fontW,displayHeight/fontH});
// Crear la instancia de salida para GxEPD2
Menu::GxEPD2Out<GxEPD2_DRIVER> displayOut(display, colors, gfx_tops, gfxPanels, fontW, fontH, offsetX, offsetY, fontMarginX, fontMarginY);

//define outputs controller
menuOut* outputs[]{&outSerial,&displayOut};//list of output devices
#endif // SUPPORT_EINK

#if (!SUPPORT_OLED && !SUPPORT_TFT && !SUPPORT_EINK)
menuOut* outputs[]{&outSerial};//No display, only serial output
#endif

outputsList out(outputs, sizeof(outputs) / sizeof(menuOut *)); // outputs list controller

// clang-format on

NAVROOT(nav, mainMenu, MAX_DEPTH, serial, out);
void copyStringToCharArray(const String &source, char *destination, size_t size, const char *label) {
#ifdef DEBUG_ARDUINOMENU
    Serial.print("-->[MENU] ");
    Serial.print(label);
    Serial.print(": #");
    Serial.print(source);
    Serial.println("#");
#endif
    source.toCharArray(destination, size);
}

void loadTempArraysWithActualValues() {
    copyStringToCharArray(rightPad(rootTopic, 30), tempMQTTTopic, 30, "tempMQTTTopic");
    copyStringToCharArray(rightPad(mqttClientId, 30), tempMQTTClientId, 30, "tempMQTTClientId");
    copyStringToCharArray(rightPad(mqttBroker, 30), tempMQTTBrokerIP, 30, "tempMQTTBrokerIP");
    copyStringToCharArray(rightPad(mqttUser, 30), tempMQTTUser, 30, "tempMQTTUser");

#ifdef WIFI_PRIVACY
    copyStringToCharArray(rightPad(" ", 30), tempMQTTPass, 30, "tempMQTTPass");
#else
    copyStringToCharArray(rightPad(mqttPass, 30), tempMQTTPass, 30, "tempMQTTPass");
#endif

    copyStringToCharArray(rightPad(wifiSSID, 30), tempWiFiSSID, 30, "tempWiFiSSID");

#ifdef WIFI_PRIVACY
    copyStringToCharArray(rightPad(" ", 30), tempWiFiPasswrd, 30, "tempWiFiPasswrd");
#else
    copyStringToCharArray(rightPad(wifiPass, 30), tempWiFiPasswrd, 30, "tempWiFiPasswrd");
#endif

    copyStringToCharArray(rightPad(hostName, 30), tempHostName, 30, "tempHostName");

#ifdef SUPPORT_BLE
    copyStringToCharArray(rightPad(provider.getDeviceIdString(), 30), tempBLEDeviceId, 30, "tempBLEDeviceId");
#else
    copyStringToCharArray(rightPad("Unavailable", 30), tempBLEDeviceId, 30, "tempBLEDeviceId");
#endif

    String co2SensorString = (sensorsGetMainDeviceSelected() == "SCD30" || sensorsGetMainDeviceSelected() == "SCD4x")
                                 ? rightPad("AutoSensor (I2C)", 30)
                                 : rightPad(sensorsGetMainDeviceSelected(), 30);
    copyStringToCharArray(co2SensorString, tempCO2Sensor, 30, "tempCO2Sensor");

    snprintf(tempESPNowAddress, sizeof(tempESPNowAddress), "%02X%02X%02X%02X%02X%02X",
             peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
             peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);

#if (DEBUG_ARDUINOMENU && SUPPORT_ESPNOW)
    Serial.printf("-->[MENU] peerESPNow: #%02X:%02X:%02X:%02X:%02X:%02X#\n",
                  peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
                  peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
    Serial.print("-->[MENU] tempESPNowAddress: #");
    Serial.print(tempESPNowAddress);
    Serial.println("#");
#endif

    fillTempIPAddress();
}

// when menu is suspended
result idle(menuOut &o, idleEvent e) {
    switch (e) {
        case idleStart:
#ifdef DEBUG_ARDUINOMENU
            Serial.println("-->[MENU] Event idleStart");
#endif
            setInMenu(false);
            //       nav.poll();

#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
            displayShowValues(true);
#endif
            break;
        case idling:
#ifdef DEBUG_ARDUINOMENU
            Serial.println("-->[MENU] Event iddling");
#endif
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
            displayShowValues(shouldRedrawDisplay);
            shouldRedrawDisplay = false;
#endif
            break;
        case idleEnd:
#ifdef DEBUG_ARDUINOMENU
            Serial.println("-->[MENU] Event idleEnd");
#endif
            setInMenu(true);
            //   displayNotification("Enter menu", notifyInfo);
            //   delay(1000);
            loadTempArraysWithActualValues();
            break;
        default:
#ifdef DEBUG_ARDUINOMENU
            Serial.print("-->[MENU] Unhandled event: ");
            Serial.println(e);
#endif
            break;
    }
    // displayNotification("Idle end", notifyInfo);
    // delay(1000);
    return proceed;
}

void menuLoopTFT() {
#ifdef SUPPORT_TFT
    if ((wifiChanged) && (!inMenu)) {
        wifiChanged = false;
        displayNotification("To clear display", notifyInfo);
        delay(1000);
        tft.fillScreen(TFT_BLACK);
        displayNotification("Display cleared", notifyInfo);
        delay(1000);
    }

    nav.doInput();
    if (nav.sleepTask) {
        displayShowValues(shouldRedrawDisplay);
        shouldRedrawDisplay = false;
    } else {
        if (nav.changed(0)) {
            nav.doOutput();
        }
    }
#endif
}

void menuLoopOLED() {
#ifdef SUPPORT_OLED
    if (nav.sleepTask) {
        displayShowValues(shouldRedrawDisplay);
        shouldRedrawDisplay = false;
    } else {
        if (nav.changed(0)) {
            u8g2.firstPage();
            do nav.doOutput();
            while (u8g2.nextPage());
        }
    }
#endif
}

void menuLoopEINK() {
#ifdef SUPPORT_EINK
    nav.doInput();
    if (nav.sleepTask) {
        displayShowValues(false);
    } else {
        if (nav.changed(0)) {
            nav.doOutput();
            display.displayWindow(0, 0, display.width(), display.height());  // Refresh screen in partial mode
        }
    }
#endif
}

void initMenu() {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("-->[MENU] Initializing menu...");
#endif
    menuInitialized = true;
    mustInitMenu = false;
    waitingForImprov = false;
    publishMQTTLogData("-->[MENU] Initializing menu...");
#ifdef SUPPORT_TFT
    tft.loadFont(SMALL_FONT);
#endif
    nav.idleTask = idle;  // function to be called when menu is suspended
    nav.idleOn(idle);     // start the menu in idle state
    nav.timeOut = 20;
    nav.showTitle = true;
    options->invertFieldKeys = true;
    nav.useUpdateEvent = true;
    informationMenu[0].disable();  // Make information field unselectable
    informationMenu[1].disable();
    informationMenu[2].disable();
    informationMenu[3].disable();
    informationMenu[4].disable();
    informationMenu[5].disable();
    // bleConfigMenu[0].disable(); // Disable turning OFF BLE to avoid restart of device
    if (!activeWIFI) {
        activeMQTTMenu[0].disable();  // Make MQTT active field unselectable if WIFI is not active
    }
    batteryConfigMenu[0].disable();  // Make information field unselectable
    temperatureConfigMenu[0].disable();
    setCO2Sensor = selectedCO2Sensor;
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Loaded CO2 Sensor in menu (setCO2Sensor): %d\n", setCO2Sensor);
    Serial.printf("-->[MENU] Loaded CO2 Sensor in menu (selectedCO2Sensor): %d\n", selectedCO2Sensor);
#endif

    loadTempArraysWithActualValues();
    Serial.println("");
    Serial.println("**********************************************************************");
    Serial.println("-->[MENU] Use keys + - * /");
    Serial.println("-->[MENU] to control the menu navigation");
    Serial.println("**********************************************************************");
    Serial.println("");
}

bool menuEntryCharacterReceived() {
    // If the first byte is '*', then it's a command from the serial menu
    if (Serial.available() && Serial.peek() == 0x2A) {
#ifdef DEBUG_ARDUINOMENU
        Serial.println("-->[MENU] Serial command detected.");
#endif
        return true;
    }
    return false;
}

void menuLoop() {
    if (isDownloadingBLE) return;  // Do not run the menu if downloading BLE

    if (mustInitMenu) {
        initMenu();
#ifdef DEBUG_ARDUINOMENU
        Serial.println("-->[MENU] Initializing menu by mustInitMenu = true...");
#endif
    }

    // While we are waiting for Improv-WiFi to start, check if user is trying to access the menu to disable Improv-WiFi
    if ((waitingForImprov) && (menuEntryCharacterReceived())) {
        waitingForImprov = false;
#ifdef DEBUG_ARDUINOMENU
        Serial.println("-->[MENU] Serial command detected. Improv-WiFi disabled.");
        publishMQTTLogData("-->[MENU] Serial command detected. Improv-WiFi disabled.");
#endif
        if (!menuInitialized) initMenu();
    }

    if (!menuInitialized) {
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
        displayShowValues(shouldRedrawDisplay);
#endif
        shouldRedrawDisplay = false;
#ifdef DEBUG_ARDUINOMENU
        static unsigned long lastPrintTime2 = 0;
        if (millis() - lastPrintTime2 >= 1000) {
            Serial.println("-->[MENU] Waiting for Improv-WiFi to start...");
            lastPrintTime2 = millis();
        }
#endif
        return;
    }

#ifdef DEBUG_ARDUINOMENU
    if (Serial.available()) {
        Serial.print("-->[MENU] Received unknow byte: ");
        Serial.println(Serial.peek(), HEX);
    }
#endif

#ifdef DEBUG_ARDUINOMENU
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000) {
        Serial.println("-->[MENU] menuLoop");
        lastPrintTime = millis();
    }
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S3
    // Workaround: Try to avoid Serial TX buffer full if it's not connected to a receiving device. Looks like the issue is just with ESP32 S3
    if ((Serial.availableForWrite() < 100) || (!workingOnExternalPower)) {
        Serial.println("[MENU] Serial TX buffer full or not connected to a receiving device. Restarting Serial...");
        Serial.end();
        delay(10);
        Serial.begin(115200);
    }
#endif

    if (activeWIFI) {
        activeMQTTMenu[0].enable();
    } else {
        activeMQTTMenu[0].disable();
    }

#if defined(SUPPORT_TFT)
    menuLoopTFT();
#elif defined(SUPPORT_OLED)
    menuLoopOLED();
#elif defined(SUPPORT_EINK)
    menuLoopEINK();
#else  // For serial only output with display connected
    if (!nav.sleepTask) {
        if (nav.changed(0)) {
            nav.doOutput();
        }
    }
#endif
}

#endif  // CO2_Gadget_Menu_h
