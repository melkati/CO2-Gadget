#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

// #define BUZZER_DEBUG
bool belowRedRange = true;

void beepBuzzer() {
    static uint16_t numberOfBeepsLeft = 2;
    static uint64_t timeNextBeep = 0;
#ifdef BUZZER_DEBUG
    Serial.println("[BUZZ] Buzzer beeping...");
#endif
    if (millis() > timeNextBeep) {
        Serial.printf("[BUZZ] Beep %d\n", numberOfBeepsLeft);
        if (numberOfBeepsLeft == 0) {
            if (timeBetweenBuzzerBeep > 0)
                timeNextBeep = millis() + timeBetweenBuzzerBeep * 1000;
            else
                belowRedRange = false;
            numberOfBeepsLeft = 2;
            buzzerBeeping = false;
        } else {
            tone(BUZZER_PIN, toneBuzzerBeep, durationBuzzerBeep);
            timeNextBeep = millis() + durationBuzzerBeep + (durationBuzzerBeep * 1.3);
            --numberOfBeepsLeft;
            buzzerBeeping = true;
        }
    }
}

void buzzerLoop() {
#ifdef SUPPORT_BUZZER
    if (timeBetweenBuzzerBeep == -1 || inMenu) {  // Inside Menu OR activeBuzzer=OFF stop BEEPING
        buzzerBeeping = false;
        belowRedRange = true;
        return;
    }

    if (co2 > co2RedRange && belowRedRange) {
        shouldWakeUpDisplay = true;
        beepBuzzer();
    } else if (co2 < (co2RedRange - BUZZER_HYSTERESIS))
        belowRedRange = true;
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
