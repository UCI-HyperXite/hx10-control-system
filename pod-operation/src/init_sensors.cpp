#include <future>
#include "../include/init_sensors.hpp"

class InitializeAllSensors
{
public:
    // Sensor objects
    MPU6050 gyro;
    vl6180 pod_height;
    PressureTransducer pneumatic_downstream;
    PressureTransducer pneumatic_upstream;
    PressureTransducer left_coolant_manifold;
    PressureTransducer right_coolant_manifold;
    Adafruit_ADS1115 ads2;
    Adafruit_ADS1115 ads3;
    Adafruit_ADS1115 ads4;
    WheelEncoder encoder;

    // Futures for async tasks
    std::future<void> orientation;
    std::future<void> height;
    std::future<void> pressure_downstream;
    std::future<void> pressure_upstream;
    std::future<void> pressure_left_coolant;
    std::future<void> pressure_right_coolant;
    std::future<void> adsFut2;
    std::future<void> adsFut3;
    std::future<void> adsFut4;
    std::future<void> movement;

    InitializeAllSensors()
        : gyro(0x68, false),
          pod_height(vl6180_initialise(1)),
          pneumatic_downstream(0.1f, 3.2f, 0x40, Reference::downstream()),
          pneumatic_upstream(0.1f, 3.2f, 0x41, Reference::upstream()),
          left_coolant_manifold(0.1f, 3.2f, 0x44, Reference::left_Manifold()),
          right_coolant_manifold(0.1f, 3.2f, 0x45, Reference::right_Manifold()),
          ads2(0x49),
          ads3(0x4A),
          ads4(0x4B),
          encoder(WHEEL_ENCODER_A_PIN, WHEEL_ENCODER_B_PIN)
    {
        // Launch async threads
        orientation = std::async(std::launch::async, readGyro, &gyro);
        height = std::async(std::launch::async, readPodHeight, &pod_height);
        pressure_downstream = std::async(std::launch::async, readPneumaticPressure, &pneumatic_downstream);
        pressure_upstream = std::async(std::launch::async, readPneumaticPressure, &pneumatic_upstream);
        pressure_left_coolant = std::async(std::launch::async, readCoolantPressure, &left_coolant_manifold);
        pressure_right_coolant = std::async(std::launch::async, readCoolantPressure, &right_coolant_manifold);
        adsFut2 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads2);
        adsFut3 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads3);
        adsFut4 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads4);
        movement = std::async(std::launch::async, readEncoder, &encoder);
    }

    // Optional: function to wait for all threads
    void waitForAll()
    {
        orientation.wait();
        height.wait();
        pressure_downstream.wait();
        pressure_upstream.wait();
        pressure_left_coolant.wait();
        pressure_right_coolant.wait();
        adsFut2.wait();
        adsFut3.wait();
        adsFut4.wait();
        movement.wait();
    }
};
