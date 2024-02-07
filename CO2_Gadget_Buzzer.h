#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

// #define BUZZER_DEBUG

void beepBuzzer() {
    static uint16_t numberOfBeepsLeft = 3;
    static uint64_t timeNextBeep = 0;
    if (co2 > co2RedRange) {
#ifdef BUZZER_DEBUG
        Serial.println("[BUZZ] Buzzer beeping...");
#endif
        if (millis() > timeNextBeep) {
            Serial.printf("[BUZZ] Beep %d\n", numberOfBeepsLeft);
            if (numberOfBeepsLeft == 0) {
                timeNextBeep = millis() + timeBetweenBuzzerBeep;
                numberOfBeepsLeft = 3;
                buzzerBeeping = false;
            } else {
                tone(BUZZER_PIN, toneBuzzerBeep, durationBuzzerBeep);
                timeNextBeep = millis() + durationBuzzerBeep + (durationBuzzerBeep * 1.3);
                --numberOfBeepsLeft;
                buzzerBeeping = true;
            }
        }
    }
}

void buzzerLoop() {
#ifdef SUPPORT_BUZZER
    if (!activeBuzzer || inMenu) {  // Inside Menu OR activeBuzzer=OFF stop BEEPING
        noTone(BUZZER_PIN);
        buzzerBeeping = false;
        return;
    }

    if (co2 > co2RedRange) {
        shouldWakeUpDisplay = true;
        beepBuzzer();
    }

    return;
#endif
}

void initBuzzer() {
#ifdef SUPPORT_BUZZER
#ifdef BUZZER_DEBUG
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
