#include "brakes.h"
#include "gpio.h"

struct Brakes brake;

void initializeBrakes(int pin){
    brake.pin = pin;
}

void engage(int pin){
    pinMode(pin, LOW);
}

void disengage(int pin){
    pinMode(pin, HIGH);
}