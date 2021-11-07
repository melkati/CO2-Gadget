#define LONGCLICK_MS 300 // https://github.com/LennartHennigs/Button2/issues/10
#define BTN_UP 35 // Pinnumber for button for up/previous and select / enter actions
#define BTN_DWN 0 // Pinnumber for button for down/next and back / exit actions
#include "Button2.h"
Button2 btnUp(BTN_UP);   // Initialize the up button
Button2 btnDwn(BTN_DWN); // Initialize the down button

void buttonsInit() {
  btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });

  btnUp.setClickHandler([](Button2 &b) {
    // Up
    nav.doNav(downCmd);
  });

  btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });

  btnDwn.setClickHandler([](Button2 &b) {
    // Down
    nav.doNav(upCmd);
  });
}

void buttonsLoop() {
  // Check for button presses
  btnUp.loop();
  btnDwn.loop();
}