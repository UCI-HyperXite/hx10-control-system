#include <iostream>
#include <thread>
#include <chrono>
#include <future>
// #include "../include/components.hpp"

#include "gyro.hpp"
extern "C" {
    #include "ina219.h"
    #include "pod_height.h"
}
#include "pressure_transducer.cpp" //look to delete at the end

void readGyro(MPU6050* gyro)
{
    while (true){
        float ax, ay, az, gx, gy, gz;

        gyro->getAccel(&ax, &ay, &az);
        gyro->getGyro(&gx, &gy, &gz);

        std::cout << "Accel [g]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
        std::cout << "Gyro [deg/s]: Roll=" << gx << " Pitch=" << gy << " Yaw=" << gz << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void readPodHeight(vl6180* pod_height) {
    while (true) {
        int distance = get_distance(*pod_height) + 10;
        std::cout << "[VL6180X] Pod Height: " << distance << " mm" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void readPneumaticPressure(ina219* pressure_transducer){
    float previous_pressure1, previous_pressure2, previous_pressure3 = 0;
    while (true) {
        float pressure = pressure_transducer.read_pressure();

        if (abs(pressure) < abs((previous_pressure1 + previous_pressure2 + previous_pressure3)/3)){
            std::cout << "Upstream Pressure [psi]: " << upstream_pressure_PSI << "\n" << std::endl;
        }
        previous_pressure1 = previous_pressure2;
        previous_pressure2 = previous_pressure3;
        previous_pressure3 = pressure;
    } 
}