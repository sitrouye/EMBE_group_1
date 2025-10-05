#include "controller.h"

MotorController::MotorController(State* initial, Digital_out* led)
    : currentState(initial), led(led) {
    
}

void MotorController::setState(State* state) {
    currentState = state;
    currentState->enter(this);
}

void MotorController::handleCommand(char cmd) {
    currentState->handleCommand(this, cmd);
}
