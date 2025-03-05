#include <iostream>
#include <cassert>
// #include <wiringPi.h>
// #include "pod-operation/include/utils/gpio.h"

void initailizeGPIOs(void){
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
}

void setHigh(int pin){
    digitalWrite(pin, HIGH);
}

void setLow(int pin){
    digitalWrite(pin, LOW);
}