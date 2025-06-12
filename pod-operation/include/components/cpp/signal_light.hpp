#ifndef SIGNALLIGHT_HPP
#define SIGNALLIGHT_HPP

#include <wiringPi.h>
#include <iostream>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include "timer.hpp"

#define LED_PIN 1        // WiringPi pin 1 (GPIO18)
#define T1H 680
#define T1L 310
#define T0H 300
#define T0L 750
#define TCOMP 5000       // RP1 wake up time in ns
#define RESET_TIME 50    // microseconds
#define LED_NUMBER 150
#define RAINBOW_DELAY 2000000 

extern uint32_t color_matrix[LED_NUMBER];

class SignalLight {
public:
    SignalLight();
    void setRed(uint32_t color_matrix[LED_NUMBER], int i);
    void setYellow(uint32_t color_matrix[LED_NUMBER], int i);
    void setGreen(uint32_t color_matrix[LED_NUMBER], int i);
    void sendBit(bool bit);
    void sendColors(uint32_t* colors);

private:
    Timer timer;
};

#endif