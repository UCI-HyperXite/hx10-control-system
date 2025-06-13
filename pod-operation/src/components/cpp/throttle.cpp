
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
#include <atomic>
#include "components/c/brakes.h"
#include "components/c/high_voltage_system.h"
extern "C" {
    #include "gpio.h"    
}

std::atomic<bool> encoderRunning(false);
std::atomic<bool> shutdownRequested(false);

void monitorUserInput() {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) {
        std::cout << "\nBlank input received. Requesting shutdown...\n";
        shutdownRequested = true;
    }
}


void encoderSimThread() {
    const int pinA = 23;  // Encoder A
    const int pinB = 24;  // Encoder B

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);

    bool state = false;
    std::cout << "Encoder emulation running" << "\n";
    while (encoderRunning) {
        digitalWrite(pinA, state);
        std::this_thread::sleep_for(std::chrono::microseconds(2000)); // ~1kHz

        digitalWrite(pinB, state);
        std::this_thread::sleep_for(std::chrono::microseconds(2000));

        state = !state;
    }

    // Ensure both pins are LOW when done
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
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

void safeShutdown(MCP4725& dac) {
    std::cout << "[*] Performing safe shutdown...\n";
    dac.setThrottle(0);
    digitalWrite(17, LOW);
    openContactors();
    engageBrakes();
    encoderRunning = false;
}


int main() {
    wiringPiSetupGpio();         
    pinMode(17, OUTPUT);      
    MCP4725 dac;  // Uses default address 0x60 and default device "/dev/i2c-1"
    initializeBrakes();
    initializeHighVoltageSystem();
    std::cout << "Starting throttle and encoder simulation...\n";
    
    // Ramp up
    if (!dac.setThrottle(0)) {
        std::cerr << "Error: Failed to set value " << 0 << "\n";
    } else {
        std::cout << "Throttle set to: " << 0 << "\n";
    }
    std::thread inputThread(monitorUserInput);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (shutdownRequested) {
        safeShutdown(dac);
        goto cleanup;
    }
    disengageBrakes();
    openContactors();
    
    digitalWrite(17, HIGH);
    encoderRunning = true;
    std::thread encoderThread(encoderSimThread);
    std::cout << "Pin 17 written to 1";
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (shutdownRequested) {
        safeShutdown(dac);
        goto cleanup;
    }
    
    for (int value = 0; value <= (MCP4725::MAX_VALUE*0.5); value += 50) {
        if (shutdownRequested) break; 
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value " << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Ramp down
    for (int value = MCP4725::MAX_VALUE*0.5; value >= 0; value -= 100) {
        if (shutdownRequested) break; 
        if (!dac.setThrottle(value)) {
            std::cerr << "Error: Failed to set value :" << value << "\n";
        } else {
            std::cout << "Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    safeShutdown(dac);
    encoderRunning = false;
    if (encoderThread.joinable()) encoderThread.join();
    if (inputThread.joinable()) inputThread.join();
    std::cout << "Run complete.\n";
    return 0;
}
