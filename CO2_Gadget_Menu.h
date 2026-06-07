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

#ifdef SUPPORT_OLED
#include <menuIO/chainStream.h>
#include <menuIO/u8g2Out.h>
#endif

#ifdef SUPPORT_EINK
#include "GxEPD2Out.h"
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
const char *const bthomeKeyChars[] MEMMODE = {"0123456789abcdefABCDEF-"};
const char *const reducedSet[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyz.-_"};

// field will initialize its size by this string length
char tempMQTTTopic[] = "                              ";
char tempMQTTClientId[] = "                              ";
char tempMQTTBrokerIP[] = "                              ";
char tempMQTTUser[] = "                              ";
char tempMQTTPass[] = "                              ";
char tempWiFiSSID[33] = "                                ";
char tempWiFiPasswrd[64] = "                                                               ";
char tempHostName[] = "                              ";
char tempBLEDeviceId[] = "                              ";
char tempBTHomeBindKey[33] = "                                ";
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

void copyStringToCharArray(const String &source, char *destination, size_t size, const char *label);
void clearSerialLineEndings();
bool readSerialLine(const char *prompt, String &value, size_t maxLength, bool hideInput, unsigned long timeoutMs);
bool serialWizardCanceled(const String &value, const char *wizardName);

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
result doSetEnableBLE(eventMask e, navNode &nav, prompt &item) {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("enableBLE", enableBLE);
  preferences.end();
  if (enableBLE) {
    initBLE();
  } else {
    disableBLE();
  }
  return proceed;
}

TOGGLE(enableBLE, enableBLEMenu, "BLE Enable: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetEnableBLE, exitEvent)
  ,VALUE("OFF", false, doSetEnableBLE, exitEvent));

result doSetActiveBLE(eventMask e, navNode &nav, prompt &item) {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("activeBLE", activeBLE);
  preferences.end();
  if (enableBLE && activeBLE) {
    initBLE();
  } else if (!activeBLE && !activeBTHome) {
    disableBLE();
  }
  return proceed;
}

TOGGLE(activeBLE, activeBLEMenu, "MyAmbience: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveBLE, exitEvent)
  ,VALUE("OFF", false, doSetActiveBLE, exitEvent));

#ifdef SUPPORT_BTHOME_BLE
result doSetActiveBTHome(eventMask e, navNode &nav, prompt &item) {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("activeBTHome", activeBTHome);
  preferences.end();
  if (enableBLE && activeBTHome) {
    initBLE();
  } else if (!activeBLE && !activeBTHome) {
    disableBLE();
  }
  return proceed;
}

TOGGLE(activeBTHome, activeBTHomeMenu, "BTHome: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveBTHome, exitEvent)
  ,VALUE("OFF", false, doSetActiveBTHome, exitEvent));

result doSetBTHomeEncryption(eventMask e, navNode &nav, prompt &item) {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("bthomeEncrypt", bthomeEncryption);
  preferences.end();
  return proceed;
}

TOGGLE(bthomeEncryption, bthomeEncryptionMenu, "BTHome Enc: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doSetBTHomeEncryption, exitEvent)
  ,VALUE("OFF", false, doSetBTHomeEncryption, exitEvent));

result doSetBTHomeBindKey(eventMask e, navNode &nav, prompt &item) {
  String newBindKey = String(tempBTHomeBindKey);
  newBindKey.trim();
  if ((newBindKey.length() > 0) && !setBTHomeBindKey(newBindKey)) {
    Serial.println("-->[MENU] BTHome bind key unchanged.");
    copyStringToCharArray(rightPad(bthomeBindKey, sizeof(tempBTHomeBindKey) - 1), tempBTHomeBindKey, sizeof(tempBTHomeBindKey), "tempBTHomeBindKey");
    return proceed;
  }
  preferences.begin("CO2-Gadget", false);
  preferences.putString("bthomeBindKey", bthomeBindKey);
  preferences.end();
  return proceed;
}

result doSerialBTHomeBindKey(eventMask e, navNode &nav, prompt &item) {
  String newBindKey;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial BTHome bind key setup");
  Serial.println("-->[MENU] Paste/type 32 hex characters, then press Enter.");
  Serial.println("-->[MENU] Spaces, ':' and '-' separators are accepted.");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current key.");
  Serial.println("-->[MENU] Enter a single dash (-) to generate a new key.");
  Serial.println("**********************************************************************");

  if (!readSerialLine("-->[MENU] BTHome key: ", newBindKey, 47, true, 120000)) return quit;
  if (serialWizardCanceled(newBindKey, "Serial BTHome bind key setup")) return quit;
  newBindKey.trim();
  if (newBindKey.length() == 0) {
    Serial.println("-->[MENU] BTHome bind key unchanged.");
    return quit;
  }

  if (!setBTHomeBindKey(newBindKey)) {
    Serial.println("-->[MENU] BTHome bind key unchanged.");
    return quit;
  }

  preferences.begin("CO2-Gadget", false);
  preferences.putString("bthomeBindKey", bthomeBindKey);
  preferences.end();

#ifdef WIFI_PRIVACY
  copyStringToCharArray(rightPad(" ", sizeof(tempBTHomeBindKey) - 1), tempBTHomeBindKey, sizeof(tempBTHomeBindKey), "tempBTHomeBindKey");
#else
  copyStringToCharArray(rightPad(bthomeBindKey, sizeof(tempBTHomeBindKey) - 1), tempBTHomeBindKey, sizeof(tempBTHomeBindKey), "tempBTHomeBindKey");
#endif

  Serial.println("-->[MENU] BTHome bind key saved.");
  Serial.println("-->[MENU] BTHome bind key: " + bthomeBindKey);
  nav.target->dirty = true;
  return quit;
}
#endif

MENU(bleConfigMenu, "BLE Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(enableBLEMenu)
  ,SUBMENU(activeBLEMenu)
#ifdef SUPPORT_BTHOME_BLE
  ,SUBMENU(activeBTHomeMenu)
  ,SUBMENU(bthomeEncryptionMenu)
  ,OP("BTHome key", doSerialBTHomeBindKey, enterEvent)
#endif
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

void clearSerialLineEndings() {
  while (Serial.available() && (Serial.peek() == '\r' || Serial.peek() == '\n')) {
    Serial.read();
  }
}

bool readSerialLine(const char *prompt, String &value, size_t maxLength, bool hideInput, unsigned long timeoutMs = 120000) {
  clearSerialLineEndings();
  value = "";
  Serial.print(prompt);
  unsigned long start = millis();

  while (millis() - start < timeoutMs) {
    while (Serial.available()) {
      char c = static_cast<char>(Serial.read());

      if (c == '\r' || c == '\n') {
        Serial.println();
        clearSerialLineEndings();
        return true;
      }

      if ((c == '\b') || (c == 0x7F)) {
        if (value.length() > 0) {
          value.remove(value.length() - 1);
          Serial.print("\b \b");
        }
        continue;
      }

      if ((c >= 32) && (c <= 126) && (value.length() < maxLength)) {
        value += c;
        Serial.write(hideInput ? '*' : c);
      }

      start = millis();
    }
    delay(10);
    yield();
  }

  Serial.println();
  Serial.println("-->[MENU] Serial input timed out. Settings unchanged.");
  return false;
}

bool serialWizardCanceled(const String &value, const char *wizardName) {
  if (value != "/") {
    return false;
  }

  Serial.println("-->[MENU] " + String(wizardName) + " canceled. Settings unchanged.");
  return true;
}

void refreshWiFiTempArrays() {
  copyStringToCharArray(rightPad(wifiSSID, sizeof(tempWiFiSSID) - 1), tempWiFiSSID, sizeof(tempWiFiSSID), "tempWiFiSSID");
#ifdef WIFI_PRIVACY
  copyStringToCharArray(rightPad(" ", sizeof(tempWiFiPasswrd) - 1), tempWiFiPasswrd, sizeof(tempWiFiPasswrd), "tempWiFiPasswrd");
#else
  copyStringToCharArray(rightPad(wifiPass, sizeof(tempWiFiPasswrd) - 1), tempWiFiPasswrd, sizeof(tempWiFiPasswrd), "tempWiFiPasswrd");
#endif
}

void saveSerialWiFiSettings(bool reconnect) {
  refreshWiFiTempArrays();
  saveWifiCredentials();
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("activeWIFI", activeWIFI);
  preferences.end();

  if (reconnect && activeWIFI) {
    Serial.println("-->[MENU] Trying to connect WiFi...");
    initWifi();
    fillTempIPAddress();
  }
}

result doSerialWiFiSSIDSetup(eventMask e, navNode &nav, prompt &item) {
  String newSSID;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial WiFi SSID setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current SSID: " + String(wifiSSID.length() > 0 ? wifiSSID : "(not set)"));

  if (!readSerialLine("-->[MENU] SSID: ", newSSID, 32, false)) return quit;
  if (serialWizardCanceled(newSSID, "Serial WiFi SSID setup")) return quit;
  newSSID.trim();
  if (newSSID.length() == 0) {
    Serial.println("-->[MENU] Empty SSID. WiFi SSID unchanged.");
    return quit;
  }

  wifiSSID = newSSID;
  activeWIFI = true;
  saveSerialWiFiSettings(true);
  Serial.println("-->[MENU] WiFi SSID saved: " + wifiSSID);
  nav.target->dirty = true;
  return quit;
}

result doSerialWiFiPasswordSetup(eventMask e, navNode &nav, prompt &item) {
  String newPassword;
  String passwordStatus;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial WiFi password setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current password.");
  Serial.println("-->[MENU] Enter a single dash (-) to clear the password for open WiFi.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current password: " + String(wifiPass.length() > 0 ? "(set)" : "(not set)"));
  Serial.println("-->[MENU] Current password length: " + String(wifiPass.length()) + ".");

  if (!readSerialLine("-->[MENU] Password: ", newPassword, 63, true)) return quit;
  if (serialWizardCanceled(newPassword, "Serial WiFi password setup")) return quit;
  if (newPassword == "-") {
    wifiPass = "";
    passwordStatus = "cleared";
  } else if (newPassword.length() > 0) {
    wifiPass = newPassword;
    passwordStatus = "changed";
  } else {
    passwordStatus = wifiPass.length() > 0 ? "kept existing" : "not set";
  }

  activeWIFI = true;
  saveSerialWiFiSettings(true);
  Serial.println("-->[MENU] WiFi password: " + passwordStatus + ".");
  Serial.println("-->[MENU] WiFi password length: " + String(wifiPass.length()) + ".");
  if ((wifiPass.length() > 0) && (wifiPass.length() < 8)) {
    Serial.println("-->[MENU] Warning: WPA/WPA2 passwords are normally 8-63 characters.");
  }
  nav.target->dirty = true;
  return quit;
}

result doSerialHostNameSetup(eventMask e, navNode &nav, prompt &item) {
  String newHostName;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial hostname setup");
  Serial.println("-->[MENU] Type the hostname, then press Enter.");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current hostname.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current hostname: " + hostName);

  if (!readSerialLine("-->[MENU] Hostname: ", newHostName, 31, false)) return quit;
  if (serialWizardCanceled(newHostName, "Serial hostname setup")) return quit;
  newHostName.trim();
  if (newHostName.length() == 0) {
    Serial.println("-->[MENU] Hostname unchanged.");
    return quit;
  }

  hostName = newHostName;
  copyStringToCharArray(rightPad(hostName, 30), tempHostName, 30, "tempHostName");

  preferences.begin("CO2-Gadget", false);
  preferences.putString("hostName", hostName);
  preferences.end();

  Serial.println("-->[MENU] Hostname saved: " + hostName);
  if (activeWIFI) {
    Serial.println("-->[MENU] Restarting WiFi with new hostname...");
    initWifi();
    fillTempIPAddress();
  }
  nav.target->dirty = true;
  return quit;
}

bool readSerialIPAddress(const char *prompt, IPAddress &address, bool &changed) {
  String value;
  if (!readSerialLine(prompt, value, 15, false)) return false;
  if (serialWizardCanceled(value, "Serial fixed IP setup")) return false;
  value.trim();
  if (value.length() == 0) {
    changed = false;
    return true;
  }

  IPAddress parsedAddress;
  if (!parsedAddress.fromString(value)) {
    Serial.println("-->[MENU] Invalid IP address: " + value);
    Serial.println("-->[MENU] Fixed IP settings unchanged.");
    return false;
  }

  address = parsedAddress;
  changed = true;
  return true;
}

result doSerialFixedIPSetup(eventMask e, navNode &nav, prompt &item) {
  String staticMode;
  bool newUseStaticIP = useStaticIP;
  bool staticModeChanged = false;
  bool ipChanged = false;
  bool gatewayChanged = false;
  bool subnetChanged = false;
  bool dns1Changed = false;
  bool dns2Changed = false;

  IPAddress newStaticIP = staticIP;
  IPAddress newGateway = gateway;
  IPAddress newSubnet = subnet;
  IPAddress newDns1 = dns1;
  IPAddress newDns2 = dns2;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial fixed IP setup");
  Serial.println("-->[MENU] Leave a value blank to keep the current value.");
  Serial.println("-->[MENU] Enter / alone at any prompt to cancel.");
  Serial.println("-->[MENU] Static IP mode: enter on, off, 1, or 0.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current static IP mode: " + String(useStaticIP ? "ON" : "OFF"));
  Serial.println("-->[MENU] Current IP: " + staticIP.toString());
  Serial.println("-->[MENU] Current gateway: " + gateway.toString());
  Serial.println("-->[MENU] Current subnet: " + subnet.toString());
  Serial.println("-->[MENU] Current DNS1: " + dns1.toString());
  Serial.println("-->[MENU] Current DNS2: " + dns2.toString());

  if (!readSerialLine("-->[MENU] Static IP mode: ", staticMode, 3, false)) return quit;
  if (serialWizardCanceled(staticMode, "Serial fixed IP setup")) return quit;
  staticMode.trim();
  staticMode.toLowerCase();
  if (staticMode.length() > 0) {
    if ((staticMode == "on") || (staticMode == "1")) {
      newUseStaticIP = true;
      staticModeChanged = (newUseStaticIP != useStaticIP);
    } else if ((staticMode == "off") || (staticMode == "0")) {
      newUseStaticIP = false;
      staticModeChanged = (newUseStaticIP != useStaticIP);
    } else {
      Serial.println("-->[MENU] Invalid static IP mode. Fixed IP settings unchanged.");
      return quit;
    }
  }

  if (!readSerialIPAddress("-->[MENU] IP address: ", newStaticIP, ipChanged)) return quit;
  if (!readSerialIPAddress("-->[MENU] Gateway: ", newGateway, gatewayChanged)) return quit;
  if (!readSerialIPAddress("-->[MENU] Subnet: ", newSubnet, subnetChanged)) return quit;
  if (!readSerialIPAddress("-->[MENU] DNS1: ", newDns1, dns1Changed)) return quit;
  if (!readSerialIPAddress("-->[MENU] DNS2: ", newDns2, dns2Changed)) return quit;

  useStaticIP = newUseStaticIP;
  staticIP = newStaticIP;
  gateway = newGateway;
  subnet = newSubnet;
  dns1 = newDns1;
  dns2 = newDns2;

  preferences.begin("CO2-Gadget", false);
  preferences.putBool("useStaticIP", useStaticIP);
  preferences.putString("staticIP", staticIP.toString());
  preferences.putString("gateway", gateway.toString());
  preferences.putString("subnet", subnet.toString());
  preferences.putString("dns1", dns1.toString());
  preferences.putString("dns2", dns2.toString());
  preferences.end();

  Serial.println("-->[MENU] Fixed IP settings saved.");
  Serial.println("-->[MENU] Static IP mode: " + String(useStaticIP ? "ON" : "OFF") + (staticModeChanged ? " (changed)" : " (kept)"));
  Serial.println("-->[MENU] IP address: " + staticIP.toString() + (ipChanged ? " (changed)" : " (kept)"));
  Serial.println("-->[MENU] Gateway: " + gateway.toString() + (gatewayChanged ? " (changed)" : " (kept)"));
  Serial.println("-->[MENU] Subnet: " + subnet.toString() + (subnetChanged ? " (changed)" : " (kept)"));
  Serial.println("-->[MENU] DNS1: " + dns1.toString() + (dns1Changed ? " (changed)" : " (kept)"));
  Serial.println("-->[MENU] DNS2: " + dns2.toString() + (dns2Changed ? " (changed)" : " (kept)"));
  if (activeWIFI) {
    Serial.println("-->[MENU] Restarting WiFi with fixed IP settings...");
    initWifi();
    fillTempIPAddress();
  }
  nav.target->dirty = true;
  return quit;
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

class altPromptWiFiSSID:public prompt {
public:
  altPromptWiFiSSID(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "SSID: " + String(wifiSSID.length() > 0 ? wifiSSID : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptWiFiPass:public prompt {
public:
  altPromptWiFiPass(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Pass: " + String(wifiPass.length() > 0 ? "(set)" : "(not set)") + " len " + String(wifiPass.length());
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptHostName:public prompt {
public:
  altPromptHostName(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Host: " + String(hostName.length() > 0 ? hostName : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptFixedIP:public prompt {
public:
  altPromptFixedIP(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Fixed IP: " + String(useStaticIP ? "ON " : "OFF ") + staticIP.toString();
    return out.printRaw(label.c_str(),len);
  }
};

MENU(wifiConfigMenu, "WIFI Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeWIFIMenu)
  ,altOP(altPromptWiFiSSID, "", doSerialWiFiSSIDSetup, enterEvent)
  ,altOP(altPromptWiFiPass, "", doSerialWiFiPasswordSetup, enterEvent)
  ,altOP(altPromptHostName, "", doSerialHostNameSetup, enterEvent)
#ifdef SUPPORT_OTA
  ,SUBMENU(activeOTAMenu)
#endif
  ,altOP(altPromptFixedIP, "", doSerialFixedIPSetup, enterEvent)
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

void refreshMQTTTempArrays() {
  copyStringToCharArray(rightPad(rootTopic, 30), tempMQTTTopic, 30, "tempMQTTTopic");
  copyStringToCharArray(rightPad(mqttClientId, 30), tempMQTTClientId, 30, "tempMQTTClientId");
  copyStringToCharArray(rightPad(mqttBroker, 30), tempMQTTBrokerIP, 30, "tempMQTTBrokerIP");
  copyStringToCharArray(rightPad(mqttUser, 30), tempMQTTUser, 30, "tempMQTTUser");
#ifdef WIFI_PRIVACY
  copyStringToCharArray(rightPad(" ", 30), tempMQTTPass, 30, "tempMQTTPass");
#else
  copyStringToCharArray(rightPad(mqttPass, 30), tempMQTTPass, 30, "tempMQTTPass");
#endif
}

void saveSerialMQTTSettings(bool reconnect) {
  refreshMQTTTempArrays();
  preferences.begin("CO2-Gadget", false);
  preferences.putString("rootTopic", rootTopic);
  preferences.putString("mqttClientId", mqttClientId);
  preferences.putString("mqttBroker", mqttBroker);
  preferences.putString("mqttUser", mqttUser);
  preferences.putString("mqttPass", mqttPass);
  preferences.end();

  if (reconnect && activeMQTT && activeWIFI && WiFi.isConnected()) {
    Serial.println("-->[MENU] Reconnecting MQTT...");
    initMQTT();
  }
}

bool readSerialMQTTText(const char *promptText, String &target, size_t maxLength, const char *wizardName, bool allowClear) {
  String value;
  if (!readSerialLine(promptText, value, maxLength, false)) return false;
  if (serialWizardCanceled(value, wizardName)) return false;
  value.trim();

  if (allowClear && (value == "-")) {
    target = "";
    return true;
  }
  if (value.length() == 0) {
    Serial.println("-->[MENU] Value unchanged.");
    return false;
  }

  target = value;
  return true;
}

result doSerialMQTTTopicSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial MQTT topic setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current topic: " + String(rootTopic.length() > 0 ? rootTopic : "(not set)"));
  if (!readSerialMQTTText("-->[MENU] Topic: ", rootTopic, 63, "Serial MQTT topic setup", false)) return quit;
  saveSerialMQTTSettings(true);
  Serial.println("-->[MENU] MQTT topic saved: " + rootTopic);
  nav.target->dirty = true;
  return quit;
}

result doSerialMQTTClientIdSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial MQTT client id setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current client id: " + String(mqttClientId.length() > 0 ? mqttClientId : "(not set)"));
  if (!readSerialMQTTText("-->[MENU] Client id: ", mqttClientId, 63, "Serial MQTT client id setup", false)) return quit;
  saveSerialMQTTSettings(true);
  Serial.println("-->[MENU] MQTT client id saved: " + mqttClientId);
  nav.target->dirty = true;
  return quit;
}

result doSerialMQTTBrokerSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial MQTT broker setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current broker: " + String(mqttBroker.length() > 0 ? mqttBroker : "(not set)"));
  if (!readSerialMQTTText("-->[MENU] Broker host/IP: ", mqttBroker, 63, "Serial MQTT broker setup", false)) return quit;
  saveSerialMQTTSettings(true);
  Serial.println("-->[MENU] MQTT broker saved: " + mqttBroker);
  nav.target->dirty = true;
  return quit;
}

result doSerialMQTTUserSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial MQTT user setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Enter a single dash (-) to clear the user.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current user: " + String(mqttUser.length() > 0 ? mqttUser : "(not set)"));
  if (!readSerialMQTTText("-->[MENU] User: ", mqttUser, 63, "Serial MQTT user setup", true)) return quit;
  saveSerialMQTTSettings(true);
  Serial.println("-->[MENU] MQTT user: " + String(mqttUser.length() > 0 ? "changed." : "cleared."));
  nav.target->dirty = true;
  return quit;
}

result doSerialMQTTPasswordSetup(eventMask e, navNode &nav, prompt &item) {
  String newPass;
  String passStatus;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial MQTT password setup");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current password.");
  Serial.println("-->[MENU] Enter a single dash (-) to clear the password.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current password: " + String(mqttPass.length() > 0 ? "(set)" : "(not set)"));
  Serial.println("-->[MENU] Current password length: " + String(mqttPass.length()) + ".");

  if (!readSerialLine("-->[MENU] Password: ", newPass, 63, true)) return quit;
  if (serialWizardCanceled(newPass, "Serial MQTT password setup")) return quit;
  if (newPass == "-") {
    mqttPass = "";
    passStatus = "cleared";
  } else if (newPass.length() > 0) {
    mqttPass = newPass;
    passStatus = "changed";
  } else {
    passStatus = mqttPass.length() > 0 ? "kept existing" : "not set";
  }

  saveSerialMQTTSettings(true);
  Serial.println("-->[MENU] MQTT password: " + passStatus + ".");
  Serial.println("-->[MENU] MQTT password length: " + String(mqttPass.length()) + ".");
  nav.target->dirty = true;
  return quit;
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

class altPromptMQTTTopic:public prompt {
public:
  altPromptMQTTTopic(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Topic: " + String(rootTopic.length() > 0 ? rootTopic : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptMQTTClientId:public prompt {
public:
  altPromptMQTTClientId(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Id: " + String(mqttClientId.length() > 0 ? mqttClientId : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptMQTTBroker:public prompt {
public:
  altPromptMQTTBroker(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Broker: " + String(mqttBroker.length() > 0 ? mqttBroker : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptMQTTUser:public prompt {
public:
  altPromptMQTTUser(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "User: " + String(mqttUser.length() > 0 ? mqttUser : "(not set)");
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptMQTTPass:public prompt {
public:
  altPromptMQTTPass(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Pass: " + String(mqttPass.length() > 0 ? "(set)" : "(not set)") + " len " + String(mqttPass.length());
    return out.printRaw(label.c_str(),len);
  }
};

MENU(mqttConfigMenu, "MQTT Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeMQTTMenu)
  ,altOP(altPromptMQTTTopic, "", doSerialMQTTTopicSetup, enterEvent)
  ,altOP(altPromptMQTTClientId, "", doSerialMQTTClientIdSetup, enterEvent)
  ,altOP(altPromptMQTTBroker, "", doSerialMQTTBrokerSetup, enterEvent)
  ,altOP(altPromptMQTTUser, "", doSerialMQTTUserSetup, enterEvent)
  ,altOP(altPromptMQTTPass, "", doSerialMQTTPasswordSetup, enterEvent)
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

bool isMenuHexChar(char c) {
  return ((c >= '0') && (c <= '9')) ||
         ((c >= 'a') && (c <= 'f')) ||
         ((c >= 'A') && (c <= 'F'));
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

bool setESPNowPeerAddressFromString(String peerAddress) {
  peerAddress.trim();
  peerAddress.replace(":", "");
  peerAddress.replace("-", "");
  peerAddress.replace(" ", "");
  peerAddress.toUpperCase();

  if (peerAddress.length() != 12) {
    Serial.println("-->[MENU] Invalid ESP-NOW peer MAC. Expected 12 hex characters.");
    return false;
  }

  for (uint8_t i = 0; i < peerAddress.length(); ++i) {
    if (!isMenuHexChar(peerAddress[i])) {
      Serial.println("-->[MENU] Invalid ESP-NOW peer MAC. Use hexadecimal characters only.");
      return false;
    }
  }

  esp_now_del_peer(peerESPNowAddress);
  hexCharacterStringToBytes(peerESPNowAddress, peerAddress.c_str());
  memcpy(peerInfo.peer_addr, peerESPNowAddress, 6);
  esp_now_add_peer(&peerInfo);
  snprintf(tempESPNowAddress, sizeof(tempESPNowAddress), "%02X%02X%02X%02X%02X%02X",
           peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
           peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
  return true;
}

String getESPNowPeerAddressString() {
  char peerAddress[18];
  snprintf(peerAddress, sizeof(peerAddress), "%02X:%02X:%02X:%02X:%02X:%02X",
           peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
           peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
  return String(peerAddress);
}

result doSerialESPNowPeerSetup(eventMask e, navNode &nav, prompt &item) {
  String newPeerAddress;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial ESP-NOW peer setup");
  Serial.println("-->[MENU] Enter peer MAC, then press Enter.");
  Serial.println("-->[MENU] Accepted formats: AABBCCDDEEFF or AA:BB:CC:DD:EE:FF.");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current peer.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current peer: " + getESPNowPeerAddressString());

  if (!readSerialLine("-->[MENU] Peer MAC: ", newPeerAddress, 17, false)) return quit;
  if (serialWizardCanceled(newPeerAddress, "Serial ESP-NOW peer setup")) return quit;
  newPeerAddress.trim();
  if (newPeerAddress.length() == 0) {
    Serial.println("-->[MENU] ESP-NOW peer unchanged.");
    return quit;
  }

  if (!setESPNowPeerAddressFromString(newPeerAddress)) {
    Serial.println("-->[MENU] ESP-NOW peer unchanged.");
    return quit;
  }

  preferences.begin("CO2-Gadget", false);
  preferences.putBytes("peerESPNow", peerESPNowAddress, 6);
  preferences.end();

  Serial.println("-->[MENU] ESP-NOW peer saved: " + getESPNowPeerAddressString());
  nav.target->dirty = true;
  return quit;
}

class altPromptESPNowPeer:public prompt {
public:
  altPromptESPNowPeer(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Peer MAC: " + getESPNowPeerAddressString();
    return out.printRaw(label.c_str(),len);
  }
};

MENU(espnowConfigMenu, "ESP-NOW Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeESPNOWMenu)
  ,FIELD(timeBetweenESPNowPublish, "TX Time: ", " Secs", 10, 360, 10, 100, doNothing, noEvent, noStyle)
  ,FIELD(boardIdESPNow, "Board ID: ", "", 0, 254, 1, 10, doNothing, noEvent, noStyle)
  ,altOP(altPromptESPNowPeer, "", doSerialESPNowPeerSetup, enterEvent)
  ,EXIT("<Back"));
#endif // SUPPORT_ESPNOW

result doSetvRef(eventMask e, navNode &nav, prompt &item) {
  battery.begin(vRef, voltageDividerRatio, &asigmoidal);
  delay(10);
  batteryVoltage = (float)battery.voltage() / 1000;
  nav.target-> dirty = true;
  return proceed;
}

TOGGLE(hasBattery, hasBatteryMenu, "Has battery: ", doNothing, noEvent, wrapStyle
  ,VALUE("ON", true, doNothing, noEvent)
  ,VALUE("OFF", false, doNothing, noEvent));

class altPromptBatteryVoltage:public prompt {
public:
  altPromptBatteryVoltage(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String batteryStatus = "Battery: " + String(batteryVoltage, 2) + "V";
    return out.printRaw(batteryStatus.c_str(),len);
  }
};

MENU(batteryConfigMenu, "Battery Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(hasBatteryMenu)
  ,altOP(altPromptBatteryVoltage, "", doNothing, noEvent)
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
  #ifdef SUPPORT_EINK
  if (displayReverse) {
    display.setRotation(3);
  } else {
    display.setRotation(1);
  }
  #endif
  nav.target-> dirty = true;
  return proceed;
}

TOGGLE(displayReverse, activeDisplayReverse, "Orient: ", doNothing, noEvent, wrapStyle
  ,VALUE("Normal",   false, doDisplayReverse, enterEvent)
  ,VALUE("Reversed", true,  doDisplayReverse, enterEvent));

TOGGLE(displayShowTemperature, activeDisplayShowTemperature, "Temp: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doNothing, enterEvent)
  ,VALUE("Show", true,  doNothing, enterEvent));

TOGGLE(displayShowHumidity, activeDisplayShowHumidity, "Humidity: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doNothing, enterEvent)
  ,VALUE("Show", true,  doNothing, enterEvent));

TOGGLE(displayShowBattery, activeDisplayShowBattery, "Battery: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doNothing, enterEvent)
  ,VALUE("Show", true,  doNothing, enterEvent));

TOGGLE(displayShowCO2, activeDisplayShowCO2, "CO2: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doNothing, enterEvent)
  ,VALUE("Show", true,  doNothing, enterEvent));

TOGGLE(displayShowPM25, activeDisplayShowPM25, "PM2.5: ", doNothing, noEvent, wrapStyle
  ,VALUE("Hide", false, doNothing, enterEvent)
  ,VALUE("Show", true,  doNothing, enterEvent));

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

String getOnOffLabel(bool value) {
  return value ? "ON" : "OFF";
}

void saveSerialLowPowerSettings() {
  preferences.begin("CO2-Gadget", false);
  preferences.putBool("displayReverse", displayReverse);
  preferences.putUInt("lowPowerMode", deepSleepData.lowPowerMode);
  preferences.putUInt("waitToDeep", deepSleepData.waitToGoDeepSleepOn1stBoot);
  preferences.putUInt("timeSleeping", deepSleepData.timeSleeping);
  preferences.putUInt("cyclsWifiConn", deepSleepData.activateWiFiEvery);
  preferences.putUInt("cycRedrawDis", deepSleepData.redrawDisplayEveryCycles);
  preferences.putBool("actBLEOnWake", deepSleepData.activeBLEOnWake);
  preferences.putBool("actWifiOnWake", deepSleepData.activeWifiOnWake);
  preferences.putBool("actMQTTOnWake", deepSleepData.sendMQTTOnWake);
  preferences.putBool("actESPnowWake", deepSleepData.sendESPNowOnWake);
  preferences.putBool("displayOnWake", deepSleepData.displayOnWake);
  preferences.putBool("dispRevOnWake", deepSleepData.displayReverseOnWake);
  preferences.end();
}

bool readSerialLowPowerUInt(const char *promptText, uint16_t &target, uint16_t minValue, uint16_t maxValue, const char *wizardName) {
  String value;
  if (!readSerialLine(promptText, value, 5, false)) return false;
  if (serialWizardCanceled(value, wizardName)) return false;
  value.trim();

  if (value.length() == 0) {
    Serial.println("-->[MENU] Value unchanged.");
    return false;
  }

  for (uint8_t i = 0; i < value.length(); ++i) {
    if (!isDigit(value[i])) {
      Serial.println("-->[MENU] Invalid number. Setting unchanged.");
      return false;
    }
  }

  unsigned long parsedValue = strtoul(value.c_str(), nullptr, 10);
  if ((parsedValue < minValue) || (parsedValue > maxValue)) {
    Serial.println("-->[MENU] Value must be between " + String(minValue) + " and " + String(maxValue) + ". Setting unchanged.");
    return false;
  }

  target = static_cast<uint16_t>(parsedValue);
  return true;
}

bool readSerialLowPowerBool(const char *promptText, bool &target, const char *wizardName) {
  String value;
  if (!readSerialLine(promptText, value, 3, false)) return false;
  if (serialWizardCanceled(value, wizardName)) return false;
  value.trim();
  value.toLowerCase();

  if (value.length() == 0) {
    Serial.println("-->[MENU] Value unchanged.");
    return false;
  }

  if ((value == "on") || (value == "1") || (value == "yes")) {
    target = true;
    return true;
  }

  if ((value == "off") || (value == "0") || (value == "no")) {
    target = false;
    return true;
  }

  Serial.println("-->[MENU] Enter on, off, 1, or 0. Setting unchanged.");
  return false;
}

result doSerialLowPowerModeSetup(eventMask e, navNode &nav, prompt &item) {
  String value;

  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial low power mode setup");
  Serial.println("-->[MENU] Enter 0/high for HIGH_PERFORMANCE or 1/low for LOW_POWER.");
  Serial.println("-->[MENU] Enter / alone to cancel.");
  Serial.println("-->[MENU] Leave blank to keep the current mode.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current mode: " + getLowPowerModeName(deepSleepData.lowPowerMode));

  if (!readSerialLine("-->[MENU] Mode: ", value, 4, false)) return proceed;
  if (serialWizardCanceled(value, "Serial low power mode setup")) return proceed;
  value.trim();
  value.toLowerCase();
  if (value.length() == 0) {
    Serial.println("-->[MENU] Low power mode unchanged.");
    return proceed;
  }

  if ((value == "0") || (value == "high")) {
    deepSleepData.lowPowerMode = HIGH_PERFORMANCE;
  } else if ((value == "1") || (value == "low")) {
    deepSleepData.lowPowerMode = 1;
  } else {
    Serial.println("-->[MENU] Invalid mode. Low power mode unchanged.");
    return proceed;
  }

  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Low power mode saved: " + getLowPowerModeName(deepSleepData.lowPowerMode));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerWaitSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial wait to deep sleep on 1st boot setup");
  Serial.println("-->[MENU] Enter seconds between 15 and 900.");
  Serial.println("-->[MENU] Enter / alone to cancel. Leave blank to keep current value.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current wait: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + " seconds");
  if (!readSerialLowPowerUInt("-->[MENU] Wait to deep sleep on 1st boot: ", deepSleepData.waitToGoDeepSleepOn1stBoot, 15, 900, "Serial wait to deep sleep on 1st boot setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] First boot wait saved: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + " seconds");
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerSleepSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial sleep time setup");
  Serial.println("-->[MENU] Enter seconds between 0 and 65535.");
  Serial.println("-->[MENU] Enter / alone to cancel. Leave blank to keep current value.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current sleep time: " + String(deepSleepData.timeSleeping) + " seconds");
  if (!readSerialLowPowerUInt("-->[MENU] Sleep seconds: ", deepSleepData.timeSleeping, 0, 65535, "Serial sleep time setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Sleep time saved: " + String(deepSleepData.timeSleeping) + " seconds");
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerWiFiEverySetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial WiFi wake cycle setup");
  Serial.println("-->[MENU] Enter cycles between 0 and 65535. 0 disables scheduled WiFi wake.");
  Serial.println("-->[MENU] Enter / alone to cancel. Leave blank to keep current value.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current activate WiFi every: " + String(deepSleepData.activateWiFiEvery) + " cycles");
  if (!readSerialLowPowerUInt("-->[MENU] Activate WiFi every cycles: ", deepSleepData.activateWiFiEvery, 0, 65535, "Serial WiFi wake cycle setup")) return proceed;
  deepSleepData.cyclesLeftToWiFiConnect = deepSleepData.activateWiFiEvery;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Activate WiFi every saved: " + String(deepSleepData.activateWiFiEvery) + " cycles");
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerRedrawEverySetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial display redraw cycle setup");
  Serial.println("-->[MENU] Enter cycles between 0 and 65535. 0 disables scheduled redraw.");
  Serial.println("-->[MENU] Enter / alone to cancel. Leave blank to keep current value.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current redraw display every: " + String(deepSleepData.redrawDisplayEveryCycles) + " cycles");
  if (!readSerialLowPowerUInt("-->[MENU] Redraw display every cycles: ", deepSleepData.redrawDisplayEveryCycles, 0, 65535, "Serial display redraw cycle setup")) return proceed;
  deepSleepData.cyclesLeftToRedrawDisplay = deepSleepData.redrawDisplayEveryCycles;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Redraw display every saved: " + String(deepSleepData.redrawDisplayEveryCycles) + " cycles");
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerBLEWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial active BLE on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current active BLE on wake: " + getOnOffLabel(deepSleepData.activeBLEOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Active BLE on wake: ", deepSleepData.activeBLEOnWake, "Serial active BLE on wake setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Active BLE on wake saved: " + getOnOffLabel(deepSleepData.activeBLEOnWake));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerWiFiWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial active WiFi on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current active WiFi on wake: " + getOnOffLabel(deepSleepData.activeWifiOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Active WiFi on wake: ", deepSleepData.activeWifiOnWake, "Serial active WiFi on wake setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Active WiFi on wake saved: " + getOnOffLabel(deepSleepData.activeWifiOnWake));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerMQTTWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial send MQTT on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current send MQTT on wake: " + getOnOffLabel(deepSleepData.sendMQTTOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Send MQTT on wake: ", deepSleepData.sendMQTTOnWake, "Serial send MQTT on wake setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Send MQTT on wake saved: " + getOnOffLabel(deepSleepData.sendMQTTOnWake));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerESPNowWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial send ESP-NOW on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current send ESP-NOW on wake: " + getOnOffLabel(deepSleepData.sendESPNowOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Send ESP-NOW on wake: ", deepSleepData.sendESPNowOnWake, "Serial send ESP-NOW on wake setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Send ESP-NOW on wake saved: " + getOnOffLabel(deepSleepData.sendESPNowOnWake));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerDisplayWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial display on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current display on wake: " + getOnOffLabel(deepSleepData.displayOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Display on wake: ", deepSleepData.displayOnWake, "Serial display on wake setup")) return proceed;
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Display on wake saved: " + getOnOffLabel(deepSleepData.displayOnWake));
  nav.target->dirty = true;
  return proceed;
}

result doSerialLowPowerDisplayReverseWakeSetup(eventMask e, navNode &nav, prompt &item) {
  Serial.println();
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Serial display reverse on wake setup");
  Serial.println("-->[MENU] Enter on, off, 1, or 0. Enter / alone to cancel.");
  Serial.println("**********************************************************************");
  Serial.println("-->[MENU] Current display reverse on wake: " + getOnOffLabel(deepSleepData.displayReverseOnWake));
  if (!readSerialLowPowerBool("-->[MENU] Display reverse on wake: ", deepSleepData.displayReverseOnWake, "Serial display reverse on wake setup")) return proceed;
#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
  displayReverse = deepSleepData.displayReverseOnWake;
  setDisplayReverse(displayReverse);
  reverseButtons(displayReverse);
#endif
  saveSerialLowPowerSettings();
  Serial.println("-->[MENU] Display reverse on wake saved: " + getOnOffLabel(deepSleepData.displayReverseOnWake));
  nav.target->dirty = true;
  return proceed;
}

class altPromptLowPowerMode:public prompt {
public:
  altPromptLowPowerMode(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Mode: " + getLowPowerModeName(deepSleepData.lowPowerMode);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerWait:public prompt {
public:
  altPromptLowPowerWait(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Wait to deep sleep on 1st boot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + " seconds";
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerSleep:public prompt {
public:
  altPromptLowPowerSleep(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Time sleeping: " + String(deepSleepData.timeSleeping) + " seconds";
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerWiFiEvery:public prompt {
public:
  altPromptLowPowerWiFiEvery(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Activate WiFi every " + String(deepSleepData.activateWiFiEvery) + " cycles";
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerRedrawEvery:public prompt {
public:
  altPromptLowPowerRedrawEvery(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Redraw display every " + String(deepSleepData.redrawDisplayEveryCycles) + " cycles";
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerBLEWake:public prompt {
public:
  altPromptLowPowerBLEWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Active BLE on wake: " + getOnOffLabel(deepSleepData.activeBLEOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerWiFiWake:public prompt {
public:
  altPromptLowPowerWiFiWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Active WiFi on wake: " + getOnOffLabel(deepSleepData.activeWifiOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerMQTTWake:public prompt {
public:
  altPromptLowPowerMQTTWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Send MQTT on wake: " + getOnOffLabel(deepSleepData.sendMQTTOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerESPNowWake:public prompt {
public:
  altPromptLowPowerESPNowWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Send ESP-NOW on wake: " + getOnOffLabel(deepSleepData.sendESPNowOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerDisplayWake:public prompt {
public:
  altPromptLowPowerDisplayWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Display on wake: " + getOnOffLabel(deepSleepData.displayOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

class altPromptLowPowerDisplayReverseWake:public prompt {
public:
  altPromptLowPowerDisplayReverseWake(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String label = "Display reverse on wake: " + getOnOffLabel(deepSleepData.displayReverseOnWake);
    return out.printRaw(label.c_str(),len);
  }
};

MENU(lowPowerConfigMenu, "Low Power Config", doNothing, noEvent, wrapStyle
  ,altOP(altPromptLowPowerMode, "", doSerialLowPowerModeSetup, enterEvent)
  ,altOP(altPromptLowPowerWait, "", doSerialLowPowerWaitSetup, enterEvent)
  ,altOP(altPromptLowPowerSleep, "", doSerialLowPowerSleepSetup, enterEvent)
  ,altOP(altPromptLowPowerWiFiEvery, "", doSerialLowPowerWiFiEverySetup, enterEvent)
  ,altOP(altPromptLowPowerRedrawEvery, "", doSerialLowPowerRedrawEverySetup, enterEvent)
  ,altOP(altPromptLowPowerBLEWake, "", doSerialLowPowerBLEWakeSetup, enterEvent)
  ,altOP(altPromptLowPowerWiFiWake, "", doSerialLowPowerWiFiWakeSetup, enterEvent)
  ,altOP(altPromptLowPowerMQTTWake, "", doSerialLowPowerMQTTWakeSetup, enterEvent)
  ,altOP(altPromptLowPowerESPNowWake, "", doSerialLowPowerESPNowWakeSetup, enterEvent)
  ,altOP(altPromptLowPowerDisplayWake, "", doSerialLowPowerDisplayWakeSetup, enterEvent)
  ,altOP(altPromptLowPowerDisplayReverseWake, "", doSerialLowPowerDisplayReverseWakeSetup, enterEvent)
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
  ,SUBMENU(lowPowerConfigMenu)
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

class altPromptIPAddress:public prompt {
public:
  altPromptIPAddress(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String ipStatus = "IP: " + String(tempIPAddress);
    return out.printRaw(ipStatus.c_str(),len);
  }
};

class altPromptBLEDeviceId:public prompt {
public:
  altPromptBLEDeviceId(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
    String bleStatus = "BLE Dev. Id: " + String(tempBLEDeviceId);
    return out.printRaw(bleStatus.c_str(),len);
  }
};

MENU(informationMenu, "Information", doNothing, noEvent, wrapStyle
  ,altOP(altPromptBatteryVoltage, "", doNothing, noEvent)
  ,OP("Comp " __DATE__ " at " __TIME__, doNothing, noEvent)
  ,OP("Version " CO2_GADGET_VERSION CO2_GADGET_REV, doNothing, noEvent)
  ,OP("" FLAVOUR, doNothing, noEvent)
  ,altOP(altPromptUptime, "", doNothing, noEvent)
  ,altOP(altPromptIPAddress, "", doNothing, noEvent)
  ,altOP(altPromptBLEDeviceId, "", doNothing, noEvent)
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

#if (!defined(SUPPORT_OLED) && !defined(SUPPORT_TFT) && !defined(SUPPORT_EINK))
menuOut* outputs[]{&outSerial};//No display, only serial output
#elif defined(SUPPORT_EINK) && !defined(SUPPORT_TFT) && !defined(SUPPORT_OLED)
// E-ink output: colors, panel dimensions defined in CO2_Gadget_EINK.h
// displayWidth/displayHeight are globals set per-board in CO2_Gadget_EINK.h
#define eink_fontW 6
#define eink_fontH 9
const colorDef<uint16_t> einkColors[6] MEMMODE = {
    {{0,0},{0,1,1}},  // bgColor  — black bg
    {{65535,65535},{65535,0,0}},  // fgColor — white text
    {{65535,65535},{65535,0,0}},  // valColor
    {{65535,65535},{65535,0,0}},  // unitColor
    {{0,65535},{0,0,65535}},      // cursorColor
    {{65535,65535},{65535,0,0}},  // titleColor
};
const panel einkPanels[] MEMMODE = {{0, 0, 250 / eink_fontW, 122 / eink_fontH}};
navNode *einkNodes[sizeof(einkPanels) / sizeof(panel)];
panelsList einkPList(einkPanels, einkNodes, 1);
idx_t einkTops[MAX_DEPTH] = {0};
GxEPD2Out<decltype(display)> einkOut(display, einkColors, einkTops, einkPList, eink_fontW, eink_fontH);
menuOut* outputs[]{&outSerial, &einkOut};
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

    copyStringToCharArray(rightPad(wifiSSID, sizeof(tempWiFiSSID) - 1), tempWiFiSSID, sizeof(tempWiFiSSID), "tempWiFiSSID");

#ifdef WIFI_PRIVACY
    copyStringToCharArray(rightPad(" ", sizeof(tempWiFiPasswrd) - 1), tempWiFiPasswrd, sizeof(tempWiFiPasswrd), "tempWiFiPasswrd");
#else
    copyStringToCharArray(rightPad(wifiPass, sizeof(tempWiFiPasswrd) - 1), tempWiFiPasswrd, sizeof(tempWiFiPasswrd), "tempWiFiPasswrd");
#endif

    copyStringToCharArray(rightPad(hostName, 30), tempHostName, 30, "tempHostName");

#ifdef SUPPORT_BLE
    copyStringToCharArray(rightPad(provider.getDeviceIdString(), 30), tempBLEDeviceId, 30, "tempBLEDeviceId");
#ifdef SUPPORT_BTHOME_BLE
#ifdef WIFI_PRIVACY
    copyStringToCharArray(rightPad(" ", sizeof(tempBTHomeBindKey) - 1), tempBTHomeBindKey, sizeof(tempBTHomeBindKey), "tempBTHomeBindKey");
#else
    copyStringToCharArray(rightPad(bthomeBindKey, sizeof(tempBTHomeBindKey) - 1), tempBTHomeBindKey, sizeof(tempBTHomeBindKey), "tempBTHomeBindKey");
#endif
#endif
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
    if (nav.sleepTask) {
        displayShowValues(false);
        shouldRedrawDisplay = false;
    } else {
        if (nav.changed(0)) {
            nav.doOutput();
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
    nav.inputBurst = 16;
    nav.showTitle = true;
    options->invertFieldKeys = true;
    nav.useUpdateEvent = true;
    informationMenu[0].disable();  // Make information field unselectable
    informationMenu[1].disable();
    informationMenu[2].disable();
    informationMenu[3].disable();
    informationMenu[4].disable();
    informationMenu[5].disable();
    informationMenu[6].disable();
    // bleConfigMenu[0].disable(); // Disable turning OFF BLE to avoid restart of device
    if (!activeWIFI) {
        activeMQTTMenu[0].disable();  // Make MQTT active field unselectable if WIFI is not active
    }
    batteryConfigMenu[1].disable();  // Make information field unselectable
    temperatureConfigMenu[0].disable();
    setCO2Sensor = selectedCO2Sensor;
#ifdef DEBUG_ARDUINOMENU
    Serial.printf("-->[MENU] Loaded CO2 Sensor in menu (setCO2Sensor): %d\n", setCO2Sensor);
    Serial.printf("-->[MENU] Loaded CO2 Sensor in menu (selectedCO2Sensor): %d\n", selectedCO2Sensor);
#endif

    loadTempArraysWithActualValues();
    Serial.println("");
    Serial.println("**********************************************************************");
    Serial.println("-->[MENU] Use keys + - * / to control the menu navigation.");
    Serial.println("-->[MENU] Press * to enter/open, / to go back/cancel.");
    Serial.println("**********************************************************************");
    Serial.println("");
}

bool menuEntryCharacterReceived() {
    clearSerialLineEndings();
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
#ifdef DEBUG_ARDUINOMENU
    if ((!inMenu) && (shouldRedrawDisplay)) Serial.println("-->[MENU] Entering menu loop with shouldRedrawDisplay: " + String(shouldRedrawDisplay) + " and redrawDisplayOnNextLoop: " + String(redrawDisplayOnNextLoop));
#endif
    if (isDownloadingBLE) return;  // Do not run the menu if downloading BLE

    if ((inMenu) && isMenuDirty) {
#ifdef DEBUG_ARDUINOMENU
        Serial.println("-->[MENU] Menu is dirty. Restarting menu...");
#endif
        isMenuDirty = false;
        mainMenu.dirty = true;
    }

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
#ifdef DEBUG_ARDUINOMENU
        if (shouldRedrawDisplay) Serial.println("-->[MENU] Displaying values while waiting for Improv-WiFi to start with shouldRedrawDisplay: " + String(shouldRedrawDisplay) + " and redrawDisplayOnNextLoop: " + String(redrawDisplayOnNextLoop));
#endif
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

    nav.doInput();

#ifdef DEBUG_ARDUINOMENU
    if (Serial.available()) {
        Serial.print("-->[MENU] Received unknow byte: ");
        Serial.println(Serial.peek(), HEX);
    }
#endif

#ifdef DEBUG_ARDUINOMENU
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000) {
        Serial.println("-->[MENU] menuLoop. shouldRedrawDisplay: " + String(shouldRedrawDisplay) + " and redrawDisplayOnNextLoop: " + String(redrawDisplayOnNextLoop));
        lastPrintTime = millis();
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
