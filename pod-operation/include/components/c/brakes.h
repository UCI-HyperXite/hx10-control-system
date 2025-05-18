#ifndef BRAKES_H
#define BRAKES_H

struct Brakes {
    int pin;
};

void initializeBrakes(int pin);
void engage(int pin);
void disengage(int pin);

#endif