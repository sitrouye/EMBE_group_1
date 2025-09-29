#include "Controller.h"

MotorController::MotorController(State* initial, Digital_out* led)
    : currentState(initial), led(led) {
    currentState->enter(this);
}

void MotorController::setState(State* state) {
    delete currentState;
    currentState = state;
    currentState->enter(this);
}

void MotorController::handleCommand(char cmd) {
    currentState->handleCommand(this, cmd);
}
