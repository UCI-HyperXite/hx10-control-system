#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#include "../include/components.hpp"

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

// ads function for threading call
void readADS1015ThermistorLoop(Adafruit_ADS1115* ads) {
    ads->setGain(GAIN);
    ads->begin();

    while (true) {
        // float voltage = readDifferentialVoltage(*ads, 2, 3);

        // int16_t raw = ads->readADC_SingleEnded(0);  
        // float voltage = raw * 0.1875F / 1000.0f;
        // float resistance = getThermistorResistance(voltage);
        // float temperature = getThermistorTemperator(resistance);

        // std::cout << "[ADS1015] Voltage: " << voltage << " V, "
        //           << "Resistance: " << resistance << " Ω, "
        //           << "Temperature: " << temperature << " °C\n";

        for (int channel = 0; channel < 4; ++channel) {
            int16_t raw = ads->readADC_SingleEnded(channel);
            float voltage = raw * 0.1875F / 1000.0f;
            float resistance = getThermistorResistance(voltage);
            float temperature = getThermistorTemperator(resistance);

            std::cout << "[ADS1015] Channel: " << channel << ", "
                      << " Voltage: " << voltage << " V, " 
                      << "Resistance: " << resistance << " Ω, "
                      << "Temperature: " << temperature << " °C\n"
                      << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

// void readEncoder(WheelEncoder* encoder){
//     while (true) {
//         float distance = encoder->get_distance();
//         float velocity = encoder->get_velocity();

//         std::cout << "Distance: " << distance << " m, "
//                   << "Velocity: " << velocity << " m/s\n";

//         usleep(100000);
//     }
// }

