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

MENU(calibrationMenu,"Calibration",doNothing,noEvent,wrapStyle  
  ,OP("Calibrate at 400ppm",doNothing,noEvent)
  ,OP("Calibrate at 415ppm",doNothing,noEvent)
  ,FIELD(calibrationValue,"Custom","ppm",400,2000,10,10,doNothing,noEvent,noStyle)
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
      Serial.println("Entering menu idleStart");
    }
    else if (e == idling)
    {
        Serial.println("Menu iddling");
        showValuesTFT(co2);
    }
    else if(e==idleEnd){
      Serial.println("Entering menu idleEnd");
      tft.fillScreen(TFT_BLACK);      
      readBatteryVoltage();
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
#endif
}
#endif