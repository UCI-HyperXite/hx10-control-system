#include <iostream>
#include "../../../include/components/c/Adafruit_ADS1015.h" 
#include <thread>
#include <cmath>
#include <future>

// #define ADS_I2C_ADDRESS 0x4B

// Create an instance of the ADS1115
// Adafruit_ADS1115 ads(ADS_I2C_ADDRESS); // Default I2C address

// Gain settings (change depending on your expected voltage range)
// adsGain_t GAIN = GAIN_TWOTHIRDS; // ±6.144V

// float rawToVoltage(int16_t raw, adsGain_t gain);
// float readDifferentialVoltage(Adafruit_ADS1115& ads, uint8_t ainp, uint8_t ainm);
// float readSingleVoltage(Adafruit_ADS1115& ads, uint8_t channel);
float getThermistorResistance(float v_out, float v_in);
float getThermistorTemperator(float resistance);

// // Function to convert raw ADC to voltage based on the gain setting
// float rawToVoltage(int16_t raw, adsGain_t gain) {
//     float multiplier = 0.0;

//     switch (gain) {
//         case GAIN_TWOTHIRDS: multiplier = 0.1875F; break; // ±6.144V
//         case GAIN_ONE:       multiplier = 0.125F;  break; // ±4.096V
//         case GAIN_TWO:       multiplier = 0.0625F; break; // ±2.048V
//         case GAIN_FOUR:      multiplier = 0.03125F;break; // ±1.024V
//         case GAIN_EIGHT:     multiplier = 0.015625F;break;// ±0.512V
//         case GAIN_SIXTEEN:   multiplier = 0.0078125F;break;// ±0.256V
//     }

//     return raw * multiplier / 1000.0f; // Convert mV to V
// }

// // Reads differential voltage between channels 0 and 1 or 2 and 3
// float readDifferentialVoltage(Adafruit_ADS1115& ads, uint8_t ainp, uint8_t ainm) {
//     int16_t raw;

//     if (ainp == 0 && ainm == 1) {
// 	std::cout << "Differential voltage (A0 - A1):" << std::endl;
//         raw = ads.readADC_Differential_0_1();
//     } else if (ainp == 2 && ainm == 3) {
// 	std::cout << "Differential voltage (A2 - A3):" << std::endl;
//         raw = ads.readADC_Differential_2_3();
//     } else {
//         std::cerr << "Unsupported differential pair. Use (0,1) or (2,3)." << std::endl;
//         return -1;
//     }

//     return rawToVoltage(raw, GAIN);
// }

// // Reads single ended voltage 
// float readSingleVoltage(Adafruit_ADS1115& ads, uint8_t channel){
//     if (channel < 0 || channel > 3){
// 	std::cerr << "Invalid channel. Must be 0-3." << std::endl;
//     }

//     int16_t raw = ads.readADC_SingleEnded(channel);
//     return rawToVoltage(raw, GAIN); 
// }

// resistance (Ω) given voltage and current
float getThermistorResistance(float v_out, float v_in = 5.0f) {
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