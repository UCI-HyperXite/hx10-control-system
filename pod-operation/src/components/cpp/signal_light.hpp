#ifndef SIGNALLIGHT_HPP
#define SIGNALLIGHT_HPP

#include <wiringPi.h>
#include <iostream>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

#define LED_PIN 1      // WiringPi pin 1 (GPIO18)
#define T1H 680        // Ideal time 800
#define T1L 310        // Ideal time 450
#define T0H 300        // Ideal time 400
#define T0L 750        // Ideal time 850
#define TCOMP 5000     // RP1 wake up time in nanoseconds
#define RESET_TIME 50  // Reset time in microseconds
#define LED_NUMBER 150 // Number of LEDs
#define RAINBOW_DELAY 2000000

extern uint32_t color_matrix[LED_NUMBER];

class Timer
{
public:
    void start();
    long elapsed();

private:
    struct timespec start_time;
};

class SignalLight
{
public:
    SignalLight();
    void setRed(uint32_t color_matrix[], int i);
    void setYellow(uint32_t color_matrix[], int i);
    void setGreen(uint32_t color_matrix[], int i);
    void sendBit(bool bit);
    void sendColors(uint32_t *colors);

private:
    Timer timer;
};

#endif