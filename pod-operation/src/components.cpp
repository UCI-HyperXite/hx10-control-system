#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#include "../include/components/components.hpp"

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

void readPneumaticPressure(PressureTransducer* pressure_transducer){
    while (true) {
        float pressure = pressure_transducer->read_pressure_pneumatics();
        std::cout << "Pneumatic Pressure [psi]: " << pressure << "\n" << std::endl;
        usleep(100000);
    } 
}

void readCoolantPressure(PressureTransducer* pressure_transducer){
    while (true) {
        float pressure = pressure_transducer->read_pressure_coolant();
        std::cout << "Coolant Pressure [psi]: " << pressure << "\n" << std::endl;
        usleep(100000);
    }
}