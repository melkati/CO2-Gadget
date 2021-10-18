// Based on https://drive.google.com/file/d/1_qGqs3XpFQRoT-u5-GK8aJk6f0aI7EA3/view?usp=drive_web
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                SETUP ARDUINOMENU                                  *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
#if defined SUPPORT_ARDUINOMENU
#include <menu.h>
#include <menuIO/serialIO.h>
#include <menuIO/TFT_eSPIOut.h>
// #include <menuIO/chainStream.h>
#include <menuIO/esp8266Out.h>//must include this even if not doing web output...

using namespace Menu;

uint16_t tempCalibrationValue = 0;

// namespace Menu {
//   template<typename T>
//   class cancelField:public menuField<T> {
//   protected:
//     T original;  // to be used when cancelling
//     bool editing;
//   public:
//     cancelField(const menuFieldShadow<T>& shadow): menuField<T>(shadow), editing(false) {}
//     void doNav(navNode& nav, navCmd cmd) override {
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
//           if (menuField<T>::tunning || options->nav2D || !menuField<T>::tune())
//             editing = false;
//           break;
//       }
//       menuField<T>::doNav(nav, cmd);
//     }
//   };
// }//namespace Menu

void showPath(navRoot& root) {
  Serial.print("nav level:");
  Serial.print(root.level);
  Serial.print(" path:[");
  for(int n=0;n<=root.level;n++) {
    Serial.print(n?",":"");
    Serial.print(root.path[n].sel);
  }
  Serial.println("]");
}

result showEvent(eventMask e,navNode& nav,prompt& item) {
  Serial.println();
  Serial.println("========");
  Serial.print("Event for target: 0x");
  Serial.println((long)nav.target,HEX);
  showPath(*nav.root);
  Serial.print(e);
  switch(e) {
    case noEvent://just ignore all stuff
      Serial.println(" noEvent");break;
    case activateEvent://this item is about to be active (system event)
      Serial.println(" activateEvent");break;
    case enterEvent://entering navigation level (this menu is now active)
      Serial.println(" enterEvent");break;
    case exitEvent://leaving navigation level
      Serial.println(" exitEvent");break;
    case returnEvent://TODO:entering previous level (return)
      Serial.println(" returnEvent");break;
    case focusEvent://element just gained focus
      Serial.println(" focusEvent");break;
    case blurEvent://element about to lose focus
      Serial.println(" blurEvent");break;
    case selFocusEvent://TODO:child just gained focus
      Serial.println(" selFocusEvent");break;
    case selBlurEvent://TODO:child about to lose focus
      Serial.println(" selBlurEvent");break;
    case updateEvent://Field value has been updated
      Serial.println(" updateEvent");break;
    case anyEvent:
      Serial.println(" anyEvent");break;
  }
  return proceed;
}

result doCalibration400ppm(eventMask e,navNode& nav, prompt &item) {
  Serial.printf("Calibrating sensor at %d", 400);
  Serial.print("action1 event:");
  delay(500);
  Serial.println(e);
  Serial.flush();
  delay(100);
  calibrationValue=400;
  pendingCalibration=true;
  return proceed;
}

result doCalibrationCustom(eventMask e,navNode& nav, prompt &item) {
  tempCalibrationValue = calibrationValue;
  Serial.printf("Calibrating sensor at %d", calibrationValue);
  Serial.print("action1 event:");
  delay(500);
  Serial.println(e);
  Serial.flush();
  delay(100);
  return proceed;
}

MENU(calibrationMenu,"Calibration",doNothing,noEvent,wrapStyle  
  ,OP("Calibrate at 400ppm",doCalibration400ppm,anyEvent)
  ,FIELD(calibrationValue,"Calibrate at","ppm",400,2000,10,10,showEvent,anyEvent,noStyle)
  ,OP("Test event",showEvent,anyEvent)
  ,EXIT("<Back")
);

TOGGLE(autoSelfCalibration,autoSelfCalibrationMenu,"Autom. Cal.: ",doNothing,noEvent,wrapStyle
  ,VALUE("ON",true,doNothing,noEvent)
  ,VALUE("OFF",false,doNothing,noEvent)
);

MENU(co2RangesConfigMenu,"CO2 Sensor",doNothing,noEvent,wrapStyle  
   ,SUBMENU(autoSelfCalibrationMenu)
   ,FIELD(ambientPressureValue,"Pres. Comp.","mbar",0,2000,10,10,doNothing,noEvent,noStyle)
   ,FIELD(altidudeMeters,"Altitude","mtrs",0,9999,10,10,doNothing,noEvent,noStyle)
   ,FIELD(co2OrangeRange,"Orange","ppm",400,2000,10,10,doNothing,noEvent,noStyle)
   ,FIELD(co2RedRange,"Red","ppm",400,2000,10,10,doNothing,noEvent,noStyle)
  ,EXIT("<Back")
);

MENU(wifiConfigMenu,"WIFI Config",doNothing,noEvent,wrapStyle  
   ,OP("Work In Pregress",doNothing,noEvent)
  ,EXIT("<Back")
);

