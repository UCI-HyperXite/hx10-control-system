#include <iostream>
#include <thread>
#include <chrono>

extern "C"
{
#include "components/c/pod_height.h"
#include "components/c/Adafruit_ADS1015.h"
}
#include "./components/cpp/gyro.hpp"
#include "./components/cpp/current_sensor.hpp"
#include "../src/components/cpp/pressure_transducer.cc"
#include "../src/components/cpp/voltage.cpp"
#include "../src/components/cpp/wheel_encoder.cpp"
