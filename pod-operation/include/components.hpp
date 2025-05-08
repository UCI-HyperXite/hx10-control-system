#include "components/c/brakes.h"
#include "components/c/lim_current.h"
#include "components/c/lim_temperature.h"
#include "components/c/pressure_transducer.h"
#include "components/c/wheel_encoder.h"
#include "components/c/yaw.h"
#include "components/cpp/high_voltage_system.hpp"
#include "components/cpp/inverter.hpp"
#include "components/cpp/signal_light.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include "gyro.hpp"
extern "C" {
    #include "pod_height.h"
}


void readGyro(MPU6050* gyro);

void readPodHeight(vl6180* pod_height);
