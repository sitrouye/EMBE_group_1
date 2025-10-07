#pragma once
#include "Motor.h"

class MotorControllerBase {
public:
    virtual float update(float setpoint, float measured) = 0;
    virtual void reset() = 0;
    virtual ~MotorControllerBase() {}

};
