#include <Arduino.h>
#include "Controller.h"
#include "State.h"
#include "digital.h"
#include "PI_Controller.h" 

Digital_out led(5);

Initialization initState(&led);
Operational operationalState(&led);

PI_Controller pi(0.5, 0.05, 0.1);

MotorController controller(&initState, &led, &pi);

float measurement = 0;
float setpoint = 200;
unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial); 
    led.init();
    Serial.println("System starting...");

    controller.setState(&initState); 
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        Serial.print("Received command: ");
        Serial.println(cmd);
        controller.handleCommand(cmd);
    }

    if (millis() - lastUpdate >= 100) {
        lastUpdate = millis();
        float pwm = 0;

        if (controller.getCurrentState() == &operationalState) {
            pwm = controller.getController()->update(setpoint, measurement);
            measurement += (pwm - measurement) * 0.05;
        }

        Serial.print("Current State: ");
        Serial.print((controller.getCurrentState() == &operationalState) ? "Operational" : "Initialization");
        Serial.print(" | PWM: ");
        Serial.print(pwm);
        Serial.print(" | Measurement: ");
        Serial.println(measurement);
    }
}
