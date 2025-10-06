#include "State.h"
#include "controller.h"
#include <Arduino.h>

extern Initialization initState;
extern Operational operationalState;

void Initialization::enter(MotorController* controller) {
    led->off();
    Serial.println("Initializing parameters...");
    delay(500);
    Serial.println("Type 's' to start system.");
}

void Initialization::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 's') {
        Serial.println("Starting system...");
        controller->setState(&operationalState);
    } else {
        Serial.println("Unknown command. Type 's' to start.");
    }
}

void Operational::enter(MotorController* controller) {
    led->on();
    Serial.println("System operational. Type 'r' to reset.");
}

void Operational::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 'r') {
        Serial.println("Resetting...");
        controller->setState(&initState);
    } else {
        Serial.println("Unknown command.");
    }
}