#include "high_voltage_system.h"
#include "gpio.h"

struct HighVoltageSystem highVoltageSystem;

void initializeHighVoltageSystem(int pin){
    highVoltageSystem.pin = pin;
    pinMode(pin, OUTPUT);
}

void engageContactors(int pin){
    digitalWrite(pin, HIGH);
}

void disengageContactors(int pin){
    digitalWrite(pin, LOW);
}