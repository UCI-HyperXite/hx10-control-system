#include "../include/state_machine.hpp"

StateMachine::StateMachine()
    : currentState(PodState::INIT), server(8080)  // Proper member initialization
{
    possibleStates[PodState::INIT].insert(PodState::LOAD);
    possibleStates[PodState::LOAD].insert(PodState::PRECHARGE);
    possibleStates[PodState::LOAD].insert(PodState::FAULT);
    possibleStates[PodState::PRECHARGE].insert(PodState::START);
    possibleStates[PodState::PRECHARGE].insert(PodState::STOP);
    possibleStates[PodState::PRECHARGE].insert(PodState::FAULT);
    possibleStates[PodState::START].insert(PodState::STOP);
    possibleStates[PodState::START].insert(PodState::HALT);
    possibleStates[PodState::START].insert(PodState::FAULT);
    possibleStates[PodState::STOP].insert(PodState::LOAD);
    possibleStates[PodState::STOP].insert(PodState::START);
    possibleStates[PodState::STOP].insert(PodState::FAULT);
    possibleStates[PodState::FAULT].insert(PodState::HALT);
    possibleStates[PodState::FAULT].insert(PodState::INIT);
}

void StateMachine::transitionTo(PodState newState) {
    std::lock_guard<std::mutex> lock(state_mutex);

    if (possibleStates[currentState].find(newState) != possibleStates[currentState].end()) {
        currentState = newState;
        std::cout << "Successfully switched to state: " << static_cast<int>(newState) << "\n";
    } else {
        std::cout << "Invalid State Transition from " << static_cast<int>(currentState)
                  << " to " << static_cast<int>(newState) << "\n";
    }
}

void StateMachine::fsm_tick() {
    handleState(currentState);
}

void StateMachine::handleState(PodState currState) {
    switch (currState) {
        case PodState::INIT:      runInit(); break;
        case PodState::LOAD:      runLoad(); break;
        case PodState::PRECHARGE: runPrecharge(); break;
        case PodState::START:     runStart(); break;
        case PodState::STOP:      runStop(); break;
        case PodState::FAULT:     runFault(); break;
        case PodState::HALT:      runHalt(); break;
    }
}

void StateMachine::readSensorValues() {
    std::cout << "[INFO] Reading from all sensors...\n";
    io_context.restart();
    auto snapshot = read_all_sensors(io_context, sensors);
    io_context.run();
    std::cout << "[INFO] Sensor read complete.\n";
}

void StateMachine::runInit() {
    std::cout << "[INIT] Initializing server...\n";
    server.initialize();
  
    readSensorValues();

    std::cout << "Initializing Brakes...\n";
    initializeBrakes();
    std::cout << "Brakes initialized\n";

    std::cout << "Closing Brakes\n";
    engageBrakes();
    std::cout << "Brakes Closed\n";
}


void StateMachine::runLoad() {
    std::cout << "Opening brakes...\n";
    disengageBrakes();  
    std::cout << "Brakes opened\n";
    
    readSensorValues();
}

void StateMachine::runPrecharge() {
    
    std::cout << "Opening brakes...\n";
    disengageBrakes();
    std::cout << "Brakes opened\n";

    std::cout << "Initializing High Voltage System\n";
    initializeHighVoltageSystem();
    std::cout << "High System Voltage Initialized\n";
    
    std::cout << "Turning on High Voltage System\n";
    engageContactors();    
    std::cout << "High voltage System on\n";

    readSensorValues();

    transitionTo(PodState::START);

}

void StateMachine::runStart() {
    std::cout << "Opening brakes...\n";
    disengageBrakes();
    std::cout << "Brakes opened\n";

    //throttleCode();
    
    readSensorValues();
}

void StateMachine::runStop() {
    std::cout << "Closing brakes...\n";
    engageBrakes();
    std::cout << "Brakes closed\n";

    std::cout << "Turning off High Voltage System\n";
    //throttle to 0
    std::cout << "High voltage System off\n";

    readSensorValues();

}

void StateMachine::runFault() {
  
    std::cout << "Closing brakes...\n";
    engageBrakes();
    std::cout << "Brakes closed\n";

    std::cout << "Cutting off power to HV System\n";
    disengageContactors();
    std::cout << "Power System killed\n";

    readSensorValues();

    transitionTo(PodState::HALT);

  
}

void StateMachine::runHalt() {
    std::cout << "Closing brakes...\n";
    engageBrakes();
    std::cout << "Brakes closed\n";

    readSensorValues();

    transitionTo(PodState::INIT);
}

