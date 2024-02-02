#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
	
#include <Ticker.h>  // library to async functions
Ticker buzz;

bool downOrangeRange = true;
bool downRedRange = true;

void wakeUpDisplay(){
 if (actualDisplayBrightness == 0)  // Turn on the display only if it's OFF
    {
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
        setDisplayBrightness(DisplayBrightness);  // Turn on the display at DisplayBrightness brightness
#endif
        lastTimeButtonPressed = millis();
    }
    return;
}   

void buzzerRedRange(){
    Serial.println("[ASYNC] Buzzer RED range");
    tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
    delay(durationBuzzerBeep*1.3);
    tone(BUZZER_PIN, toneBuzzerBeep+250+co2 , durationBuzzerBeep);
    delay(durationBuzzerBeep*1.3);
    tone(BUZZER_PIN, toneBuzzerBeep+500+co2 , durationBuzzerBeep);
}

void buzzerOrangeRange(){
    Serial.println("[ASYNC] Buzzer ORANGE range");
    tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
    delay(durationBuzzerBeep*1.3);
    tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
}

void buzzerLoop(){

    if(!activeBuzzer)        
        return;

    if(inMenu){  // Inside Menu stop BEEPING
        noTone(BUZZER_PIN);
        return;
    }

    if ((millis() - lastTimeBuzzerBeep >= timeBetweenBuzzerBeep * 1000) || (lastTimeBuzzerBeep == 0)) {
        lastTimeBuzzerBeep = millis();

        if(co2>co2RedRange){
            if(downRedRange || repeatBuzzer){
                wakeUpDisplay();
                buzz.once(0, buzzerRedRange);
                downRedRange = false;
            }
            return;
        } else if(co2 < (co2RedRange - BUZZER_HYSTERESIS)) downRedRange = true;

        if(co2>co2OrangeRange){
            if(downOrangeRange || repeatBuzzer){
                wakeUpDisplay();
                buzz.once(0, buzzerOrangeRange);
                downOrangeRange = false;
            }
            return;
        } else if(co2 < (co2OrangeRange - BUZZER_HYSTERESIS)) downOrangeRange = true;
        return;
    }
}
#endif
