
#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <lgpio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "MCP4725.h"
#include <wiringPi.h>
extern "C" {
    #include "gpio.h"    
}


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
    wiringPiSetupGpio();         
    pinMode(17, OUTPUT);      
    MCP4725 dac;  // Uses default address 0x60 and default device "/dev/i2c-1"

    std::cout << "Testing MCP4725 throttle output...\n";
    
    // Ramp up
    if (!dac.setThrottle(0)) {
        std::cerr << "Error: Failed to set value " << 0 << "\n";
    } else {
        std::cout << "Throttle set to: " << 0 << "\n";
    }
    // while (true){
    //     digitalWrite(17, HIGH);
    //     std::cout << "1" << "\n";
    //     //std::this_thread::sleep_for(std::chrono::seconds(1));
    //     sleep(1);
    //     digitalWrite(17, LOW);
    //     std::cout << "0" << "\n";
    //     sleep(1);
    // }
    digitalWrite(17, HIGH);
    std::cout << "Pin 17 written to 1";
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    sleep(30);
    
    for (int value = 0; value <= (MCP4725::MAX_VALUE*0.5); value += 50) {
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value " << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Ramp down
    for (int value = MCP4725::MAX_VALUE*0.5; value >= 0; value -= 100) {
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value :" << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    digitalWrite(17, LOW);
    std::cout << "DAC test complete.\n";
    return 0;
}
