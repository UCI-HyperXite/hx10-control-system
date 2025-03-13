#ifndef BRAKES_H
#define BRAKES_H

struct Brakes {
    int pin;
    void (*engage)(struct Brakes* self);
    void (*disengage)(struct Brakes* self);
};

void initializeBrakes(struct Brakes* brake);
void engage(struct Brakes* self);
void disengage(struct Brakes* self);

#endif
