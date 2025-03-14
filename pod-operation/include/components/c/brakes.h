#ifndef BRAKES_H
#define BRAKES

struct Brakes {
    int pin;
};


void engage(int pin);
void disengage(int pin);


#endif