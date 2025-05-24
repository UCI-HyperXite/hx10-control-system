#ifndef HIGHVOLTAGESYSTEM_H
#define HIGHVOLTAGESYSTEM_H

#include "../../utils/gpio.h"

struct HighVoltageSystem {
    int pin;
};

void initializeHighVoltageSystem(int pin);
void engageContactors(int pin);
void disengageContactors(int pin);


#endif