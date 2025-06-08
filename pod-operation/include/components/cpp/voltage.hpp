#ifndef ADS1015_UTILS_HPP
#define ADS1015_UTILS_HPP

#include <cstdint>
#include "../c/Adafruit_ADS1015.h"

// Default gain setting (±6.144V)
// Defined in the corresponding .cpp file as:
//     adsGain_t GAIN = GAIN_TWOTHIRDS;
// extern adsGain_t GAIN;

// /**
//  * @brief  Convert a raw ADC reading to voltage (in volts), based on the ADS gain setting.
//  * @param  raw:    The raw 16-bit reading from the ADS1115.
//  * @param  gain:   The gain setting (adsGain_t) used for conversion.
//  * @return Voltage in volts.
//  */
// float rawToVoltage(int16_t raw, adsGain_t gain);

/**
 * @brief  Read a differential voltage between two ADC channels (0–1 or 2–3).
 * @param  ads:   Reference to an initialized Adafruit_ADS1115 instance.
 * @param  ainp:  Positive input channel (must be 0 or 2).
 * @param  ainm:  Negative input channel (must be 1 or 3).
 * @return Differential voltage in volts, or -1 on invalid channel pairing.
 */
float readDifferentialVoltage(Adafruit_ADS1115& ads, uint8_t ainp, uint8_t ainm);

/**
 * @brief  Read a single‐ended voltage on a given ADC channel (0–3).
 * @param  ads:      Reference to an initialized Adafruit_ADS1115 instance.
 * @param  channel:  ADC channel index (0–3).
 * @return Voltage in volts, or -1 on invalid channel.
 */
float readSingleVoltage(Adafruit_ADS1115& ads, uint8_t channel);

/**
 * @brief  Compute the thermistor resistance given the measured output voltage.
 * @param  v_out:  Voltage measured across the thermistor (in volts).
 * @param  v_in:   Supply/reference voltage (default: 5.0V).
 * @return Thermistor resistance in ohms, or -1 on invalid ratio.
 */
float getThermistorResistance(float v_out, float v_in = 5.0f);

/**
 * @brief  Convert thermistor resistance (Ω) to temperature (°C) using the Beta parameter method.
 * @param  resistance:  Thermistor resistance in ohms.
 * @return Temperature in degrees Celsius.
 */
float getThermistorTemperator(float resistance);

/**
 * @brief  Continuously read thermistor data on channels A2–A3 in a loop,
 *         converting differential voltage → resistance → temperature every second.
 * @param  ads:  Pointer to an initialized Adafruit_ADS1115 instance.
 * 
 * @note   This function calls ads->begin() and ads->setGain(GAIN) internally.
 *         Prints voltage, resistance, and temperature to std::cout once per second.
 */
void readADS1015ThermistorLoop(Adafruit_ADS1115* ads);

#endif // ADS1015_UTILS_HPP