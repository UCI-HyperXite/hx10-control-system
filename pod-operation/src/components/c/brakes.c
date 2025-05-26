#include "../../../include/components/c/brakes.h"

struct Brakes brake;

void initializeBrakes(){
    brake.pin = PNEUMATICS_RELAY;
}

void engageBrakes(){
    setHigh(brake.pin);
}

void disengageBrakes(){
    setLow(brake.pin);
}