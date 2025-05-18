#include "MCP4725.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdio>

MCP4725::MCP4725(int address, const char* i2cDevice)
    : i2c_fd(-1), i2c_addr(address), i2c_path(i2cDevice) {}

MCP4725::~MCP4725() {
    closeBus();
}

bool MCP4725::openBus() {
    i2c_fd = open(i2c_path, O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C bus");
        return false;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, i2c_addr) < 0) {
        perror("Failed to set I2C address");
        close(i2c_fd);
        i2c_fd = -1;
        return false;
    }
    return true;
}

void MCP4725::closeBus() {
    if (i2c_fd >= 0) {
        close(i2c_fd);
        i2c_fd = -1;
    }
}

bool MCP4725::setThrottle(int value) {
    if (value < 0) value = 0;
    if (value > MAX_VALUE) value = MAX_VALUE;

    if (!openBus()) return false;

    uint8_t buffer[3];
    buffer[0] = 0x40; // Write command to DAC register
    buffer[1] = (value >> 4) & 0xFF;
    buffer[2] = (value & 0x0F) << 4;

    bool success = (write(i2c_fd, buffer, 3) == 3);
    if (!success) {
        perror("Failed to write to DAC");
    }

    closeBus();
    return success;
}


int main() {
    MCP4725 dac;  // Uses default address 0x60 and default device "/dev/i2c-1"

    std::cout << "Testing MCP4725 throttle output...\n";

    // Ramp up
    for (int value = 0; value <= MCP4725::MAX_VALUE; value += 512) {
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value " << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // Ramp down
    for (int value = MCP4725::MAX_VALUE; value >= 0; value -= 512) {
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value " << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "DAC test complete.\n";
    return 0;
}
