#ifndef BRAKES_H
#define BRAKES_H

#include <wiringPi.h>
#include "../../utils/gpio.h"

struct Brakes {
    int pin;
};

void initializeBrakes();
void engageBrakes();
void disengageBrakes();

#endif