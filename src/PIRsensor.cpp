#include <../include/PIRsensor.hpp>

void PIRSensor::PIRTask(){
    if( digitalRead(pir_pin) == HIGH){
        last_movement_time = millis();
    }
};

unsigned int PIRSensor::getLastMovement(){
    int time = millis() - last_movement_time;
    int seconds = time/SECOND;
    return seconds;
};