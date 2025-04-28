#include <iostream>
#include <thread>
#include <chrono>
#include "../include/components.h"

void readGyro(MPU6050* gyro)
{
    while (true){
    float ax, ay, az;
    float gx, gy, gz;

    gyro->getAccel(&ax, &ay, &az);
    gyro->getGyro(&gx, &gy, &gz);

    std::cout << "Accel [g]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
    std::cout << "Gyro [deg/s]: Roll=" << gx << " Pitch=" << gy << " Yaw=" << gz << "\n";
    //std::cout << "--------------------------" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Read every 100ms
    }
}