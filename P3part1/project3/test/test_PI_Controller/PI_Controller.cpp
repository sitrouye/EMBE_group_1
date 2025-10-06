#include "PI_Controller.h"
#include "MotorControllerBase.h"

PI_Controller::PI_Controller(float kp, float ki, float dt)
    : Kp(kp), Ki(ki), dt(dt), integral(0.0), output_min(0.0), output_max(100.0) {}

float PI_Controller::update(float setpoint, float measurement) {
    float error = setpoint - measurement;

    float temp_output = Kp * error + Ki * integral;
    if (temp_output < output_max && temp_output > output_min) {
        integral += error * dt;
    }

    float output = Kp * error + Ki * integral;

    // Saturate
    if (output > output_max) output = output_max;
    if (output < output_min) output = output_min;

    return output;
}

void PI_Controller::reset() {
    integral = 0.0;
}
