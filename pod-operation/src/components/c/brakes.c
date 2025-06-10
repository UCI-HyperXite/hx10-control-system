#include "../../../include/components/c/brakes.h"
#include <wiringPi.h>

struct Brakes brake;

void initializeBrakes(){
    brake.pin = PNEUMATICS_RELAY;
}

void engageBrakes(){
    setLow(brake.pin);
}

void disengageBrakes(){
    setHigh(brake.pin);
}