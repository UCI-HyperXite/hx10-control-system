#ifndef MCP4725_H
#define MCP4725_H

#include <cstdint>

class MCP4725
{
public:
    static const int MAX_VALUE = 4095;

    MCP4725(int address = 0x62, const char *i2cDevice = "/dev/i2c-1");
    ~MCP4725();

    bool setThrottle(int value); // Value: 0–4095 = 0–5V

private:
    int i2c_fd;
    int i2c_addr;
    const char *i2c_path;

    bool openBus();
    void closeBus();
};

#endif // MCP4725_H