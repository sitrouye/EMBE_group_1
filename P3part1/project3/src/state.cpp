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
    extern Motor mot;
    mot.set_pwm(50);
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
    else if (cmd == 'f'){
        Serial.println("Stop");
        controller->setState(new Stopped(led));
    }
    else {
        Serial.println("Unknown command.");
    }
}






void Stopped::enter(MotorController* controller) {
    led->blink(250); //led toggles every 250ms
    // controller->set(0);
    extern Motor mot;
    mot.set_pwm(0);
    Serial.println("Stopped");
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
    extern double Kp;
    extern double Ti;
    extern double dt;
    Serial.println("Pre-operational : please enter value for Kp: ");
    Serial.setTimeout(5000);
    Kp = Serial.parseInt();
    Serial.println("Pre-operational : please enter value for Ti: ");
    Ti = Serial.parseInt();
    controller->change_ctrl(Kp, Ti,dt);
}




void PreOperational::handleCommand(MotorController* controller, char cmd) {
    if (cmd == 'r') {
        Serial.println("Resetting...");
        controller->setState(new Initialization(led));
    } 
    else if (cmd == 'f'){
        Serial.println("stop");
        controller->setState(new Stopped(led));
    }
    else if (cmd == 'o'){
        Serial.println("set operational...");
        controller->setState(new Operational(led));
    }
    
    else {
        Serial.println("Unknown command.");
    }
}