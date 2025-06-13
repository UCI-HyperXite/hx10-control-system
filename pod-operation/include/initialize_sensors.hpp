#pragma once

#include <memory>
class SensorInitializer {
public:
    SensorInitializer(); 

    std::shared_ptr<MPU6050> gyro;
    std::shared_ptr<vl6180> pod_height;

    std::shared_ptr<PressureTransducer> downstream;
    std::shared_ptr<PressureTransducer> upstream;
    std::shared_ptr<PressureTransducer> left_manifold;
    std::shared_ptr<PressureTransducer> right_manifold;

    std::shared_ptr<Adafruit_ADS1115> ads1;
    std::shared_ptr<Adafruit_ADS1115> ads2;
    std::shared_ptr<Adafruit_ADS1115> ads3;
    std::shared_ptr<Adafruit_ADS1115> ads4;

    std::shared_ptr<WheelEncoder> encoder;
};

