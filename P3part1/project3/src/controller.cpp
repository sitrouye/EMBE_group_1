#include "Controller.h"
#include "PI_Controller.h"

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


void MotorController::change_ctrl(float kp, float ki, float dt){
    PI_Controller pi(kp, ki, dt);
    ctrl = &pi;
}