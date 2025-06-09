#include "../include/initialize_sensors.hpp"

SensorInitializer::SensorInitializer() {
    gyro = std::make_shared<MPU6050>(0x68, false);
    pod_height = std::make_shared<vl6180>(vl6180_initialise(1));

    downstream = std::make_shared<PressureTransducer>(0.1f, 3.2f, 0x40, Reference::downstream());
    upstream   = std::make_shared<PressureTransducer>(0.1f, 3.2f, 0x41, Reference::upstream());
    left_manifold  = std::make_shared<PressureTransducer>(0.1f, 3.2f, 0x44, Reference::left_Manifold());
    right_manifold = std::make_shared<PressureTransducer>(0.1f, 3.2f, 0x45, Reference::right_Manifold());

    ads1 = std::make_shared<Adafruit_ADS1115>(0x48);
    ads2 = std::make_shared<Adafruit_ADS1115>(0x49);
    ads3 = std::make_shared<Adafruit_ADS1115>(0x4A);
    ads4 = std::make_shared<Adafruit_ADS1115>(0x4B);

    encoder = std::make_shared<WheelEncoder>(WHEEL_ENCODER_A_PIN, WHEEL_ENCODER_B_PIN);
}

