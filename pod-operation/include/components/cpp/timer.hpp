#ifndef TIMER_HPP
#define TIMER_HPP

#include <ctime>

class Timer {
public:
    void start() {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
    }

    long elapsed() {
        struct timespec end_time;
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        return (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    }

private:
    struct timespec start_time;
};

#endif