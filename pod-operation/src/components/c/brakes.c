#include <iostream>
#include <stdio.h>
// #include <wiringPi.h>
#include "../../include/components/c/brakes.h"
#include "../../include/utils/gpio.h"

struct Brakes brake;    

void initializeBrakes(){
    brake.pin = PNEUMATICS_RELAY;
}

void engage(int pin){
    printf("Brakes engaged\n");
    setHigh(brake.pin);
}

void disengage(int pin){
    printf("Brakes disengaged\n");
    setLow(brake.pin);
}