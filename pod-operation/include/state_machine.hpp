#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <set>
#include <map>
#include <iostream>
#include "components/c/brakes.h"
#include "components/c/high_voltage_system.h"
#include "components/cpp/inverter.hpp"
#include "./read_all_sensors.hpp"
#include "./initialize_sensors.hpp"
#include "server.hpp"

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
    void fsm_tick();  // Make it public so it can be called externally

private:
    PodState currentState;
    std::mutex state_mutex; 
    Server server;
    std::map<PodState, std::set<PodState>> possibleStates;
    SensorInitializer sensors;
    bool validateSensorSnapshot(const boost::json::object& snapshot);


    void handleState(PodState currState);
    void readSensorValues();
    boost::asio::io_context io_context;


    void runInit();
    void runLoad();
    void runPrecharge();
    void runStart();
    void runStop();
    void runFault();
    void runHalt();
};

#endif // STATE_MACHINE_HPP

