/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                   SETUP BUZZER                                    *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#define BUZZZER_PIN  13 // ESP32 pin GPIO13 connected to piezo buzzer

int lowestco2=10000;
int highestco2=0;

bool downOrangeRange = true;
bool downRedRange = true;


void buzzerLoop(){

    if(!activeAlarm)        
        return;

    if(inMenu){  // Inside Menu stop BEEPING
        noTone(BUZZZER_PIN);
        return;
    }

    if ((millis() - lastTimeAlarmBeep >= timeBetweenAlarmBeep * 1000) || (lastTimeAlarmBeep == 0)) {
        lastTimeAlarmBeep = millis();

        if(co2>co2RedRange){
            if(downRedRange || repeatAlarm){
                tone(BUZZZER_PIN, toneAlarmBeep+co2 , durationAlarmBeep);
                delay(durationAlarmBeep*1.3);
                tone(BUZZZER_PIN, toneAlarmBeep+250+co2 , durationAlarmBeep);
                delay(durationAlarmBeep*1.3);
                tone(BUZZZER_PIN, toneAlarmBeep+500+co2 , durationAlarmBeep);
                downRedRange = false;
            }
            return;
        } else downRedRange = true;

        if(co2>co2OrangeRange){
            if(downOrangeRange || repeatAlarm){
                tone(BUZZZER_PIN, toneAlarmBeep+co2 , durationAlarmBeep);
                delay(durationAlarmBeep*1.3);
                tone(BUZZZER_PIN, toneAlarmBeep+co2 , durationAlarmBeep);
                downOrangeRange = false;
            }
            return;
        } else downOrangeRange = true;
        return;
    }
}