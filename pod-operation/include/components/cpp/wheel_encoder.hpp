#pragma once

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <wiringPi.h>

// WHEEL_DIAMETER is in meters
constexpr float WHEEL_DIAMETER = 0.0762f;
constexpr float ENCODER_RESOLUTION = 16.0f; // pulse/rev
constexpr float DISTANCE_PER_COUNT = (WHEEL_DIAMETER * static_cast<float>(M_PI)) / ENCODER_RESOLUTION;

// HIGH or LOW state of a GPIO pin
enum class Level : int8_t {
    Low = 0,
    High = 1
};

// 4 possible states of a quadrature encoder (gray code)
enum class EncoderState : int8_t {
    A = 0b00,
    B = 0b01,
    C = 0b11,
    D = 0b10,
    Unknown = -1
};

// Change between two encoder states
enum class EncoderDiff : int8_t {
    Backwards = -1,
    Stationary = 0,
    Forwards = 1,
    Undersampling = 2, // if step missed 
    Unknown = 3
};

// determine direction
EncoderDiff operator-(EncoderState current, EncoderState previous);

int8_t to_int(EncoderDiff diff);
float to_float(EncoderDiff diff);

EncoderState encode_state(Level a, Level b);

Level read_level(int pin);


/**
 * @class WheelEncoder
 * @brief Manages a quadrature wheel encoder to track distance and velocity.
 *
 * This class uses hardware interrupts on two GPIO pins (A and B) to monitor
 * the rotation of an encoder. It calculates total distance traveled and
 * instantaneous velocity.
 */
class WheelEncoder {
public:
    /**
     * @brief Construct a new Wheel Encoder object.
     * @param a The BCM GPIO pin number for the 'A' output of the encoder.
     * @param b The BCM GPIO pin number for the 'B' output of the encoder.
     */
    WheelEncoder(int a, int b);

    /**
     * @brief Destructor.
     */
    ~WheelEncoder();

    // disable copy and move constructors/assignments
    WheelEncoder(const WheelEncoder&) = delete;
    WheelEncoder& operator=(const WheelEncoder&) = delete;
    WheelEncoder(WheelEncoder&&) = delete;
    WheelEncoder& operator=(WheelEncoder&&) = delete;


    /**
     * @brief Get the total distance traveled since initialization.
     * @return The distance in meters.
     */
    float get_distance() const;

    /**
     * @brief Get the most recently calculated instantaneous velocity.
     * @return The velocity in meters per second.
     */
    float get_velocity() const;

private:
    /**
     * @brief Reads the current state from the GPIO pins.
     * @return The current EncoderState.
     */
    EncoderState read_state() const;

    /**
     * @brief The main interrupt handling logic. Called whenever a pin changes state.
     */
    void handle_interrupt();

    // Static ISR callbacks required by wiringPi
    static void isr_a();
    static void isr_b();

    int pin_a;
    int pin_b;

    // ISR and main thread
    std::atomic<int32_t> counter{0};
    std::atomic<float> velocity{0.0f};

    EncoderState last_state;
    std::chrono::steady_clock::time_point last_time;

    static WheelEncoder* instance;
};
