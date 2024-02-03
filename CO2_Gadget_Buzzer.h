#ifndef CO2_Gadget_Buzzer_h
#define CO2_Gadget_Buzzer_h

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
	
#include <Ticker.h>  // library to async functions
Ticker buzz_red;
Ticker buzz_orange;

bool belowOrangeRange = true;
bool belowRedRange = true;

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
    if(co2>co2RedRange){
        Serial.println("[BUZZ] Buzzer RED range");
        tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
        delay(durationBuzzerBeep*1.3);
        tone(BUZZER_PIN, toneBuzzerBeep+250+co2 , durationBuzzerBeep);
        delay(durationBuzzerBeep*1.3);
        tone(BUZZER_PIN, toneBuzzerBeep+500+co2 , durationBuzzerBeep);
    }
}

void buzzerOrangeRange(){
    if(co2>co2OrangeRange){
        Serial.println("[BUZZ] Buzzer ORANGE range");
        tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
        delay(durationBuzzerBeep*1.3);
        tone(BUZZER_PIN, toneBuzzerBeep+co2 , durationBuzzerBeep);
    }
}

void buzzerLoop(){

    if(!activeBuzzer || inMenu){ // Inside Menu OR activeBuzzer=OFF stop BEEPING
        if(buzz_red.active() || buzz_orange.active()){
            noTone(BUZZER_PIN);
            buzz_red.detach();
            buzz_orange.detach();
        }
        belowRedRange = true;
        belowOrangeRange = true;
        return;
    }

    if(co2>co2RedRange && belowRedRange){
        wakeUpDisplay();
        buzz_orange.detach();
        belowOrangeRange = true;
        if(repeatBuzzer)  buzz_red.attach(timeBetweenBuzzerBeep, buzzerRedRange);
        else buzz_red.once(0, buzzerRedRange);
        belowRedRange = false;
        return;
    } else if(co2 < (co2RedRange - BUZZER_HYSTERESIS)){
        buzz_red.detach();
        belowRedRange = true;
    } 

    if(co2<co2RedRange && co2>co2OrangeRange && belowOrangeRange){
        wakeUpDisplay();
        if(repeatBuzzer)  buzz_orange.attach(timeBetweenBuzzerBeep, buzzerOrangeRange);
        else buzz_orange.once(0, buzzerOrangeRange);
        belowOrangeRange = false;
        return;
    } else if(co2 < (co2OrangeRange - BUZZER_HYSTERESIS)){
        buzz_orange.detach();
        belowOrangeRange = true;
    } 
    return;
}
#endif
