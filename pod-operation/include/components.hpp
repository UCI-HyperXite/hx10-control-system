#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <iostream>
#include <thread>
#include <chrono>

extern "C" {
    #include "components/c/brakes.h"
    #include "components/c/high_voltage_system.h"
    #include "components/c/pod_height.h"
    #include "components/c/Adafruit_ADS1015.h"
}
#include "components/cpp/gyro.hpp"
#include "components/cpp/current_sensor.hpp"
#include "../src/components/cpp/pressure_transducer.cc"
#include "components/cpp/voltage.hpp"
// #include "../src/components/cpp/wheel_encoder.cpp"

void readGyro(MPU6050* gyro);

void readPodHeight(vl6180* pod_height);

void readPneumaticPressure(PressureTransducer* pressure_transducer);

void readCoolantPressure(PressureTransducer* pressure_transducer);

void readADS1015ThermistorLoop(Adafruit_ADS1115* ads);

// void readEncoder(WheelEncoder* encoder);

#endif