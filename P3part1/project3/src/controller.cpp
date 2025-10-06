#include "Controller.h"

MotorController::MotorController(State* initial, Digital_out* led, MotorControllerBase* ctrl)
    : currentState(initial), led(led), ctrl(ctrl) {
}

void MotorController::setState(State* state) {
    currentState = state;
    currentState->enter(this);
}

void MotorController::handleCommand(char cmd) {
    currentState->handleCommand(this, cmd);
}
