#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "../include/components.hpp"

extern "C" {
    #include "pod_height.h"
}

// void readGyro(MPU6050* gyro)
// {
//     while (true){
//         float ax, ay, az, gx, gy, gz;

//         gyro->getAccel(&ax, &ay, &az);
//         gyro->getGyro(&gx, &gy, &gz);

//         std::cout << "Accel [g]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
//         std::cout << "Gyro [deg/s]: Roll=" << gx << " Pitch=" << gy << " Yaw=" << gz << "\n";

//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }

std::future<void> readGyro(MPU6050* gyro){
    return std::async(std::launch::async, [gyro]() {
        while (true){
            float ax, ay, az, gx, gy, gz;

            gyro->getAccel(&ax, &ay, &az);
            gyro->getGyro(&gx, &gy, &gz);

            std::cout << "Accel [g]: X=" << ax << " Y=" << ay << " Z=" << az << "\n";
            std::cout << "Gyro [deg/s]: Roll=" << gx << " Pitch=" << gy << " Yaw=" << gz << "\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}



// void readPodHeight(vl6180* pod_height) {
//     while (true) {
//         int distance = get_distance(*pod_height) + 10;
//         std::cout << "[VL6180X] Pod Height: " << distance << " mm" << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }

std::future<void> readPodHeight(vl6180* pod_height) {
    return std::async(std::launch::async, [pod_height]() {
        while (true) {
            int distance = get_distance(*pod_height) + 10;
            std::cout << "[VL6180X] Pod Height: " << distance << " mm" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}