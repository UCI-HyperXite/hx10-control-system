#include <iostream>
#include <stdio.h>
// #include <wiringPi.h>
#include "../../include/components/c/signal_light.h"
#include "../../include/utils/gpio.h"

//https://github.com/jgarff/rpi_ws281x


void initializeSignalLight(struct SignalLight* signalLight) {
    signalLight->pin = SIGNAL_LIGHT_RELAY;
    signalLight->setColor;
}

void setColor(struct SignalLight* self){
    //self.CHANGE COLOR (USE THE LIBRARY)
}