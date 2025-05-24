#ifndef BRAKES_H
#define BRAKES_H

#include "../../utils/gpio.h"

struct Brakes {
    int pin;
};

void initializeBrakes(int pin);
void engage(int pin);
void disengage(int pin);

#endif