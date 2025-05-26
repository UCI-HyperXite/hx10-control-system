#include <iostream>
#include "../../../include/components/c/Adafruit_ADS1015.h" // This will use ADS1115 by default
#include <thread>
#define ADS_I2C_ADDRESS 0x4B

// Create an instance of the ADS1115
Adafruit_ADS1115 ads(ADS_I2C_ADDRESS); // Default I2C address

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
	std::cout << "Differential voltage (A0 - A1):" << std::endl;
        raw = ads.readADC_Differential_0_1();
    } else if (ainp == 2 && ainm == 3) {
	std::cout << "Differential voltage (A2 - A3):" << std::endl;
        raw = ads.readADC_Differential_2_3();
    } else {
        std::cerr << "Unsupported differential pair. Use (0,1) or (2,3)." << std::endl;
        return -1;
    }

    return rawToVoltage(raw, GAIN);
}

// Reads single ended voltage 
float readSingleVoltage(uint8_t channel){
    if (channel < 0 || channel > 3){
	std::cerr << "Invalid channel. Must be 0-3." << std::endl;
    }

    int16_t raw = ads.readADC_SingleEnded(channel);
    return rawToVoltage(raw, GAIN); 
}

// resistance (Ω) given voltage and current
float resistatanceFromVoltageAndCurrent(float voltage, float current){
    //calculate resistance 
    return 0.0;
}

int main() {
    ads.setGain(GAIN);
    ads.begin();

    while (true) {
        float voltage = readDifferentialVoltage(2, 3);
	std::cout << "Output voltage: " << voltage << " V" << std::endl;

	//float v0 = readSingleVoltage(0);
	//float v1 = readSingleVoltage(1);
	//float v2 = readSingleVoltage(2);
	//float v3 = readSingleVoltage(3);

	//std::cout << "A0 Voltage: " << v0 << " V" << std::endl;
	//std::cout << "A1 Voltage: " << v1 << " V" << std::endl;
	//std::cout << "A2 Voltage: " << v2 << " V" << std::endl;
	//std::cout << "A3 Voltage: " << v3 << " V" << std::endl;
	

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }

    return 0;
}