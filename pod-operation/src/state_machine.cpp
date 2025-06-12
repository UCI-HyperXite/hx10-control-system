#include "../include/state_machine.hpp"

StateMachine::StateMachine() : currentState(PodState::INIT) {
    // stubbed functions w/ target states
    registerEventAction(PodState::INIT, []() { return false; }, []() { std::cout << "[FSM] INIT -> LOAD action triggered." << std::endl; }, PodState::LOAD);
    registerEventAction(PodState::LOAD, []() { return false; }, []() { std::cout << "[FSM] LOAD -> PRECHARGE action triggered." << std::endl; }, PodState::PRECHARGE);
    registerEventAction(PodState::PRECHARGE, []() { return false; }, []() { std::cout << "[FSM] PRECHARGE -> START action triggered." << std::endl; }, PodState::START);
    registerEventAction(PodState::START, []() { return false; }, []() { std::cout << "[FSM] START -> STOP action triggered." << std::endl; }, PodState::STOP);
    registerEventAction(PodState::STOP, []() { return false; }, []() { std::cout << "[FSM] STOP -> FAULT action triggered." << std::endl; }, PodState::FAULT);
    registerEventAction(PodState::FAULT, []() { return false; }, []() { std::cout << "[FSM] FAULT -> HALT action triggered." << std::endl; }, PodState::HALT);
    registerEventAction(PodState::HALT, []() { return false; }, []() { std::cout << "[FSM] HALT state entered." << std::endl; }, PodState::HALT);
}

void StateMachine::transitionTo(PodState newState) {
    if (currentState == PodState::HALT) {
        std::cout << "FSM HALTED" << std::endl;
        return;
    }
    
    std::cout << "FSM transitioning from " << static_cast<int>(currentState) << " to " << static_cast<int>(newState) << std::endl;
    currentState = newState;
    handleState();
}

void StateMachine::handleState() {
    if (fsmTransitions.find(currentState) != fsmTransitions.end()) {
        auto &transition = fsmTransitions[currentState];
        if (transition.event && transition.event()) {
            if (transition.action) {
                transition.action();
            }
            transitionTo(transition.targetState);
        }
    }
    controlLED();
}

void StateMachine::update() {
    if (fsmTransitions.find(currentState) != fsmTransitions.end()) {
        auto &transition = fsmTransitions[currentState];
        if (transition.event && transition.event()) {
            transitionTo(transition.targetState);
        }
    }
}

PodState StateMachine::getCurrentState() const {
    return currentState;
}

void StateMachine::registerEventAction(PodState state, std::function<bool()> event, std::function<void()> action, PodState targetState) {
    fsmTransitions[state] = {event, action, targetState};
}

void StateMachine::controlLED() {
    // if (currentState == PodState::START) {
    //     gpio_set_led_color("green");
    // } else if (currentState == PodState::STOP || currentState == PodState::INIT) {
    //     gpio_set_led_color("off");
    // } else if (currentState == PodState::HALT) {
    //     gpio_blink_led("red");
    // }
}
