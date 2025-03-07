
#include <iostream>
#include <cassert>
// #include <wiringPi.h>
#include "../../include/utils/gpio.h"

void initailizeGPIOs(void){
    wiringPiSetup();

    // pinMode(I2C_SDA, );
    // pinMode(I2C_SCL, );

    pinMode(VL53L0X_FRONT, OUTPUT);
    pinMode(VL53L0X_BACK, OUTPUT);
    pinMode(WHEEL_ENCODER_A, INPUT);
    pinMode(WHEEL_ENCODER_B, INPUT);
    pinMode(VL6180X, OUTPUT);
    pinMode(CONTACTOR_RELAY, OUTPUT);
    pinMode(SIGNAL_LIGHT_RELAY, OUTPUT);
    pinMode(PNEUMATICS_RELAY, OUTPUT);
}

void setHigh(int pin){
    digitalWrite(pin, 1);
}

void setLow(int pin){
    digitalWrite(pin, 0);
}
