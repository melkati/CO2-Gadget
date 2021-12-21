#include "Button2.h"
#define LONGCLICK_TIME_MS 300 // https://github.com/LennartHennigs/Button2/issues/10
#define BTN_UP 35 // Pinnumber for button for up/previous and select / enter actions
#define BTN_DWN 0 // Pinnumber for button for down/next and back / exit actions
Button2 btnUp(BTN_UP);   // Initialize the up button
Button2 btnDwn(BTN_DWN); // Initialize the down button

void IRAM_ATTR buttonUpISR() {
  if(actualDisplayBrightness==0) // Turn on the display only if it's OFF
  {
    setDisplayBrightness(DisplayBrightness); // Turn on the display at DisplayBrightness brightness    
    actualDisplayBrightness = DisplayBrightness;
    nextTimeToDisplayOff = millis() + (timeToDisplayOff*1000);
  }
}

void buttonsInit() {  
  // Interrupt Service Routine to turn on the display on button UP press 
  attachInterrupt(BTN_UP, buttonUpISR, RISING);

  btnUp.setLongClickTime(LONGCLICK_TIME_MS);  
  btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
  btnUp.setClickHandler([](Button2 &b) {
    // Up
    nav.doNav(downCmd);
  });

  btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
  btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
  btnDwn.setClickHandler([](Button2 &b) {
    // Down
    nav.doNav(upCmd);
  });
}

void reverseButtons(bool reversed) {
  // Interrupt Service Routine to turn on the display on button UP press
  if (reversed) {
    attachInterrupt(BTN_UP, buttonUpISR, RISING);
  } else {
    attachInterrupt(BTN_DWN, buttonUpISR, RISING);
  }

  if (reversed) {
    btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
    btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
    btnDwn.setClickHandler([](Button2 &b) {
      // Up
      nav.doNav(downCmd);
    });

    btnUp.setLongClickTime(LONGCLICK_TIME_MS);
    btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
    btnUp.setClickHandler([](Button2 &b) {
      // Down
      nav.doNav(upCmd);
    });
  } else {
    btnUp.setLongClickTime(LONGCLICK_TIME_MS);
    btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
    btnUp.setClickHandler([](Button2 &b) {
      // Up
      nav.doNav(downCmd);
    });

    btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
    btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
    btnDwn.setClickHandler([](Button2 &b) {
      // Down
      nav.doNav(upCmd);
    });
  }
}

  void buttonsLoop() {
    // Check for button presses
    btnUp.loop();
    btnDwn.loop();
  }
