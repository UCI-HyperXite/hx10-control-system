#include "../../../include/components/c/high_voltage_system.h"

struct HighVoltageSystem highVoltageSystem;

void initializeHighVoltageSystem(){
    highVoltageSystem.pin = CONTACTOR_RELAY;
}

void engageContactors(){
    setHigh(highVoltageSystem.pin);
}

void disengageContactors(){
    setLow(highVoltageSystem.pin);
}