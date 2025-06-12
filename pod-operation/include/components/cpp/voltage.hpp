#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

float getThermistorResistance(float v_out, float v_in = 5.0f);
float getThermistorTemperator(float resistance);

#endif // VOLTAGE_HPP
