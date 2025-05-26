// #include <iostream>
// #include <cassert>
#include <wiringPi.h>
// #include "pod-operation/include/utils/gpio.h"
#include "../../include/utils/gpio.h"

void initailizeGPIOs(void){
    wiringPiSetup();

    pinMode(VL53L0X_FRONT, INPUT);
    pinMode(VL53L0X_BACK, INPUT);
    pinMode(WHEEL_ENCODER_A, INPUT);
    pinMode(WHEEL_ENCODER_B, INPUT);
    pinMode(VL6180X, INPUT);
    pinMode(CONTACTOR_RELAY, OUTPUT);
    pinMode(SIGNAL_LIGHT_RELAY, OUTPUT);
    pinMode(PNEUMATICS_RELAY, OUTPUT);
}

void setHigh(int pin){
    digitalWrite(pin, HIGH);
}

void setLow(int pin){
    digitalWrite(pin, LOW);
}
