#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <Ticker.h>  // library to async functions
Ticker buzz;

uint64_t lastTimeBuzzerBeep = 0;  // Time of last Buzzer loop

bool belowOrangeRange = true;
bool belowRedRange = true;

void wakeUpDisplay() {
    if (actualDisplayBrightness == 0)  // Turn on the display only if it's OFF
    {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
        setDisplayBrightness(DisplayBrightness);  // Turn on the display at DisplayBrightness brightness
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}

void buzzerRedRange() {
    Serial.println("[BUZZ] Buzzer RED range");
    tone(BUZZER_PIN, toneBuzzerBeep + co2, durationBuzzerBeep);
    delay(durationBuzzerBeep * 1.3);
    tone(BUZZER_PIN, toneBuzzerBeep + 250 + co2, durationBuzzerBeep);
    delay(durationBuzzerBeep * 1.3);
    tone(BUZZER_PIN, toneBuzzerBeep + 500 + co2, durationBuzzerBeep);
}

void buzzerOrangeRange() {
    Serial.println("[BUZZ] Buzzer ORANGE range");
    tone(BUZZER_PIN, toneBuzzerBeep + co2, durationBuzzerBeep);
    delay(durationBuzzerBeep * 1.3);
    tone(BUZZER_PIN, toneBuzzerBeep + co2, durationBuzzerBeep);
}

void buzzerLoop() {
    if (!activeBuzzer)
        return;

    if (inMenu) {  // Inside Menu stop BEEPING
        noTone(BUZZER_PIN);
        return;
    }

    if ((millis() - lastTimeBuzzerBeep >= timeBetweenBuzzerBeep * 1000) || (lastTimeBuzzerBeep == 0)) {
        lastTimeBuzzerBeep = millis();

        if (co2 > co2RedRange) {
            if (belowRedRange || repeatBuzzer) {
                wakeUpDisplay();
                buzz.once(0, buzzerRedRange);
                belowRedRange = false;
            }
            return;
        } else if (co2 < (co2RedRange - BUZZER_HYSTERESIS))
            belowRedRange = true;

        if (co2 > co2OrangeRange) {
            if (belowOrangeRange || repeatBuzzer) {
                wakeUpDisplay();
                buzz.once(0, buzzerOrangeRange);
                belowOrangeRange = false;
            }
            return;
        } else if (co2 < (co2OrangeRange - BUZZER_HYSTERESIS))
            belowOrangeRange = true;
        return;
    }
}

void initBuzzer() {
#ifdef SUPPORT_BUZZER
    Serial.println("-->[BUZZ] Initializing Buzzer..");
    pinMode(BUZZER_PIN, OUTPUT);

    // LEDC initialization
    ledcSetup(0, 5000, 8);         // LEDC channel 0, 5000 Hz, 8-bit resolution
    ledcAttachPin(BUZZER_PIN, 0);  // Attach BUZZER_PIN to channel 0
#endif
}

#endif
