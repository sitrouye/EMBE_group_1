#include <Arduino.h>
#include "unity.h"

#include "PI_Controller.h"
#include "Controller.h"
#include "State.h"
#include "digital.h"

#include "PI_Controller.cpp"
#include "controller.cpp"
#include "state.cpp"


void setUp(void) {
}

void tearDown(void) {
}

void test_PIController_NormalUpdate(void) {
    PI_Controller pi = PI_Controller(1.0, 0.5, 0.1);
    float setpoint = 100;
    float measurement = 0;

    float out1 = pi.update(setpoint, measurement);
    float out2 = pi.update(setpoint, measurement);

    TEST_ASSERT_TRUE(out2 > out1);
}

void test_PIController_AntiWindup(void) {
    PI_Controller pi = PI_Controller(1.0, 0.5, 0.1);
    float setpoint = 1000; // intentionally high to saturate
    float measurement = 0;
    float output;

    for (int i = 0; i < 50; i++) {
        output = pi.update(setpoint, measurement);
        TEST_ASSERT_TRUE(output <= 100.0f); // ensuring output does not exceed max
    }
}

void test_PIController_Reset(void) {
    PI_Controller pi = PI_Controller(1.0, 0.5, 0.1);
    float setpoint = 50;
    float measurement = 0;

    pi.update(setpoint, measurement);
    pi.reset();
    float out = pi.update(setpoint, measurement);

    TEST_ASSERT_TRUE(out < 100); // integral reset should reduce output
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_PIController_NormalUpdate);
    RUN_TEST(test_PIController_AntiWindup);
    RUN_TEST(test_PIController_Reset);
    return UNITY_END();
}
