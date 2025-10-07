#pragma once
#include "MotorControllerBase.h"

class P_Controller : public MotorControllerBase {
    float Kp;
public:
    P_Controller(float Kp) : Kp(Kp) {}
    float update(float setpoint, float measurement) override {
        return Kp * (setpoint - measurement);
    }

};
