#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <functional>
#include "components/c/brakes.h"
#include "components/c/wheel_encoder.h"
#include "components/cpp/inverter.hpp"
#include "components/cpp/gyro.hpp"
#include "utils/gpio.h"
//add other ones later after :< 

// enum different states of FSM
enum class PodState {
    INIT,
    LOAD,
    PRECHARGE,
    START,
    STOP,
    FAULT,
    HALT
};

class StateMachine {
public:
    StateMachine();
    void transitionTo(PodState newState);
    void update();
    PodState getCurrentState() const;
    void controlLED();
    void registerEventAction(PodState state, std::function<bool()> event, std::function<void()> action, PodState targetState);

private:
    PodState currentState;
    void handleState();
    
    struct FSM_Transition {
        std::function<bool()> event;
        std::function<void()> action;
        PodState targetState;
    };
    
    std::map<PodState, FSM_Transition> fsmTransitions;
};

#endif 