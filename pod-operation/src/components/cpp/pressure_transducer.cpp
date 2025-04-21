#include <iostream>
#include <unistd.h>
#include "src/ina219.h"

#define INA219_UPSTREAM_ADDRESS 0x40
#define INA219_DOWNSTREAM_ADDRESS 0x41

class Reference
{
public:
    float pressure_lo;
    float pressure_span;
    float current_lo;
    float current_span;

    Reference(float pressure_lo, float pressure_hi, float current_lo, float current_hi)
        : pressure_lo(pressure_lo), current_lo(current_lo),
          pressure_span(pressure_hi - pressure_lo), current_span(current_hi - current_lo) {}

    static Reference upstream() { return Reference(0.0f, 5000.0f, 4.0f, 20.0f); }
    static Reference downstream() { return Reference(0.0f, 300.0f, 4.0f, 20.0f); }
};

class PressureTransducer
{
private:
    INA219 ina;
    Reference ref_values;

public:
    PressureTransducer(uint8_t address, Reference ref)
        : ina(address), ref_values(ref)
    {
        ina.configure(RANGE_16V, GAIN_8_320MV, ADC_12BIT, ADC_12BIT);
    }

    static PressureTransducer upstream() { return PressureTransducer(INA219_UPSTREAM_ADDRESS, Reference::upstream()); }
    static PressureTransducer downstream() { return PressureTransducer(INA219_DOWNSTREAM_ADDRESS, Reference::downstream()); }

    float readPressure()
    {
        float current = ina.current(); // Use library’s current() method
        return ref_values.pressure_lo + ref_values.pressure_span * (current - ref_values.current_lo) / ref_values.current_span;
    }
};

int main()
{
    PressureTransducer upstream_sensor = PressureTransducer::upstream();
    PressureTransducer downstream_sensor = PressureTransducer::downstream();

    std::cout << "time_s,upstream_pressure,downstream_pressure" << std::endl;

    for (int t = 0; t < 5; ++t)
    {
        float up_p = upstream_sensor.readPressure();
        float down_p = downstream_sensor.readPressure();

        std::cout << t << "," << up_p << "," << down_p << std::endl;
        usleep(1000000);
    }

    return 0;
}