MENU(mqttConfigMenu,"MQTT Config",doNothing,noEvent,wrapStyle  
   ,OP("Work In Pregress",doNothing,noEvent)
  ,EXIT("<Back")
);

MENU(espnowConfigMenu,"ESP-NOW Config",doNothing,noEvent,wrapStyle  
   ,OP("Work In Pregress",doNothing,noEvent)
  ,EXIT("<Back")
);

MENU(batteryConfigMenu,"Battery Config",doNothing,noEvent,wrapStyle  
   ,FIELD(vref,"Voltaje reference","",0,2000,1,1,doNothing,noEvent,noStyle)
  ,EXIT("<Back")
);

MENU(configMenu,"Configuration",doNothing,noEvent,wrapStyle  
  ,SUBMENU(co2RangesConfigMenu)
  ,SUBMENU(wifiConfigMenu)
  ,SUBMENU(mqttConfigMenu)
  ,SUBMENU(espnowConfigMenu)
  ,SUBMENU(batteryConfigMenu)
  ,EXIT("<Back")
);

//when entering main menu
result enterMainMenu(menuOut &o, idleEvent e)
{
  Serial.println("Enter main menu");
  return proceed;
}

MENU(mainMenu,"CO2 Gadget",doNothing,noEvent,wrapStyle
  ,OP("Display brightness",doNothing,noEvent)
  ,FIELD(battery_voltage,"Battery","V",0,9,0,0,doNothing,noEvent,noStyle)
  ,SUBMENU(calibrationMenu)
  ,SUBMENU(configMenu)
  ,EXIT("<Salir")
);

// define menu colors --------------------------------------------------------
#define Black RGB565(0,0,0)
#define Red RGB565(255,0,0)
#define Green RGB565(0,255,0)
#define Blue RGB565(0,0,255)
#define Gray RGB565(128,128,128)
#define LighterRed RGB565(255,150,150)
#define LighterGreen RGB565(150,255,150)
#define LighterBlue RGB565(150,150,255)
#define LighterGray RGB565(211,211,211)
#define DarkerRed RGB565(150,0,0)
#define DarkerGreen RGB565(0,150,0)
#define DarkerBlue RGB565(0,0,150)
#define Cyan RGB565(0,255,255)
#define Magenta RGB565(255,0,255)
#define Yellow RGB565(255,255,0)
#define White RGB565(255,255,255)
#define DarkerOrange RGB565(255,140,0)

// TFT color table
const colorDef<uint16_t> colors[6] MEMMODE = {
  //{{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
  {{(uint16_t)Black, (uint16_t)Black}, {(uint16_t)Black, (uint16_t)Blue,   (uint16_t)Blue}}, //bgColor
  {{(uint16_t)White, (uint16_t)White},  {(uint16_t)White, (uint16_t)White, (uint16_t)White}},//fgColor
  {{(uint16_t)Red, (uint16_t)Red}, {(uint16_t)Yellow, (uint16_t)Yellow, (uint16_t)Yellow}}, //valColor
  {{(uint16_t)White, (uint16_t)White}, {(uint16_t)White, (uint16_t)White, (uint16_t)White}}, //unitColor
  {{(uint16_t)White, (uint16_t)Gray},  {(uint16_t)Black, (uint16_t)Red,  (uint16_t)White}}, //cursorColor
  {{(uint16_t)White, (uint16_t)Yellow}, {(uint16_t)Black,  (uint16_t)Blue,   (uint16_t)Red}}, //titleColor
};

#define MAX_DEPTH 5

serialIn serial(Serial);

//define serial output device
idx_t serialTops[MAX_DEPTH]={0};
serialOut outSerial(Serial,serialTops);

#define tft_WIDTH  240
#define tft_HEIGHT 135
#define fontW 12
#define fontH 18

const panel panels[] MEMMODE = {{0, 0, tft_WIDTH / fontW, tft_HEIGHT / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t eSpiTops[MAX_DEPTH]={0};
TFT_eSPIOut eSpiOut(tft,colors,eSpiTops,pList,fontW,fontH+1);
menuOut* constMEM outputs[] MEMMODE={&outSerial,&eSpiOut};//list of output devices
outputsList out(outputs,sizeof(outputs)/sizeof(menuOut*));//outputs list controller

NAVROOT(nav,mainMenu,MAX_DEPTH,serial,out);

//when menu is suspended
result idle(menuOut &o, idleEvent e)
{
#if defined SUPPORT_TFT
    if(e==idleStart){
      Serial.println("Event idleStart");      
      readBatteryVoltage();
    }
    else if (e == idling)
    {
      Serial.println("Event iddling");
      showValuesTFT(co2);
      Serial.flush();
    }
    else if(e==idleEnd){
      Serial.println("Event idleEnd");
      Serial.flush();
    }    
    else {
      Serial.print("Unhandled event: ");
      Serial.println(e);
      Serial.flush();
    }
    return proceed;
#endif    
}

void menu_init()
{
#if defined SUPPORT_ARDUINOMENU
    nav.idleTask=idle; // function to be used when menu is suspended
    nav.idleOn(idle);
    nav.timeOut=15;
    nav.showTitle=true;
    options->invertFieldKeys=true;    
    nav.useUpdateEvent=true;
#endif
}
#endif