#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <Ticker.h>

#define BUFFER_DEBUG

Ticker buzzer;
bool belowRedRange = true;

void wakeUpDisplay() {
    if (actualDisplayBrightness == 0) {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
        setDisplayBrightness(DisplayBrightness);
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}

void beepBuzzer() {
    if (co2 > co2RedRange) {
#ifdef BUFFER_DEBUG
        Serial.println("[BUZZ] Buzzer beeping...");
#endif
        tone(BUZZER_PIN, toneBuzzerBeep, durationBuzzerBeep);
        delay(durationBuzzerBeep * 1.3);
        tone(BUZZER_PIN, toneBuzzerBeep, durationBuzzerBeep);
        delay(durationBuzzerBeep * 1.3);
        tone(BUZZER_PIN, toneBuzzerBeep, durationBuzzerBeep);
    }
}

void buzzerLoop() {
#ifdef SUPPORT_BUZZER
    if (!activeBuzzer || inMenu) {  // Inside Menu OR activeBuzzer=OFF stop BEEPING
        if (buzzer.active()) {
            noTone(BUZZER_PIN);
            buzzer.detach();
        }
        belowRedRange = true;
        return;
    }

    if (co2 > co2RedRange && belowRedRange) {
        wakeUpDisplay();
        if (repeatBuzzer)
            buzzer.attach(timeBetweenBuzzerBeep, beepBuzzer);
        else
            buzzer.once(0, beepBuzzer);
        belowRedRange = false;
        return;
    } else if (co2 < (co2RedRange - BUZZER_HYSTERESIS)) {
        buzzer.detach();
        belowRedRange = true;
    }

    return;
#endif
}

void initBuzzer() {
#ifdef SUPPORT_BUZZER
#ifdef BUFFER_DEBUG
    activeBuzzer = true;
    Serial.println("-->[BUZZ] Initializing Buzzer..");
#endif
    pinMode(BUZZER_PIN, OUTPUT);

    // LEDC initialization
    ledcSetup(0, 5000, 8);
    ledcAttachPin(BUZZER_PIN, 0);
#endif
}

#endif
