#include "../../../include/components/cpp/signal_light.hpp"

SignalLight::SignalLight() {}

void SignalLight::setRed(uint32_t color_matrix[LED_NUMBER], int i) {
    color_matrix[i] = 0x002000;
}

void SignalLight::setYellow(uint32_t color_matrix[LED_NUMBER], int i) {
    color_matrix[i] = 0x202000;
}

void SignalLight::setGreen(uint32_t color_matrix[LED_NUMBER], int i) {
    color_matrix[i] = 0x200000;
}

void SignalLight::sendBit(bool bit) {
    if (bit) {
        digitalWrite(LED_PIN, HIGH);
        timer.start();
        while (timer.elapsed() < T1H);
        digitalWrite(LED_PIN, LOW);
        timer.start();
        while (timer.elapsed() < T1L);
    } else {
        digitalWrite(LED_PIN, HIGH);
        timer.start();
        while (timer.elapsed() < T0H);
        digitalWrite(LED_PIN, LOW);
        timer.start();
        while (timer.elapsed() < T0L);
    }
}

void SignalLight::sendColors(uint32_t* colors) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
    timer.start();
    while (timer.elapsed() < TCOMP);

    for (int j = 0; j < LED_NUMBER; j++) {
        for (int i = 23; i >= 0; i--) {
            sendBit((colors[j] >> i) & 1);
        }
    }

    usleep(RESET_TIME);
}