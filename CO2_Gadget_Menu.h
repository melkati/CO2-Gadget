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

int test=55;

MENU(calibrationMenu,"Calibration",doNothing,noEvent,wrapStyle  
  ,OP("Calibration at 415ppm",doNothing,noEvent)
  ,OP("Calibration at 415ppm",doNothing,noEvent)
  ,FIELD(calibrationValue,"Custom calibration","ppm",400,2000,10,10,doNothing,noEvent,noStyle)
  ,EXIT("<Salir")
);

MENU(mainMenu,"CO2 Gadget",doNothing,noEvent,wrapStyle
  ,OP("Calibration",doNothing,noEvent)
  ,OP("Display brightness",doNothing,noEvent)
  ,FIELD(battery_voltage,"Battery","V",0,9,0,0,doNothing,noEvent,noStyle)
  ,SUBMENU(calibrationMenu)
  // ,OP("Op2",doNothing,noEvent)
  // ,FIELD(test,"Test","%",0,100,10,1,doNothing,noEvent,wrapStyle)
  // ,SUBMENU(subMenu)
  // ,SUBMENU(setLed)
  // ,OP("LED On",myLedOn,enterEvent)
  // ,OP("LED Off",myLedOff,enterEvent)
  // ,SUBMENU(selMenu)
  // ,SUBMENU(chooseMenu)
  //,OP("Alert test",doAlert,enterEvent)
  // ,EDIT("Hex",buf1,hexNr,doNothing,noEvent,noStyle)
  ,EXIT("<Salir")
);

// define menu colors --------------------------------------------------------
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
//monochromatic color table


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
result idleOld(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.println(F("suspended..."));
    o.println(F("press [select]"));
    o.println(F("to continue"));
  }
  return proceed;
}

//when menu is suspended
result idle(menuOut &o, idleEvent e)
{
#if defined SUPPORT_TFT
    if(e==idleStart){
      // Serial.println("Entering manu idleStart");
      // tft.fillScreen(TFT_BLACK);
      // tft.setFreeFont(FF99);
      // // tft.pushImage(0, 0,  240, 22, titleBar);
    }
    else if (e == idling)
    {
        Serial.println("Menu iddling");
        if (co2 > 9999)
        {
            co2 = 9999;
        }

        tft.fillScreen(TFT_BLACK);

        uint8_t defaultDatum = tft.getTextDatum();

        tft.setTextSize(1);
        tft.setFreeFont(FF90);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        tft.setTextDatum(6); // bottom left
        tft.drawString("CO2", 10, 125);

        tft.setTextDatum(8); // bottom right
        tft.drawString(gadgetBle.getDeviceIdString(), 230, 125);

        // Draw CO2 number
        if (co2 >= 1000)
        {
            tft.setTextColor(TFT_RED, TFT_BLACK);
        }
        else if (co2 >= 700)
        {
            tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        }
        else
        {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        }

        tft.setTextDatum(8); // bottom right
        tft.setTextSize(1);
        tft.setFreeFont(FF95);
        tft.drawString(String(co2), 195, 105);

        // Draw CO2 unit
        tft.setTextSize(1);
        tft.setFreeFont(FF90);
        tft.drawString("ppm", 230, 90);

        // Revert datum setting
        tft.setTextDatum(defaultDatum);

        // set default font for menu
        // tft.setTextSize(1);
        // tft.setFreeFont(FF99);
        tft.setFreeFont(NULL);
        tft.setTextSize(2);
    }
    else if(e==idleEnd){
      Serial.println("Entering menu idleEnd");
      tft.fillScreen(TFT_BLACK);
    }    
    return proceed;
#endif    
}

//config myOptions('*','-',defaultNavCodes,false);

void menu_init()
{
#if defined SUPPORT_ARDUINOMENU
    nav.idleTask=idle; //point a function to be used when menu is suspended
    nav.idleOn(idle);
    nav.timeOut=15;
    // mainMenu[1].disable();
    nav.showTitle=true; //show menu title?
    // outGfx.usePreview=true;//reserve one panel for preview?
#endif
}
#endif