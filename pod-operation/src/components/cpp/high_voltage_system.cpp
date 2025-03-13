#include <iostream>
#include <stdio.h>
// #include <wiringPi.h>
#include "../../include/components/cpp/high_voltage_system.hpp"
#include "../../include/utils/gpio.h"


void initializeHighVoltageSystem(struct HighVoltageSystem* highVoltageSystem) {
    highVoltageSystem->pin = CONTACTOR_RELAY;
    highVoltageSystem->enable = enable;
    highVoltageSystem->disable = disable;
}

void enable(struct HighVoltageSystem* self){
    std::cout<< "Enabling HV Contactors" << std::endl;
    setHigh(self->pin);
}

void disable(struct HighVoltageSystem* self){
    std::cout<< "Disabling HV Contactors" << std::endl;
    setLow(self->pin);
}