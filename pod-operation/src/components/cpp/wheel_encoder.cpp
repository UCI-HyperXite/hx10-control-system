#include <iostream>
#include <chrono>
#include <cmath>
#include <memory>
#include <optional>
#include <cstdint>
#include <wiringPi.h>
#include <unistd.h> 
#include <atomic>

constexpr float WHEEL_DIAMETER = 0.0762f;
constexpr float ENCODER_RESOLUTION = 16.0f;
constexpr float DISTANCE_PER_COUNT = WHEEL_DIAMETER * static_cast<float>(M_PI) / ENCODER_RESOLUTION;

constexpr int WHEEL_ENCODER_A_PIN = 0;
constexpr int WHEEL_ENCODER_B_PIN = 1;

enum class Level : int8_t {
    Low = 0,
    High = 1
};

enum class EncoderState : int8_t {
    A = 0b00,
    B = 0b01,
    C = 0b11,
    D = 0b10,
    Unknown = -1
};

enum class EncoderDiff : int8_t {
    Backwards = -1,
    Stationary = 0,
    Forwards = 1,
    Undersampling = 2,
    Unknown = 3
};

EncoderDiff operator-(EncoderState current, EncoderState previous) {
    int8_t diff = (static_cast<int8_t>(current) - static_cast<int8_t>(previous) + 5) % 4 - 1;
    switch (diff) {
        case -1: return EncoderDiff::Backwards;
        case 0:  return EncoderDiff::Stationary;
        case 1:  return EncoderDiff::Forwards;
        case 2:  return EncoderDiff::Undersampling;
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

class WheelEncoder {
private:
    int pin_a;
    int pin_b;

    std::atomic<int16_t> counter{0};
    std::atomic<float> velocity{0.0f};

    EncoderState last_state = EncoderState::Unknown;
    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

    static WheelEncoder* instance;

public:
    WheelEncoder(int a, int b) : pin_a(a), pin_b(b) {
        wiringPiSetup();
        pinMode(pin_a, INPUT);
        pullUpDnControl(pin_a, PUD_DOWN);
        pinMode(pin_b, INPUT);
        pullUpDnControl(pin_b, PUD_DOWN);

        last_state = read_state();

        instance = this;
        wiringPiISR(pin_a, INT_EDGE_BOTH, &WheelEncoder::isr_a);
        wiringPiISR(pin_b, INT_EDGE_BOTH, &WheelEncoder::isr_b);
    }

    float get_distance() const {
        return static_cast<float>(counter.load()) * DISTANCE_PER_COUNT;
    }

    float get_velocity() const {
        return velocity.load();
    }

private:
    EncoderState read_state() const {
        return encode_state(read_level(pin_a), read_level(pin_b));
    }

    void handle_interrupt() {
        auto state = read_state();
        EncoderDiff inc = state - last_state;

        if (inc == EncoderDiff::Undersampling || inc == EncoderDiff::Unknown) {
            return;
        }

        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = now - last_time;

        if (inc != EncoderDiff::Stationary && dt.count() > 0.00001f) {
            float vel = DISTANCE_PER_COUNT * to_float(inc) / dt.count();

            // Decay logic
            if (vel * dt.count() > DISTANCE_PER_COUNT) {
                vel = DISTANCE_PER_COUNT * std::copysign(1.0f, vel) / dt.count();
            }

            velocity.store(vel);
            last_time = now;
        }

        counter.fetch_add(to_int(inc));
        last_state = state;
    }

    static void isr_a() { if (instance) instance->handle_interrupt(); }
    static void isr_b() { if (instance) instance->handle_interrupt(); }
};

WheelEncoder* WheelEncoder::instance = nullptr;

int main() {
    WheelEncoder encoder(WHEEL_ENCODER_A_PIN, WHEEL_ENCODER_B_PIN);

    while (true) {
        std::cout << "Distance: " << encoder.get_distance() << " m, ";
        usleep(100000); 
        delay(100); 
    }

    return 0;
}