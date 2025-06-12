// #include <iostream>
// #include <future>
// #include <thread>
// #include "../include/components/cpp/signal_light.hpp"

// std::future<void> blink(SignalLight signallight){
//     if (wiringPiSetup() == -1) {
//         std::cerr << "WiringPi setup failed!" << std::endl;
//     }
//     pinMode(LED_PIN, OUTPUT);
//     while (true) {
//         for (int i = 0; i < LED_NUMBER; i++) {
//             setRed(color_matrix, i);
//         }
//         sendColors(color_matrix);
//         usleep(RAINBOW_DELAY);
//         for (int i = 0; i < LED_NUMBER; i++) {
//             setYellow(color_matrix, i);
//         }
//         sendColors(color_matrix);
//         usleep(RAINBOW_DELAY);
//         for (int i = 0; i < LED_NUMBER; i++) {
//             setGreen(color_matrix, i);
//         }
//         sendColors(color_matrix);
//         usleep(RAINBOW_DELAY);
//     }
// }