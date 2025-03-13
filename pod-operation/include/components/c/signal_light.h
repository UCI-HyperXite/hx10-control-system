#ifndef SIGNALLIGHT_H
#define SIGNALLIGHT_H

struct SignalLight {
    int pin;
    void (*setColor)(struct SignalLight* self);
};

void initializeSignalLight(struct SignalLight* signalLight);
void setColor(struct SignalLight* self);

#endif