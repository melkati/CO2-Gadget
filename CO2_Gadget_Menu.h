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
#include <menuIO/TFT_eSPIOut.h>
// #include <menuIO/chainStream.h>
#include <menuIO/esp8266Out.h> //must include this even if not doing web output...

using namespace Menu;

//customizing a menu prompt look
class confirmReboot:public menu {
public:
  confirmReboot(constMEM menuNodeShadow& shadow):menu(shadow) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t p) override {
    return idx<0?//idx will be -1 when printing a menu title or a valid index when printing as option
      menu::printTo(root,sel,out,idx,len,p)://when printing title
      out.printRaw((constText*)F("Reboot w/o saving"),len);//when printing as regular option
  }
};

result systemReboot() {
  Serial.println();
  Serial.println("Reboot CO2 Gadget at user request from menu...");
  //do some termination stuff here
  if (sensors.getMainDeviceSelected().equals("SCD30")) {
    Serial.println("Resetting SCD30 sensor...");
    delay(100);
    sensors.scd30.reset();
  }
  ESP.restart();
  return quit;
}

//using the customized menu class
//note that first parameter is the class name
altMENU(confirmReboot,subMenu,"Reboot?",doNothing,noEvent,wrapStyle,(Menu::_menuData|Menu::_canNav)
  ,OP("Yes",systemReboot,enterEvent)
  ,EXIT("Cancel")
);

char tempIPAddress[16];

