#include "../../include/components/c/high_voltage_system.h"

struct HighVoltageSystem highVoltageSystem;

void initializeHighVoltageSystem(int pin){
    highVoltageSystem.pin = pin;
    highVoltageSystem->pin = GPIOPins->CONTACTOR_RELAY;
    pinMode(pin, OUTPUT);
}

void engageContactors(int pin){
    digitalWrite(pin, HIGH);
}

void disengageContactors(int pin){
    digitalWrite(pin, LOW);
}