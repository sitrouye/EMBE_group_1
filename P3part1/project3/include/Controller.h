#pragma once
#include "State.h"

class MotorController {
    State* currentState;
    Digital_out* led;
public:
    MotorController(State* initial, Digital_out* led);
    void setState(State* state);
    void handleCommand(char cmd);
};
