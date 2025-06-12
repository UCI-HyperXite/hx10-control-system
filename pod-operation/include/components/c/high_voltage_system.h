#ifndef HIGHVOLTAGESYSTEM_H
#define HIGHVOLTAGESYSTEM_H

#include <wiringPi.h>
#include "../../utils/gpio.h"

struct HighVoltageSystem {
    int pin;
};

void initializeHighVoltageSystem();
void closeContactors();
void openContactors();


#endif