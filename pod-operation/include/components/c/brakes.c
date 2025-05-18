#include "brakes.h"
#include "gpio.h"

struct Brakes brake;
brake->pin = PNEUMATICS_RELAY;

void engage(int pin){
    pinMode(pin, LOW);
}

void disengage(int pin){
    pinMode(pin, HIGH);
}