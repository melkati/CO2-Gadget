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

result doAlert(eventMask e, prompt &item);

int test=55;

// TOGGLE(ledCtrl,setLed,"Led: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
//   ,VALUE("On",HIGH,doNothing,noEvent)
//   ,VALUE("Off",LOW,doNothing,noEvent)
// );

int selTest=0;
SELECT(selTest,selMenu,"Select",doNothing,noEvent,noStyle
  ,VALUE("Zero",0,doNothing,noEvent)
  ,VALUE("One",1,doNothing,noEvent)
  ,VALUE("Two",2,doNothing,noEvent)
);

int chooseTest=-1;
CHOOSE(chooseTest,chooseMenu,"Choose",doNothing,noEvent,noStyle
  ,VALUE("First",1,doNothing,noEvent)
  ,VALUE("Second",2,doNothing,noEvent)
  ,VALUE("Third",3,doNothing,noEvent)
  ,VALUE("Last",-1,doNothing,noEvent)
);

//customizing a prompt look!
//by extending the prompt class
class altPrompt:public prompt {
public:
  altPrompt(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t) override {
    return out.printRaw(F("special prompt!"),len);;
  }
};

MENU(subMenu,"Sub-Menu",doNothing,noEvent,noStyle
  ,altOP(altPrompt,"",doNothing,noEvent)
  ,OP("Op",doNothing,noEvent)
  ,EXIT("<Back")
);

const char* constMEM hexDigit MEMMODE="0123456789ABCDEF";
const char* constMEM hexNr[] MEMMODE={"0","x",hexDigit,hexDigit};
char buf1[]="0x11";

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("Calibración",doNothing,noEvent)
  ,OP("Brillo de pantalla",doNothing,noEvent)
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


#define Black RGB565(0,0,0)
#define Red	RGB565(255,0,0)
#define Green RGB565(0,255,0)
#define Blue RGB565(0,0,255)
#define Gray RGB565(128,128,128)
#define LighterRed RGB565(255,150,150)
#define LighterGreen RGB565(150,255,150)
#define LighterBlue RGB565(150,150,255)
#define DarkerRed RGB565(150,0,0)
#define DarkerGreen RGB565(0,150,0)
#define DarkerBlue RGB565(0,0,150)
#define Cyan RGB565(0,255,255)
#define Magenta RGB565(255,0,255)
#define Yellow RGB565(255,255,0)
#define White RGB565(255,255,255)

const colorDef<uint16_t> colors[6] MEMMODE={
  {{(uint16_t)Black,(uint16_t)Black}, {(uint16_t)Black, (uint16_t)Blue,  (uint16_t)Blue}},//bgColor
  {{(uint16_t)Gray, (uint16_t)Gray},  {(uint16_t)White, (uint16_t)White, (uint16_t)White}},//fgColor
  {{(uint16_t)White,(uint16_t)Black}, {(uint16_t)Yellow,(uint16_t)Yellow,(uint16_t)Red}},//valColor
  {{(uint16_t)White,(uint16_t)Black}, {(uint16_t)White, (uint16_t)Yellow,(uint16_t)Yellow}},//unitColor
  {{(uint16_t)White,(uint16_t)Gray},  {(uint16_t)Black, (uint16_t)Blue,  (uint16_t)White}},//cursorColor
  {{(uint16_t)White,(uint16_t)Yellow},{(uint16_t)Blue,  (uint16_t)Red,   (uint16_t)Red}},//titleColor
};

#define MAX_DEPTH 4

serialIn serial(Serial);

//MENU_INPUTS(in,&serial);its single, no need to `chainStream`

//define serial output device
idx_t serialTops[MAX_DEPTH]={0};
serialOut outSerial(Serial,serialTops);

#define tft_WIDTH 240
#define tft_HEIGHT 135
#define fontW 16
#define fontH 24

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
    if (e == idling)
    {
#if defined SUPPORT_TFT
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
#endif
    }
    return proceed;
}

//config myOptions('*','-',defaultNavCodes,false);
#endif