#include <iostream>
#include "Adafruit_ADS1015.h" // This will use ADS1115 by default
#include <thread>

// Create an instance of the ADS1115
Adafruit_ADS1115 ads(0x48); // Default I2C address

// Gain settings (change depending on your expected voltage range)
adsGain_t GAIN = GAIN_TWOTHIRDS; // ±6.144V

// Function to convert raw ADC to voltage based on the gain setting
float rawToVoltage(int16_t raw, adsGain_t gain) {
    float multiplier = 0.0;

    switch (gain) {
        case GAIN_TWOTHIRDS: multiplier = 0.1875F; break; // ±6.144V
        case GAIN_ONE:       multiplier = 0.125F;  break; // ±4.096V
        case GAIN_TWO:       multiplier = 0.0625F; break; // ±2.048V
        case GAIN_FOUR:      multiplier = 0.03125F;break; // ±1.024V
        case GAIN_EIGHT:     multiplier = 0.015625F;break;// ±0.512V
        case GAIN_SIXTEEN:   multiplier = 0.0078125F;break;// ±0.256V
    }

    return raw * multiplier / 1000.0f; // Convert mV to V
}

// Reads differential voltage between channels 0 and 1 or 2 and 3
float readDifferentialVoltage(uint8_t ainp, uint8_t ainm) {
    int16_t raw;

    if (ainp == 0 && ainm == 1) {
        raw = ads.readADC_Differential_0_1();
    } else if (ainp == 2 && ainm == 3) {
        raw = ads.readADC_Differential_2_3();
    } else {
        std::cerr << "Unsupported differential pair. Use (0,1) or (2,3)." << std::endl;
        return NAN;
    }

    return rawToVoltage(raw, GAIN);
}

/// @returns current in amperes given voltage (V) and resistance (Ω)
float currentFromVoltageAndResistance(float voltage, float resistance) {
    // I = V / R
    return voltage / resistance;
}

int main() {
    ads.setGain(GAIN);
    ads.begin();

    while (true) {
        float voltage = readDifferentialVoltage(0, 1);
        std::cout << "Differential voltage (A0 - A1): " << voltage << " V" << std::endl;
        delay(1000); // 1 second delay
    }

    return 0;
}


  