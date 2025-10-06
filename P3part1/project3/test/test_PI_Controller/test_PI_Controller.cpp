#include <Arduino.h>
#include <unity.h>
#include "PI_Controller.h"

PI_Controller* pi;

void setUp(void) {
    pi = new PI_Controller(0.5, 0.1, 0.1); // Kp, Ki, dt
}

void tearDown(void) {
    delete pi;
}

void test_zero_error(void) {
    float output = pi->update(10.0, 10.0);
    TEST_ASSERT_EQUAL_FLOAT(0.0, output); // error = 0, output = 0
}

void test_positive_error(void) {
    float output = pi->update(10.0, 5.0);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0, output);
}

void test_integral_windup(void) {
    // simulate saturation
    for (int i = 0; i < 50; i++) {
        pi->update(200.0, 0.0); // output will saturate at 100 internally
    }
    float output = pi->update(200.0, 0.0);
    TEST_ASSERT_LESS_OR_EQUAL_FLOAT(100.0, output); // anti-windup check
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_zero_error);
    RUN_TEST(test_positive_error);
    RUN_TEST(test_integral_windup);
    UNITY_END();
}

void loop() {
}
