#pragma once
#include "MotorControllerBase.h"

class PI_Controller : public MotorControllerBase {
private:
    float Kp;
    float Ki;
    float dt;
    float integral;
    float output_min;
    float output_max;

public:
    PI_Controller(float kp, float ki, float dt);

    float update(float setpoint, float measurement);
    void reset() override;
};
