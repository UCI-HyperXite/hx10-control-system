#include "signal_light.hpp"

class Timer
{
public:
    void start()
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
    }

    long elapsed()
    {
        struct timespec end_time;
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        return (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

private:
    struct timespec start_time;
};

class SignalLight
{
public:
    SignalLight()
    {
    }
    void setRed(uint32_t color_matrix[LED_NUMBER], int i)
    {
        // color_matrix[i] = 0x00FF00;
        color_matrix[i] = 0x002000;
    }
    void setYellow(uint32_t color_matrix[LED_NUMBER], int i)
    {
        // color_matrix[i] = 0xFFFF00;
        color_matrix[i] = 0x202000;
    }
    void setGreen(uint32_t color_matrix[LED_NUMBER], int i)
    {
        // color_matrix[i] = 0xFF0000;
        color_matrix[i] = 0x200000;
    }

    void sendBit(bool bit)
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

    void sendColors(uint32_t *colors)
    {
        digitalWrite(LED_PIN, HIGH); // Changed the way WAKEUP occurs. Now a single 20ns pulse get generated, then we wait.
        digitalWrite(LED_PIN, LOW);  //  This pulse is so fast it gets ignored by the WS2812B and ensure accurate timings.
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

        // Ensure reset time
        usleep(RESET_TIME);
    }
};