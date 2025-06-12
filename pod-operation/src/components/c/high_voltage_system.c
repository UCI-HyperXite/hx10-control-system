#include "../../../include/components/c/high_voltage_system.h"

struct HighVoltageSystem highVoltageSystem;

void initializeHighVoltageSystem(){
    highVoltageSystem.pin = CONTACTOR_RELAY;
}

void closeContactors(){
    setHigh(highVoltageSystem.pin);
}

void openContactors(){
    setLow(highVoltageSystem.pin);
}