#include "high_voltage_system.h"
#include "gpio.h"

struct HighVoltageSystem highVoltageSystem;

void initializeHighVoltageSystem(int pin){
    highVoltageSystem.pin = pin;
}

void engageContactors(int pin){
    pinMode(pin, LOW);
}

void disengageContactors(int pin){
    pinMode(pin, HIGH);
}