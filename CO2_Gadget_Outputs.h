#ifndef CO2_Gadget_Outputs_h
#define CO2_Gadget_Outputs_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********          UNIFIED OUTPUTS: GPIO relays, RGB LEDs, NeoPixel, Buzzer                *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

// Pull in individual output drivers
#include "CO2_Gadget_Neopixel.h"
#include "CO2_Gadget_Buzzer.h"

/*****************************************************************************************************/
// GPIO: relay and RGB LED outputs
/*****************************************************************************************************/

void initOutputsGPIO() {
#ifdef GREEN_PIN
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(GREEN_PIN, LOW);
#endif
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(BLUE_PIN, LOW);
    pinMode(RED_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
// If BTN_WAKEUP is defined and BTN_WAKEUP_IS_TOUCHPAD is not defined or set to 0, set it as input
#if defined(BTN_WAKEUP) && (!defined(BTN_WAKEUP_IS_TOUCHPAD) || BTN_WAKEUP_IS_TOUCHPAD == 0)
    pinMode(BTN_WAKEUP, INPUT_PULLUP);
#endif
}

void outputsRelays() {
    if ((!outputsModeRelay) || (co2 == 0)) return;  // Don't turn on relays until there is CO2 Data
#ifdef GREEN_PIN
    if (co2 >= co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
    }
    if (co2 < co2OrangeRange) {
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
    }
#endif
    if (co2 >= co2OrangeRange) {
        digitalWrite(BLUE_PIN, BLUE_PIN_HIGH);
    }
    if (co2 < co2OrangeRange - PIN_HYSTERESIS) {
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
    }
    if (co2 > co2RedRange) {
        digitalWrite(RED_PIN, RED_PIN_HIGH);
    }
    if (co2 <= co2RedRange - PIN_HYSTERESIS) {
        digitalWrite(RED_PIN, RED_PIN_LOW);
    }
}

void outputsRGBLeds() {
    if ((outputsModeRelay) || (co2 == 0)) return;  // Don't turn on led until there is CO2 Data
    if (co2 > co2RedRange) {
#ifdef GREEN_PIN
        digitalWrite(GREEN_PIN, GREEN_PIN_LOW);
#endif
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        return;
    }
    if (co2 >= co2OrangeRange) {
#ifdef GREEN_PIN
        digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
#endif
        digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
        digitalWrite(RED_PIN, RED_PIN_HIGH);
        return;
    }
#ifdef GREEN_PIN
    digitalWrite(GREEN_PIN, GREEN_PIN_HIGH);
#endif
    digitalWrite(BLUE_PIN, BLUE_PIN_LOW);
    digitalWrite(RED_PIN, RED_PIN_LOW);
}

/*****************************************************************************************************/
// Unified init and loop
/*****************************************************************************************************/

void initOutputs() {
    initOutputsGPIO();
    initNeopixel();
    initBuzzer();
}

void outputsLoop() {
    if (isDownloadingBLE) return;
    outputsRelays();
    outputsRGBLeds();
    neopixelLoop();
    buzzerLoop();
}

#endif  // CO2_Gadget_Outputs_h
