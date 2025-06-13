#include <iostream>
#include <chrono>
#include <cmath>
#include <memory>
#include <optional>
#include <cstdint>
#include <wiringPi.h>
#include <unistd.h> 
#include <atomic>
#include "../../../include/utils/gpio.h"
#include "../../../include/components/cpp/wheel_encoder.hpp"
#include <stdexcept>
#include <csignal>

WheelEncoder* WheelEncoder::instance = nullptr;


EncoderDiff operator-(EncoderState current, EncoderState previous) {
    // The quadrature states are ordered A -> B -> C -> D -> A for forward rotation.
    int8_t diff = (static_cast<int8_t>(current) - static_cast<int8_t>(previous) + 5) % 4 - 1;
    switch (diff) {
        case -1: return EncoderDiff::Backwards;
        case 0:  return EncoderDiff::Stationary;
        case 1:  return EncoderDiff::Forwards;
        case 2:  return EncoderDiff::Undersampling; // A jump of 2 indicates a missed state
        default: return EncoderDiff::Unknown;
    }
}

int8_t to_int(EncoderDiff diff) {
    return static_cast<int8_t>(diff);
}

float to_float(EncoderDiff diff) {
    return static_cast<float>(to_int(diff));
}

EncoderState encode_state(Level a, Level b) {
    int8_t sa = static_cast<int8_t>(a);
    int8_t sb = static_cast<int8_t>(b);
    int8_t code = (sa << 1) + (sa ^ sb);

    switch (code) {
        case 0b00: return EncoderState::A;
        case 0b01: return EncoderState::B;
        case 0b11: return EncoderState::C;
        case 0b10: return EncoderState::D;
        default:   return EncoderState::Unknown;
    }
}

Level read_level(int pin) {
    return digitalRead(pin) == HIGH ? Level::High : Level::Low;
}


WheelEncoder::WheelEncoder(int a, int b) : pin_a(a), pin_b(b) {
    if (instance != nullptr) {
        throw std::runtime_error("WheelEncoder instance already exists. This class is a singleton.");
    }

    // Set pin modes to input and enable pull-down resistors
    pinMode(pin_a, INPUT);
    pullUpDnControl(pin_a, PUD_DOWN);
    pinMode(pin_b, INPUT);
    pullUpDnControl(pin_b, PUD_DOWN);

    last_state = read_state();
    last_time = std::chrono::steady_ck::now();

    instance = this;

    if (wiringPiISR(pin_a, INT_EDGE_BOTH, &WheelEncoder::isr_a) < 0) {
        throw std::runtime_error("Failed to set up ISR for encoder pin A.");
    }
    if (wiringPiISR(pin_b, INT_EDGE_BOTH, &WheelEncoder::isr_b) < 0) {
        throw std::runtime_error("Failed to set up ISR for encoder pin B.");
    }
    std::cout << "WheelEncoder initialized on pins " << pin_a << " and " << pin_b << std::endl;
}

WheelEncoder::~WheelEncoder() {
    instance = nullptr;
    std::cout << "WheelEncoder destroyed." << std::endl;
}


float WheelEncoder::get_distance() const {
    return static_cast<float>(counter.load()) * DISTANCE_PER_COUNT;
}

float WheelEncoder::get_velocity() const {
    return velocity.load();
}

EncoderState WheelEncoder::read_state() const {
    return encode_state(read_level(pin_a), read_level(pin_b));
}

void WheelEncoder::handle_interrupt() {
    auto current_time = std::chrono::steady_clock::now();
    EncoderState current_state = read_state();
    
    EncoderDiff inc = current_state - last_state;

    if (inc == EncoderDiff::Undersampling || inc == EncoderDiff::Unknown) {
        last_state = current_state;
        return;
    }

    std::chrono::duration<float> dt = current_time - last_time;
    if (inc != EncoderDiff::Stationary && dt.count() > 0.00001f) { // 10 microsec debounce
        float vel = (DISTANCE_PER_COUNT * to_float(inc)) / dt.count();
        
        velocity.store(vel);
        last_time = current_time;
    }
    
    counter.fetch_add(to_int(inc));
    last_state = current_state;
}

void WheelEncoder::isr_a() {
    if (instance) {
        instance->handle_interrupt();
    }
}

void WheelEncoder::isr_b() {
    if (instance) {
        instance->handle_interrupt();
    }
}