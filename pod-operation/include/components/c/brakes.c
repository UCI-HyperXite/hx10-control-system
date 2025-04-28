#include "brakes.h"
#include "gpio.h"

struct Brakes brake;

void initializeBrakes(int pin){
    brake.pin = pin;
    pinMode(pin, OUTPUT);
}

void engage(int pin){
    digitalWrite(pin, HIGH);
}

void disengage(int pin){
    digitalWrite(pin, LOW);
}