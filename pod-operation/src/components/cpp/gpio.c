#include <iostream>
#include <cassert>
#include <wiringPi.h>
#include "gpio.h"

void initailizeGPIOs(){
    wiringPiSetup();

    // pinMode(I2C_SDA, );
    // pinMode(I2C_SCL, );

    pinMode(VL53L0X_FRONT, INPUT);
    pinMode(VL53L0X_BACK, INPUT);
    pinMode(WHEEL_ENCODER_A, INPUT);
    pinMode(WHEEL_ENCODER_B, INPUT);
    pinMode(VL6180X, INPUT);
    pinMode(CONTACTOR_RELAY, INPUT);
    pinMode(SIGNAL_LIGHT_RELAY, INPUT);
    pinMode(PNEUMATICS_RELAY, INPUT);
    pinMode(THROTTLE_RELAY, INPUT);
}

void setHigh(int pin){
    digitalWrite(pin, 1);
}

void setLow(int pin){
    digitalWrite(pin, 0);
}