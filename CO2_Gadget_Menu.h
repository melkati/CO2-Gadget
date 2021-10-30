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

char tempIPAddress[16];

// namespace Menu {
//   template<typename T>
//   class cancelField:public menuField<T> {
//   protected:
//     T original;  // to be used when cancelling
//     bool editing;
//   public:
//     cancelField(const menuFieldShadow<T>& shadow): menuField<T>(shadow),
//     editing(false) {} void doNav(navNode& nav, navCmd cmd) override {
//       if (!editing) {
//         original = menuField<T>::target();
//         editing = true;
//       }
//       switch(cmd.cmd) {
//         case escCmd:
//           editing = false;
//           menuField<T>::target() = original;
//           menuField<T>::tunning = false;
//           menuField<T>::dirty = true;
//           // nav.root(options->useUpdateEvent ? updateEvent : enterEvent);
//           // nav.event(options->useUpdateEvent ? updateEvent : enterEvent);
//           nav.root->exit();
//           return;
//         case enterCmd:
//           if (menuField<T>::tunning || options->nav2D ||
//           !menuField<T>::tune())
//             editing = false;
//           break;
//       }
//       menuField<T>::doNav(nav, cmd);
//     }
//   };
// }//namespace Menu

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
  Serial.print("action1 event:");
  Serial.println(e);
  Serial.flush();
#endif
  calibrationValue = 400;
  pendingCalibration = true;
  return quit;
}

result doCalibrationCustom(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Calibrating sensor at %d/n", customCalibrationValue);
  Serial.print("action1 event:");
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
  Serial.print("action1 event:");
  Serial.println(e);
  Serial.flush();
#endif
  putPreferences();
  return quit;
}

result doSetTFTBrightness(eventMask e, navNode &nav, prompt &item) {
#ifdef DEBUG_ARDUINOMENU
  Serial.printf("Setting TFT brightness at %d", TFTBrightness);
  Serial.print("action1 event:");
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
  ,FIELD(customCalibrationValue, "Custom cal", "ppm", 400, 2000, 10, 10, showEvent, enterEvent, noStyle)
  ,OP("Calibrate at custom ppm", doCalibrationCustom, enterEvent)
  ,OP("Test menu event", showEvent, anyEvent), EXIT("<Back"));

