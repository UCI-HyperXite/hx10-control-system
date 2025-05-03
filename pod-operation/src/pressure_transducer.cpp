#include <iostream>
#include <unistd.h> // for usleep
#include "ina219.h"

// This struct helps convert current (mA) to pressure (PSI)
struct Reference
{
    float pressure_lo;
    float pressure_span;
    float current_lo;
    float current_span;

    Reference(float plo, float phi, float clo, float chi)
        : pressure_lo(plo), pressure_span(phi - plo),
          current_lo(clo), current_span(chi - clo) {}

    static Reference upstream() { return Reference(0.0f, 5000.0f, 4.0f, 20.0f); }
    // static Reference downstream() { return Reference(0.0f, 300.0f, 4.0f, 20.0f); }
};

// This class ties together INA219 + Reference
class PressureTransducer
{
public:
    INA219 sensor;
    Reference ref;
    static constexpr float scaling_value = 160.0f; // scaling from datasheet

    PressureTransducer(float shunt_ohms, float max_expected_amps, uint8_t address, Reference ref_values)
        : sensor(shunt_ohms, max_expected_amps, address), ref(ref_values)
    {
        sensor.configure(RANGE_16V, GAIN_8_320MV, ADC_12BIT, ADC_12BIT);
        sensor.set_calibration(0xFFFF); // manually set calibration
    }

    float read_current()
    {
        return sensor.current() / scaling_value;
    }

    float read_pressure()
    {
        float current_mA = read_current();
        // return ref.pressure_lo + ref.pressure_span * (current_mA - ref.current_lo) / ref.current_span;
        return read_current() * 339.60933 - 1513.72096;
    }
};

