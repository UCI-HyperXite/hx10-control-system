#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <iostream>
#include <thread>
#include <chrono>

extern "C" {
    #include "pod_height.h"
}
#include "components/cpp/gyro.hpp"
#include "components/cpp/current_sensor.hpp"
#include "components/cpp/pressure_transducer.cc" //look to delete at the end


void readGyro(MPU6050* gyro);

void readPodHeight(vl6180* pod_height);

void readPneumaticPressure(PressureTransducer* pressure_transducer);

void readCoolantPressure(PressureTransducer* pressure_transducer);

#endif