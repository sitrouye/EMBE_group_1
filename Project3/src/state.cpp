#include "State.h"
#include "Controller.h"
#include <Arduino.h>

void Initialization::enter(MotorController* controller) {
    led->off();
    Serial.println("Initializing parameters...");
    delay(500);
    Serial.println("Boot-up message sent.");
    controller->setState(new Operational(led));
}


void Operational::enter(MotorController* controller) {
    led->on();
    Serial.println("System operational. Type 'r' to reset.");
}

void Operational::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 'r') {
        Serial.println("Resetting...");
        controller->setState(new Initialization(led));
    } else {
        Serial.println("Unknown command.");
    }
}