MENU(co2RangesConfigMenu, "CO2 Sensor", doNothing, noEvent, wrapStyle
  ,SUBMENU(autoSelfCalibrationMenu)
  ,FIELD(ambientPressureValue, "Pres. Comp.", "mbar", 0, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(altidudeMeters, "Altitude", "mtrs", 0, 9999, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2OrangeRange, "Orange", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,FIELD(co2RedRange, "Red", "ppm", 400, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,EXIT("<Back"));

result doSetActiveWIFI(eventMask e, navNode &nav, prompt &item) {
  if (!activeWIFI) {
    activeMQTT = false;
    WiFi.disconnect();
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

TOGGLE(activeWIFI, activeWIFIMenu, "WIFI Enable: ", doNothing,noEvent, wrapStyle
  ,VALUE("ON", true, doSetActiveWIFI, exitEvent)
  ,VALUE("OFF", false, doSetActiveWIFI, exitEvent));

MENU(wifiConfigMenu, "WIFI Config", doNothing, noEvent, wrapStyle
  ,SUBMENU(activeWIFIMenu)
  ,OP("Work In Progress", doNothing, noEvent)
  , EXIT("<Back"));


// list of allowed characters
const char *const digit = "0123456789";
const char *const hexChars MEMMODE = "0123456789ABCDEF";
const char *const alphaNum[] MEMMODE = {
    " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
char tempTopicMQTT[] =
    "                              "; // field will initialize its
                                      // size by this string length

result doSetMQTTTopic(eventMask e, navNode &nav, prompt &item) {
#ifndef DEBUG_ARDUINOMENU
  Serial.printf("Setting MQTT Topic to %s", tempTopicMQTT);
  Serial.print("action1 event:");
  Serial.println(e);
  Serial.flush();
#endif  
  char * p = strchr (tempTopicMQTT, ' ');  // search for space
  if (p)     // if found truncate at space
    *p = 0;
  rootTopic = tempTopicMQTT;
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
  // ,EDIT(label,target buffer,validators,action,events mask,styles)
  ,SUBMENU(activeMQTTMenu)
  ,EDIT("Topic", tempTopicMQTT, alphaNum, doSetMQTTTopic, exitEvent, wrapStyle)
  ,OP("Work In Progress", doNothing, noEvent)
  , EXIT("<Back"));

MENU(espnowConfigMenu, "ESP-NOW Config", doNothing, noEvent, wrapStyle
  ,OP("Work In Progress", doNothing, noEvent)
  ,EXIT("<Back"));

MENU(batteryConfigMenu, "Battery Config", doNothing, noEvent, wrapStyle
  ,FIELD(vref, "Voltage ref", "", 0, 2000, 10, 10, doNothing, noEvent, noStyle)
  ,EXIT("<Back"));

MENU(configMenu, "Configuration", doNothing, noEvent, wrapStyle
  ,SUBMENU(co2RangesConfigMenu), SUBMENU(wifiConfigMenu)
  ,SUBMENU(mqttConfigMenu), SUBMENU(espnowConfigMenu)
  ,SUBMENU(batteryConfigMenu)
  ,FIELD(TFTBrightness, "Brightness ", "", 10, 255, 10, 10, doSetTFTBrightness, anyEvent, wrapStyle)
  ,OP("Save preferences", doSavePreferences, enterEvent)
  , EXIT("<Back"));

MENU(informationMenu, "Information", doNothing, noEvent, wrapStyle
  ,FIELD(battery_voltage, "Battery", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,OP("Comp: " BUILD_GIT, doNothing, noEvent)
  ,OP("Version: " CO2_GADGET_VERSION CO2_GADGET_REV, doNothing, noEvent)
  ,EDIT("IP: ", tempIPAddress, alphaNum, doNothing, noEvent, wrapStyle)
  ,EXIT("<Back"));

// when entering main menu
result enterMainMenu(menuOut &o, idleEvent e) {
#ifdef DEBUG_ARDUINOMENU
  Serial.println("Enter main menu");
#endif
  return proceed;
}

MENU(mainMenu, "CO2 Gadget  " BUILD_GIT, doNothing, noEvent, wrapStyle
  ,FIELD(battery_voltage, "Battery", "V", 0, 9, 0, 0, doNothing, noEvent, noStyle)
  ,SUBMENU(informationMenu)
  ,SUBMENU(calibrationMenu)
  ,SUBMENU(configMenu)
  ,EXIT("<Salir"));

// clang-format on

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
const colorDef<uint16_t> colors[6] MEMMODE = {
    //{{disabled normal,disabled selected},{enabled normal,enabled selected,
    //enabled editing}}
    {{(uint16_t)Black, (uint16_t)Black},
     {(uint16_t)Black, (uint16_t)Blue, (uint16_t)Blue}}, // bgColor
    {{(uint16_t)White, (uint16_t)White},
     {(uint16_t)White, (uint16_t)White, (uint16_t)White}}, // fgColor
    {{(uint16_t)Red, (uint16_t)Red},
     {(uint16_t)Yellow, (uint16_t)Yellow, (uint16_t)Yellow}}, // valColor
    {{(uint16_t)White, (uint16_t)White},
     {(uint16_t)White, (uint16_t)White, (uint16_t)White}}, // unitColor
    {{(uint16_t)White, (uint16_t)Gray},
     {(uint16_t)Black, (uint16_t)Red, (uint16_t)White}}, // cursorColor
    {{(uint16_t)White, (uint16_t)Yellow},
     {(uint16_t)Black, (uint16_t)Blue, (uint16_t)Red}}, // titleColor
};

#define MAX_DEPTH 5

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

// when menu is suspended
result idle(menuOut &o, idleEvent e) {
#if defined SUPPORT_TFT
  if (e == idleStart) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("Event idleStart");
#endif
    readBatteryVoltage();
  } else if (e == idling) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("Event iddling");
    Serial.flush();
#endif
    showValuesTFT(co2);
  } else if (e == idleEnd) {
#ifdef DEBUG_ARDUINOMENU
    Serial.println("Event idleEnd");
    Serial.flush();
#endif
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
  nav.timeOut = 120;
  nav.showTitle = true;
  options->invertFieldKeys = true;
  nav.useUpdateEvent = true;
  mainMenu[0].disable();        // Make battery voltage field unselectable
  informationMenu[0].disable(); // Make information field unselectable
  informationMenu[1].disable();
  informationMenu[2].disable();
  informationMenu[3].disable();
  if (!activeWIFI) {
    activeMQTTMenu[0].disable(); // Make MQTT active field unselectable if WIFI is not active
  }
  strcpy(tempTopicMQTT, rootTopic.c_str());
  fillTempIPAddress();  
}