// list of allowed characters
const char *const digit = "0123456789";
const char *const hexChars MEMMODE = "0123456789ABCDEF";
const char *const alphaNum[] MEMMODE = {" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
const char *const allChars[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!#@$%&/()=+-*^~:.[]{}?¿"};
const char *const ssidChars[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!#@%&/()=-*^~:.{}¿"};

const char *const reducedSet[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyz.-_"};

// field will initialize its size by this string length
char tempMQTTTopic[]    = "                              ";
char tempMQTTClientId[] = "                              ";
char tempMQTTBrokerIP[] = "                              ";
char tempMQTTUser[]     = "                              ";
char tempMQTTPass[]     = "                              ";
char tempWiFiSSID[]     = "                              ";
char tempWiFiPasswrd[]  = "                              ";
char tempHostName[]     = "                              ";
char tempBLEDeviceId[]  = "                              ";

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
  Serial.print("nav level:");
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
  Serial.print("Event for target: 0x");
  Serial.println((long)nav.target, HEX);
  showPath(*nav.root);
  Serial.print(e);
  switch (e) {
  case noEvent: // just ignore all stuff
    Serial.println(" noEvent");
    break;
  case activateEvent: // this item is about to be active (system event)
    Serial.println(" activateEvent");
    break;
  case enterEvent: // entering navigation level (this menu is now active)
    Serial.println(" enterEvent");
    break;
  case exitEvent: // leaving navigation level
    Serial.println(" exitEvent");
    break;
  case returnEvent: // TODO:entering previous level (return)
    Serial.println(" returnEvent");
    break;
  case focusEvent: // element just gained focus
    Serial.println(" focusEvent");
    break;
  case blurEvent: // element about to lose focus
    Serial.println(" blurEvent");
    break;
  case selFocusEvent: // TODO:child just gained focus
    Serial.println(" selFocusEvent");
    break;
  case selBlurEvent: // TODO:child about to lose focus
    Serial.println(" selBlurEvent");
    break;
  case updateEvent: // Field value has been updated
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
  Serial.printf("Calibrating sensor at %d", 400);
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  calibrationValue = 400;
  pendingCalibration = true;
  return quit;
}

result doCalibrationCustom(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Calibrating sensor at %d\n", customCalibrationValue);
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  calibrationValue = customCalibrationValue;
  pendingCalibration = true;
  return quit;
}

result doSavePreferences(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.println("Saving preferences to NVR");
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  putPreferences();
  return quit;
}

result doSetTFTBrightness(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting TFT brightness at %d", TFTBrightness);
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  setTFTBrightness(TFTBrightness);
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
  ,OP("Test menu event", showEvent, anyEvent),
  EXIT("<Back"));

MENU(co2RangesConfigMenu, "CO2 Sensor", doNothing, noEvent, wrapStyle
  ,SUBMENU(autoSelfCalibrationMenu)
  ,FIELD(ambientPressureValue, "Pres. Comp.", "mbar", 0, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(altidudeMeters, "Altitude", "mtrs", 0, 9999, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2OrangeRange, "Orange", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2RedRange, "Red", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,EXIT("<Back"));

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

result doSetActiveWIFI(eventMask e, navNode &nav, prompt &item) {
  if (!activeWIFI) {
    activeMQTT = false;
    disableWiFi();
  } else {
    initWifi();
    activeMQTT = preferences.getBool("activeMQTT", false);
    if ((activeMQTT) && (WiFi.isConnected())) {
      initMQTT();
    }
  }
  fillTempIPAddress();
  return proceed;
}

result doSetWiFiSSID(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting WiFi SSID to #%s#\n", tempWiFiSSID);
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif  
  // Serial.printf("tempWiFiSSID: #%s#\n", tempWiFiSSID);
  wifiSSID = String(tempWiFiSSID);
  // Serial.printf("wifiSSID: #%s#\n", wifiSSID.c_str());
  wifiSSID.trim();
  // Serial.printf("wifiSSID: #%s#\n", wifiSSID.c_str());
  return proceed;
}

result doSetWiFiPasswrd(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting WiFi Password to #%s#\n", tempWiFiPasswrd);
  Serial.print(F("action1 event:"));
  Serial.println(e);
  Serial.flush();
#endif
  wifiPass = String(tempWiFiPasswrd);
  wifiPass.trim();
  return proceed;
}

result doSetHostName(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting WiFi Password to #%s#\n", tempWiFiPasswrd);
  Serial.print(F("action1 event:"));
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

MENU(wifiConfigMenu, "WIFI Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeWIFIMenu)  
  ,EDIT("SSID", tempWiFiSSID, ssidChars, doSetWiFiSSID, exitEvent, wrapStyle)
  #ifndef WIFI_PRIVACY
  ,EDIT("Pass:", tempWiFiPasswrd, allChars, doSetWiFiPasswrd, exitEvent, wrapStyle)
  #endif  
  ,EDIT("Host:", tempHostName, allChars, doSetHostName, exitEvent, wrapStyle)
  ,EXIT("<Back"));


result doSetMQTTTopic(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting MQTT Topic to #%s#\n", tempMQTTTopic);
  Serial.print(F("action1 event:"));
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
  Serial.printf("Setting MQTT Client Id to #%s#\n", tempMQTTClientId);
  Serial.print(F("action1 event:"));
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
  Serial.printf("Setting MQTT Broker IP to: #%s#\n", tempMQTTBrokerIP);
  Serial.print(F("action1 event:"));
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
  Serial.printf("Setting MQTT User to: #%s#\n", tempMQTTBrokerIP);
  Serial.print(F("action1 event:"));
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
  Serial.printf("Setting MQTT Pass to: #%s#\n", tempMQTTPass);
  Serial.print(F("action1 event:"));
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

#if defined SUPPORT_ESPNOW
MENU(espnowConfigMenu, "ESP-NOW Config", doNothing, noEvent, wrapStyle
  ,OP("Work In Progress", doNothing, noEvent)
  ,EXIT("<Back"));
#endif

result doSetvRef(eventMask e, navNode &nav, prompt &item) {
  battery.begin(vRef, voltageDividerRatio, &sigmoidal);
  delay(10);
  battery_voltage = (float)battery.voltage() / 1000;
  nav.target-> dirty = true;
  return proceed;
}

MENU(batteryConfigMenu, "Battery Config", doNothing, noEvent, wrapStyle
  ,FIELD(battery_voltage, "Battery:", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,FIELD(vRef, "Voltage ref:", "", 0, 2000, 10, 10, doSetvRef, anyEvent, noStyle)
  ,FIELD(batteryFullyChargedMillivolts, "Bat Full (mV):", "", 0, 4200, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(batteryDischargedMillivolts, "Bat Empty (mV):", "", 2700, 3700, 10, 10, doNothing, noEvent, noStyle)
  ,EXIT("<Back"));

result doSetTempOffset(eventMask e, navNode &nav, prompt &item) {
  #ifdef DEBUG_ARDUINOMENU
    Serial.printf("[MENU] Setting setTempOffset to %.2f\n",tempOffset);
  #endif
  sensors.setTempOffset(tempOffset);
  preferences.begin("CO2-Gadget", false);
  preferences.putFloat("tempOffset", tempOffset);
  preferences.end();
  nav.target-> dirty = true;
  return proceed;
}

MENU(temperatureConfigMenu, "Temp Config", doNothing, noEvent, wrapStyle
  ,FIELD(temp, "Temp", " deg C", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,altFIELD(decPlaces<1>::menuField,tempOffset,"Offset"," deg C",-50,50,1,0.1,doSetTempOffset,(eventMask)(enterEvent | exitEvent | updateEvent),wrapStyle)
  ,EXIT("<Back"));

TOGGLE(displayOffOnExternalPower, activeDisplayOffMenuOnBattery, "Off on USB: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doNothing, noEvent)
  ,VALUE("OFF", false, doNothing, noEvent));

MENU(displayConfigMenu, "Display Config", doNothing, noEvent, wrapStyle
  ,FIELD(TFTBrightness, "Brightness:", "", 10, 255, 10, 10, doSetTFTBrightness, anyEvent, wrapStyle)
  ,FIELD(timeToDisplayOff, "Time To Off:", "", 0, 900, 5, 5, doNothing, noEvent, wrapStyle)
  ,SUBMENU(activeDisplayOffMenuOnBattery)
  ,EXIT("<Back"));

MENU(configMenu, "Configuration", doNothing, noEvent, wrapStyle
  ,SUBMENU(co2RangesConfigMenu)
  ,SUBMENU(bleConfigMenu)
  ,SUBMENU(wifiConfigMenu)
  ,SUBMENU(mqttConfigMenu)
#if defined SUPPORT_ESPNOW  
  ,SUBMENU(espnowConfigMenu)
#endif  
  ,SUBMENU(batteryConfigMenu)
  ,SUBMENU(temperatureConfigMenu)
  ,SUBMENU(displayConfigMenu)
  ,OP("Save preferences", doSavePreferences, enterEvent)
  ,EXIT("<Back"));

MENU(informationMenu, "Information", doNothing, noEvent, wrapStyle
  ,FIELD(battery_voltage, "Battery", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,OP("Comp" BUILD_GIT, doNothing, noEvent)
  ,OP("Version" CO2_GADGET_VERSION CO2_GADGET_REV, doNothing, noEvent)
  ,EDIT("IP", tempIPAddress, alphaNum, doNothing, noEvent, wrapStyle)
  ,EDIT("BLE Dev. Id", tempBLEDeviceId, alphaNum, doNothing, noEvent, wrapStyle)  
  ,EXIT("<Back"));

// when entering main menu
result enterMainMenu(menuOut &o, idleEvent e) {
#ifdef DEBUG_ARDUINOMENU
  Serial.println("Enter main menu");
#endif
  return proceed;
}

// TOGGLE(rebootMenu, rebootMenu, "Off on USB: ", doNothing,noEvent, wrapStyle
//   ,VALUE("ON", true, doNothing, noEvent)
//   ,VALUE("OFF", false, doNothing, noEvent));

MENU(mainMenu, "CO2 Gadget", doNothing, noEvent, wrapStyle
  ,FIELD(battery_voltage, "Battery", "Volts", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,SUBMENU(informationMenu)
  ,SUBMENU(calibrationMenu)
  ,SUBMENU(configMenu)
  // ,SUBMENU(rebootMenu)
  ,SUBMENU(subMenu)
  ,EXIT("<Exit"));



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

// TFT color table
// const colorDef<uint16_t> colors[6] MEMMODE = {
//     //{{disabled normal,disabled selected}, {enabled normal,  enabled selected, enabled editing}}
//     {{(uint16_t)Black,  (uint16_t)Black},  {(uint16_t)Black,  (uint16_t)Blue,   (uint16_t)Blue}},   // bgColor
//     {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,  (uint16_t)White}},  // fgColor
//     {{(uint16_t)Red,    (uint16_t)Red},    {(uint16_t)Yellow, (uint16_t)Yellow, (uint16_t)Yellow}}, // valColor
//     {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,  (uint16_t)White}},  // unitColor
//     {{(uint16_t)White,  (uint16_t)Gray},   {(uint16_t)Black,  (uint16_t)Red,    (uint16_t)White}},  // cursorColor
//     {{(uint16_t)White,  (uint16_t)Yellow}, {(uint16_t)Black,  (uint16_t)Blue,   (uint16_t)Red}},    // titleColor
// };

const colorDef<uint16_t> colors[6] MEMMODE = {
    //{{disabled normal,disabled selected}, {enabled normal,  enabled selected,       enabled editing}}
    {{(uint16_t)Black,  (uint16_t)Black},  {(uint16_t)Black,  (uint16_t)Blue,         (uint16_t)Blue}},   // bgColor
    {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,        (uint16_t)White}},  // fgColor
    {{(uint16_t)Green,  (uint16_t)Green},  {(uint16_t)Green,  (uint16_t)Green,        (uint16_t)Green}},  // valColor - Numbers
    {{(uint16_t)White,  (uint16_t)White},  {(uint16_t)White,  (uint16_t)White,        (uint16_t)White}},  // unitColor - Numeric field unit color
    {{(uint16_t)White,  (uint16_t)Gray},   {(uint16_t)Black,  (uint16_t)Red,          (uint16_t)White}},  // cursorColor
    {{(uint16_t)White,  (uint16_t)Yellow}, {(uint16_t)Black,  (uint16_t)DarkerOrange, (uint16_t)Red}},    // titleColor - Menu title color
};
// clang-format on

#define MAX_DEPTH 4

serialIn serial(Serial);

// define serial output device
idx_t serialTops[MAX_DEPTH] = {0};
serialOut outSerial(Serial, serialTops);

#define tft_WIDTH 240
#define tft_HEIGHT 135
#define fontW 12
#define fontH 18

const panel panels[] MEMMODE = {{0, 0, tft_WIDTH / fontW, tft_HEIGHT / fontH}};
navNode *nodes[sizeof(panels) /
               sizeof(panel)];      // navNodes to store navigation status
panelsList pList(panels, nodes, 1); // a list of panels and nodes
idx_t eSpiTops[MAX_DEPTH] = {0};
TFT_eSPIOut eSpiOut(tft, colors, eSpiTops, pList, fontW, fontH + 1);
menuOut *constMEM outputs[] MEMMODE = {&outSerial,
                                       &eSpiOut}; // list of output devices
outputsList out(outputs,
                sizeof(outputs) / sizeof(menuOut *)); // outputs list controller

NAVROOT(nav, mainMenu, MAX_DEPTH, serial, out);

String rightPad(String aString,uint8_t aLenght) {
  String tempString = aString;
  while (tempString.length()<aLenght) {
    tempString = tempString + " ";
  }
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("Original String: #");
  Serial.print(aString);
  Serial.println("#");
  Serial.print("Padded String: #");
  Serial.print(tempString);
  Serial.println("#");
  #endif
  return tempString;
} 

void loadTempArraysWithActualValues() {
  String paddedString;

  #ifdef DEBUG_ARDUINOMENU
  Serial.println("-->[MENU] loadTempArraysWithActualValues()");
  #endif

  paddedString = rightPad(rootTopic, 30);
  paddedString.toCharArray(tempMQTTTopic, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempMQTTTopic: #");
  Serial.print(tempMQTTTopic);
  Serial.println("#");
  #endif

  paddedString = rightPad(mqttClientId, 30);
  paddedString.toCharArray(tempMQTTClientId, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempMQTTClientId: #");
  Serial.print(tempMQTTClientId);
  Serial.println("#");
  #endif

  paddedString = rightPad(mqttBroker, 30);
  paddedString.toCharArray(tempMQTTBrokerIP, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.printf("mqttBroker: #%s#\n", mqttBroker.c_str());
  Serial.printf("mqttBroker.length(): %d\n", paddedString.length());
  Serial.print("tempMQTTBrokerIP: #");
  Serial.print(tempMQTTBrokerIP);
  Serial.println("#");
  #endif

  paddedString = rightPad(mqttUser, 30);
  paddedString.toCharArray(tempMQTTUser, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempMQTTUser: #");
  Serial.print(tempMQTTUser);
  Serial.println("#");  
  #endif

  paddedString = rightPad(mqttPass, 30);
  paddedString.toCharArray(tempMQTTPass, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempMQTTPass: #");
  Serial.print(tempMQTTPass);
  Serial.println("#");
  #endif

  paddedString = rightPad(wifiSSID, 30);
  paddedString.toCharArray(tempWiFiSSID, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempWiFiSSID: #");
  Serial.print(tempWiFiSSID);
  Serial.println("#");
  #endif

  paddedString = rightPad(wifiPass, 30);
  paddedString.toCharArray(tempWiFiPasswrd, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempWiFiPasswrd: #");
  Serial.print(tempWiFiPasswrd);
  Serial.println("#");
  #endif

  paddedString = rightPad(hostName, 30);
  paddedString.toCharArray(tempHostName, paddedString.length());
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempHostName: #");
  Serial.print(tempHostName);
  Serial.println("#");
  #endif

  paddedString = rightPad(gadgetBle.getDeviceIdString(), 30);
  paddedString.toCharArray(tempBLEDeviceId, paddedString.length());  
  #ifdef DEBUG_ARDUINOMENU
  Serial.print("tempBLEDeviceId: #");
  Serial.print(tempBLEDeviceId);
  Serial.println("#");
  #endif

  fillTempIPAddress();
}

// when menu is suspended
result idle(menuOut &o, idleEvent e) {
#if defined SUPPORT_TFT
  if (e == idleStart) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("-->[MQTT] Event idleStart");
#endif
    setInMenu(false);
    readBatteryVoltage();
  } else if (e == idling) { // When out of menu (CO2 Monitor is doing his business)
#ifdef DEBUG_ARDUINOMENU
    Serial.println("-->[MQTT] Event iddling");
    Serial.flush();
#endif
    showValuesTFT(co2);
    readBatteryVoltage();
  } else if (e == idleEnd) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("-->[MQTT] Event idleEnd");
    Serial.flush();
#endif
  setInMenu(true);
  loadTempArraysWithActualValues();
  } else {
#ifdef DEBUG_ARDUINOMENU
    Serial.print("Unhandled event: ");
    Serial.println(e);
    Serial.flush();
#endif
  }
  return proceed;
#endif
}

void menu_init() {
  nav.idleTask = idle; // function to be used when menu is suspended
  nav.idleOn(idle);
  nav.timeOut = 30;
  nav.showTitle = true;
  options->invertFieldKeys = true;
  nav.useUpdateEvent = true;
  mainMenu[0].disable();        // Make battery voltage field unselectable
  informationMenu[0].disable(); // Make information field unselectable
  informationMenu[1].disable();
  informationMenu[2].disable();
  informationMenu[3].disable();
  informationMenu[4].disable();
  // bleConfigMenu[0].disable(); // Disable turning OFF BLE to avoid restart of device
  if (!activeWIFI) {
    activeMQTTMenu[0].disable(); // Make MQTT active field unselectable if WIFI is not active
  }
  batteryConfigMenu[0].disable(); // Make information field unselectable
  temperatureConfigMenu[0].disable();

  loadTempArraysWithActualValues();
  Serial.println("");
  Serial.println("-->[MENU] Use keys + - * /");
  Serial.println("-->[MENU] to control the menu navigation");
  Serial.println("");
}