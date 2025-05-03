#include <iostream>
#include <unistd.h> // for usleep
#include "ina219.h"
#include "pressure_transducer.cpp"


int main()
{
    float SHUNT_OHMS = 0.1f;
    float MAX_EXPECTED_AMPS = 3.2f;

    PressureTransducer upstream(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x40, Reference::upstream());
    // PressureTransducer downstream(SHUNT_OHMS, MAX_EXPECTED_AMPS, 0x41, Reference::downstream());

    std::cout << "time_s,upstream_pressure_PSI" << std::endl;
    // std::cout << "time_s,upstream_pressure_PSI,downstream_pressure_PSI" << std::endl;

    int c = 0;
    int startFlag = 0;
    float previous_pressure1, previous_pressure2, previous_pressure3 = 0;
    while (c <= 10000)
    {
        float current_pressure = upstream.read_pressure();
        if (startFlag > 2){
            if (abs(current_pressure) < abs((previous_pressure1 + previous_pressure2 + previous_pressure3)/3)){
                std::cout << c << "," << current_pressure << std::endl; // ","
                //   << downstream.read_pressure() << std::endl;
            // c++;
            usleep(1000000); // 1 second delay
            }
        }
        previous_pressure1 = previous_pressure2;
        previous_pressure2 = previous_pressure3;
        previous_pressure3 = current_pressure;
        // std::cout << c << "Reading all previous pressures: " << previous_pressure1 << ", " << previous_pressure2 << ", " << previous_pressure3 << std::endl; // ","
        startFlag++;
        // std::cout << startFlag << std::endl;
        c++;
    }

    return 0;
}