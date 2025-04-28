#ifndef HIGHVOLTAGESYSTEM_H
#define HIGHVOLTAGESYSTEM_H

struct HighVoltageSystem {
    int pin;
};

void initializeHighVoltageSystem(int pin);
void engageContactors(int pin);
void disengageContactors(int pin);


#endif