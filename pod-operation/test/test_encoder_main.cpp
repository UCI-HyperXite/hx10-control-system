#include "../include/components/cpp/wheel_encoder.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    running = 0;
}

int main() {
    
    signal(SIGINT, signal_handler);

    if (wiringPiSetupGpio() < 0) {
        std::cerr << "Error: Failed to initialize wiringPi" << std::endl;
        return 1;
    }

    
    const int ENCODER_PIN_A = 23;
    const int ENCODER_PIN_B = 24;

    try {
        
        
        WheelEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

        std::cout << "Reading from encoder. Press Ctrl+C to exit." << std::endl;

        
        while (running) {
            
            float distance = encoder.get_distance();
            float velocity = encoder.get_velocity();

            printf("\rDistance: %.3f m  |  Velocity: %.3f m/s     ", distance, velocity);
            fflush(stdout);

            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    
    std::cout << "\nShutting down." << std::endl;

    return 0;
}