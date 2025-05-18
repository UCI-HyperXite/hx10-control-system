#include "brakes.h"
#include "../../utils/gpio.h"

struct Brakes brake;

void initializeBrakes(int pin){
    brake->pin = GPIOPins->PNEUMATICS_RELAY;
    pinMode(pin, OUTPUT);
}

void engage(int pin){
    pinMode(pin, LOW);
}

void disengage(int pin){
    pinMode(pin, HIGH);
}