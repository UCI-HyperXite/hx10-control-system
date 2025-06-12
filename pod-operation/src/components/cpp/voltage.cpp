#include <iostream>
#include "../../../include/components/c/Adafruit_ADS1015.h" 
#include <thread>
#include <cmath>
#include <future>
#include "../../../include/components/cpp/voltage.hpp"  


// resistance (Ω) given voltage and current
float getThermistorResistance(float v_out, float v_in) {
    float R_fixed = 10000.0f;  // R1
    if (v_out <= 0.0f || v_out >= v_in) return -1.0f;  // Avoid division by 0 or invalid ratios
    return (v_out * R_fixed) / (v_in - v_out);  // R2
}

// temperature (C) measured by thermistor given resistance (Ω) 
float getThermistorTemperator(float resistance){
    float beta_value = 3950.0f;     // material constant for NRL3104F3950B1F
    float nominal_resistance = 10000.0f;    // ohms (Ω)
    float reference_temp = 298.15f;     // Kelvin (K)

    float temp_kelvin = beta_value / (std::log(resistance / nominal_resistance) + (beta_value / reference_temp));
    float temp_celsius = temp_kelvin - 273.15f;

    return temp_celsius;

}