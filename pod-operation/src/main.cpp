#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <future>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <iomanip>
#include <csignal>


#include "../include/components/cpp/wheel_encoder.hpp"
#include "../include/components/cpp/can_decoder.hpp"

extern "C" {
    #include "../include/components/c/high_voltage_system.h"
    #include "../include/utils/gpio.h"
}


std::atomic<bool> g_running(true);
std::atomic<bool> g_encoder_sim_running(false);



void signal_handler(int signum) {
    std::cout << "\nCaught signal " << signum << ". Shutting down..." << std::endl;
    g_running = false;
}



class MCP4725 {
public:
    static const int MAX_VALUE = 4095;

    MCP4725(int address = 0x60, const char* i2cDevice = "/dev/i2c-1")
        : i2c_fd(-1), i2c_addr(address), i2c_path(i2cDevice) {}

    ~MCP4725() {
        closeBus();
    }

    bool setThrottle(int value) {
        if (value < 0) value = 0;
        if (value > MAX_VALUE) value = MAX_VALUE;

        if (!openBus()) return false;

        uint8_t buffer[3];
        buffer[0] = 0x40; 
        buffer[1] = (value >> 4) & 0xFF;
        buffer[2] = (value & 0x0F) << 4;

        bool success = (write(i2c_fd, buffer, 3) == 3);
        if (!success) {
            perror("Failed to write to DAC");
        }

        closeBus();
        return success;
    }

private:
    int i2c_fd;
    int i2c_addr;
    const char* i2c_path;

    bool openBus() {
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

    void closeBus() {
        if (i2c_fd >= 0) {
            close(i2c_fd);
            i2c_fd = -1;
        }
    }
};




void encoder_simulation_task() {
    const int pinA = 23; 
    const int pinB = 24; 

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);

    bool state = false;
    std::cout << "[Encoder Sim] Emulation thread started." << std::endl;
    while (g_encoder_sim_running) {
        digitalWrite(pinA, state);
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        digitalWrite(pinB, state);
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        state = !state;
    }
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    std::cout << "[Encoder Sim] Emulation thread finished." << std::endl;
}


void throttle_simulation_task() {
    MCP4725 dac;
    pinMode(17, OUTPUT); 

    std::cout << "[Throttle Sim] Starting throttle and encoder simulation...\n";
    
    
    g_encoder_sim_running = true;
    std::thread encoder_thread(encoder_simulation_task);
    
    digitalWrite(17, HIGH);
    std::cout << "[Throttle Sim] Motor Enable Pin (17) set to HIGH.\n";
    
    
    for (int value = 0; value <= (MCP4725::MAX_VALUE * 0.5) && g_running; value += 50) {
        if (!dac.setThrottle(value)) {
            std::cerr << "[Throttle Sim] Error: Failed to set throttle to " << value << "\n";
        } else {
            std::cout << "[Throttle Sim] Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    
    for (int value = (MCP4725::MAX_VALUE * 0.5); value >= 0 && g_running; value -= 100) {
        if (!dac.setThrottle(value)) {
            std::cerr << "[Throttle Sim] Error: Failed to set throttle to " << value << "\n";
        } else {
            std::cout << "[Throttle Sim] Throttle set to: " << value << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    
    g_encoder_sim_running = false;
    encoder_thread.join();
    digitalWrite(17, LOW);
    dac.setThrottle(0);
    std::cout << "[Throttle Sim] Simulation complete. Motor Enable Pin (17) set to LOW.\n";
}


void wheel_encoder_reader_task() {
    try {
        WheelEncoder encoder(23, 24); 
        std::cout << "[Encoder Reader] Started. Reading from physical encoder." << std::endl;
        while (g_running) {
            printf("\r[Encoder Reader] Distance: %.3f m | Velocity: %.3f m/s", 
                encoder.get_distance(), encoder.get_velocity());
            fflush(stdout);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "\n[Encoder Reader] Error: " << e.what() << std::endl;
    }
    std::cout << "\n[Encoder Reader] Task finished." << std::endl;
}


const char* direction_to_string(DrivingDirection dir) {
    switch(dir) {
        case DrivingDirection::Neutral: return "Neutral";
        case DrivingDirection::Forward: return "Forward";
        case DrivingDirection::Reverse: return "Reverse";
        default: return "Unknown";
    }
}

void print_decoded_message(uint32_t id, const DecodedMessage& msg) {
    std::cout << "----------------------------------------\n";
    printf("[CAN Decoder] Received CAN ID: 0x%X\n", id);

    if (id == 0x10F8109A) {
        const auto& state = msg.state;
        printf("  Type: Controller State\n  Direction: %s\n  Motor RPM: %d\n  Error Code: %d\n",
            direction_to_string(state.direction), state.motor_rpm, state.error_code);
    } else if (id == 0x10F8108D) {
        const auto& telemetry = msg.telemetry;
        printf("  Type: Controller Telemetry\n  Battery Voltage: %.1f V\n  Motor Current: %.1f A\n  Motor Temp: %.1f C\n  Controller Temp: %.1f C\n",
            telemetry.battery_voltage, telemetry.motor_current, telemetry.motor_temp_c, telemetry.controller_temp_c);
    }
    std::cout << "----------------------------------------\n\n";
}


void can_decoder_task() {
    CANDecoder decoder;
    std::string line;
    std::cout << "[CAN Decoder] Started. Waiting for candump input via stdin...\n";

    while (g_running && std::getline(std::cin, line)) {
        if (!g_running) break;
        auto result = decoder.decode(line);
        if (result) {
            print_decoded_message(result->first, result->second);
        }
    }
    std::cout << "[CAN Decoder] Task finished." << std::endl;
}


int main(int argc, char *argv[]) {
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    
    if (wiringPiSetupGpio() < 0) {
        std::cerr << "Fatal: Failed to initialize wiringPi" << std::endl;
        return 1;
    }
    std::cout << "GPIO Initialized." << std::endl;

    
    
    std::future<void> throttle_sim_future;
    std::future<void> encoder_reader_future;
    std::future<void> can_decoder_future;

    
    if (argc > 1) {
        std::string mode = argv[1];
        if (mode == "throttle_sim") {
            std::cout << "Mode: Running Throttle & Encoder Simulation." << std::endl;
            throttle_sim_future = std::async(std::launch::async, throttle_simulation_task);
        } else if (mode == "encoder_read") {
            std::cout << "Mode: Running Physical Encoder Reader." << std::endl;
            encoder_reader_future = std::async(std::launch::async, wheel_encoder_reader_task);
        } else if (mode == "can_decode") {
            std::cout << "Mode: Running CAN Decoder." << std::endl;
            can_decoder_future = std::async(std::launch::async, can_decoder_task);
        } else {
            std::cerr << "Invalid mode specified. Use 'throttle_sim', 'encoder_read', or 'can_decode'." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "No mode specified. Please run with an argument: 'throttle_sim', 'encoder_read', or 'can_decode'." << std::endl;
        return 1;
    }
    
    
    while (g_running) {
        
        if (throttle_sim_future.valid() && throttle_sim_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) break;
        if (encoder_reader_future.valid() && encoder_reader_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) break;
        if (can_decoder_future.valid() && can_decoder_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) break;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    g_running = false; 
    g_encoder_sim_running = false; 

    
    if (throttle_sim_future.valid()) throttle_sim_future.get();
    if (encoder_reader_future.valid()) encoder_reader_future.get();
    if (can_decoder_future.valid()) {

    }

    std::cout << "Main function finished." << std::endl;
    return 0;
}
