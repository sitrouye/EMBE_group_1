#pragma once
#include "State.h"
#include "MotorControllerBase.h"
#include "digital.h"

class MotorController {
    State* currentState;
    Digital_out* led;
    MotorControllerBase* ctrl;   // part3

public:
    MotorController(State* initial, Digital_out* led, MotorControllerBase* ctrl);

    void setState(State* state);
    void handleCommand(char cmd);
    void change_ctrl(float kp, float ki, float dt);

    MotorControllerBase* getController() { return ctrl; }
    State* getCurrentState() { return currentState; }
};
