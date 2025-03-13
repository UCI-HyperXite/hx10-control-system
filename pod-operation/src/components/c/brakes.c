#include <iostream>
#include <stdio.h>
// #include <wiringPi.h>
#include "../../include/components/c/brakes.h"
#include "../../include/utils/gpio.h"


void initializeBrakes(struct Brakes* brake){
    brake->pin = PNEUMATICS_RELAY;
    brake->engage = engage;
    brake->disengage = disengage;
}

void engage(struct Brakes* self){
    printf("Brakes engaged\n");
    setHigh(self->pin);
}

void disengage(struct Brakes* self){
    printf("Brakes disengaged\n");
    setLow(self->pin);
}