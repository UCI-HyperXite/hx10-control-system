// #include "../../../include/components/cpp/signal_light.hpp"
#include "signal_light.hpp"

SignalLight::SignalLight() {}
uint32_t color_matrix[LED_NUMBER];  // Define the array


void Timer::start()
{
    clock_gettime(CLOCK_MONOTONIC, &start_time);
}

long Timer::elapsed()
{
    struct timespec end_time;
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            return (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
}

void SignalLight::setRed(uint32_t color_matrix[LED_NUMBER], int i)
{
    color_matrix[i] = 0x002000;
}

void SignalLight::setYellow(uint32_t color_matrix[LED_NUMBER], int i)
{
    color_matrix[i] = 0x202000;
}

void SignalLight::setGreen(uint32_t color_matrix[LED_NUMBER], int i)
{
    color_matrix[i] = 0x200000;
}

void SignalLight::sendBit(bool bit)
{
    if (bit)
    {
        // Send '1'
        digitalWrite(LED_PIN, HIGH);
        timer.start();
        while (timer.elapsed() < T1H)
            ; // High time for '1'
        digitalWrite(LED_PIN, LOW);
        timer.start();
        while (timer.elapsed() < T1L)
            ; // Low time for '1'
    }
    else
    {
        // Send '0'
        digitalWrite(LED_PIN, HIGH);
        timer.start();
        while (timer.elapsed() < T0H)
            ; // High time for '0'
        digitalWrite(LED_PIN, LOW);
        timer.start();
        while (timer.elapsed() < T0L)
            ; // Low time for '0'
    }
}

void SignalLight::sendColors(uint32_t *colors)
{
    digitalWrite(LED_PIN, HIGH); // Generate a short pulse
    digitalWrite(LED_PIN, LOW);

    timer.start();
    while (timer.elapsed() < TCOMP)
        ; // Wait for the RP1 to wake up before sending data

    for (int j = 0; j < LED_NUMBER; j++)
    {
        for (int i = 23; i >= 0; i--)
        {
            sendBit((colors[j] >> i) & 1);
        }
    }

    usleep(RESET_TIME);
}
