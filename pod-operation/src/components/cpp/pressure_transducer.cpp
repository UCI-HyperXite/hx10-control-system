#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

#define INA219_REG_CALIBRATION 0x05
#define INA219_REG_CURRENT 0x04

constexpr int INA219_UPSTREAM_ADDRESS = 0x40;
constexpr int INA219_DOWNSTREAM_ADDRESS = 0x41;

constexpr uint16_t INA219_CALIBRATION_VALUE = 0x2000;
constexpr float INA219_SCALING_VALUE = 160.0f;

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

class INA219
{
private:
    int device;

public:
    explicit INA219(int device_address)
    {
        device = wiringPiI2CSetup(device_address);
        if (device == -1)
        {
            std::cerr << "Failed to initialize INA219 at address: " << device_address << std::endl;
            exit(1);
        }
        calibrate();
    }

    void calibrate()
    {
        wiringPiI2CWriteReg16(device, INA219_REG_CALIBRATION, INA219_CALIBRATION_VALUE);
    }

    float readCurrent()
    {
        int raw = wiringPiI2CReadReg16(device, INA219_REG_CURRENT);
        return static_cast<float>(raw) / INA219_SCALING_VALUE;
    }
};

class PressureTransducer
{
private:
    INA219 ina;
    Reference ref_values;

public:
    explicit PressureTransducer(int address, Reference ref) : ina(address), ref_values(ref) {}

    static PressureTransducer upstream() { return PressureTransducer(INA219_UPSTREAM_ADDRESS, Reference::upstream()); }
    static PressureTransducer downstream() { return PressureTransducer(INA219_DOWNSTREAM_ADDRESS, Reference::downstream()); }

    float readPressure()
    {
        float current = ina.readCurrent();
        return ref_values.pressure_lo + ref_values.pressure_span * (current - ref_values.current_lo) / ref_values.current_span;
    }
};
