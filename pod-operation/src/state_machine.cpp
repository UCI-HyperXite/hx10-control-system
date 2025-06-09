#include "../include/state_machine.hpp"

StateMachine::StateMachine()
    : currentState(PodState::INIT), server(8080)  
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

bool StateMachine::validateSensorSnapshot(const boost::json::object& snapshot) {
    bool valid = true;

    auto check_range = [&](const std::string& key, double min, double max) {
        if (!snapshot.if_contains(key)) return;
        double value = snapshot.at(key).as_double();
        if (value < min || value > max) {
            std::cerr << "[ERROR] " << key << " out of range: " << value
                      << " (expected " << min << " - " << max << ")\n";
            valid = false;
        }
    };

    check_range("gx", -500, 500);
    check_range("gy", -500, 500);
    check_range("gz", -500, 500);

    check_range("ax", -10, 10);
    check_range("ay", -10, 10);
    check_range("az", -10, 10);

    check_range("pod_height_mm", 0, 500); 

    check_range("pressure_downstream", 0, 300);
    check_range("pressure_upstream", 0, 5000);

    check_range("pressure_left_coolant", 0, 300);
    check_range("pressure_right_coolant", 0, 300);

    check_range("temp_ads1", -40, 125);
    check_range("temp_ads2", -40, 125);
    check_range("temp_ads3", -40, 125);
    check_range("temp_ads4", -40, 125);

    check_range("encoder_distance", 0, 1000); 
    check_range("encoder_velocity", -50, 50);  

    return valid;
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

    std::cout << "[INFO] Snapshot:\n" << boost::json::serialize(snapshot) << "\n";

    server.send_json(snapshot);

    if (!validateSensorSnapshot(snapshot)) {
        std::cerr << "[WARNING] Sensor validation failed. Transitioning to FAULT state.\n";
        transitionTo(PodState::FAULT);
    }
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

