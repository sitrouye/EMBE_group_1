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
    } 
    else if (cmd == 'p'){
        Serial.println("set pre-operational...");
        controller->setState(new PreOperational(led));
    }
    else if (cmd == 's'){
        Serial.println("Stop");
        controller->setState(new Stopped(led));
    }
    else {
        Serial.println("Unknown command.");
    }
}






void Stopped::enter(MotorController* controller) {
    led->on();
    controller->stop();
    Serial.println("System operational. Type 'r' to reset.");
}

void Stopped::during(MotorController* controller){
    led->toggle();
    delay(250);
}

void Stopped::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 'r') {
        Serial.println("Resetting...");
        controller->setState(new Initialization(led));
    } 
    else if (cmd == 'p'){
        Serial.println("set pre-operational...");
        controller->setState(new PreOperational(led));
    }
    else if (cmd == 'o'){
        Serial.println("set operational...");
        controller->setState(new Operational(led));
    }
    
    else {
        Serial.println("Unknown command.");
    }
}



void PreOperational::enter(MotorController* controller) {
    led->on();
    controller->stop();
    Serial.println("System operational. Type 'r' to reset.");
}

void PreOperational::during(MotorController* controller){
    led->toggle();
    delay(250);
}


void PreOperational::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 'r') {
        Serial.println("Resetting...");
        controller->setState(new Initialization(led));
    } 
    else if (cmd == 'p'){
        Serial.println("set pre-operational...");
        controller->setState(new PreOperational(led));
    }
    else if (cmd == 'o'){
        Serial.println("set operational...");
        controller->setState(new Operational(led));
    }
    
    else {
        Serial.println("Unknown command.");
    }
}

