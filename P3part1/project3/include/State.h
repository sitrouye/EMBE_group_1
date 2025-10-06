#pragma once
#include "digital.h"

class MotorController; // forward declaration

class State {
public:
    virtual void enter(MotorController* controller) = 0;
    virtual void handleCommand(MotorController* controller, char cmd) = 0;
    virtual ~State() {}
};

class Initialization : public State {
public:
    Initialization(Digital_out* led) : led(led) {}
    void enter(MotorController* controller) override;
    void handleCommand(MotorController* controller, char cmd) override;
private:
    Digital_out* led;
};

class Operational : public State {
public:
    Operational(Digital_out* led) : led(led) {}
    void enter(MotorController* controller) override;
    void handleCommand(MotorController* controller, char cmd) override;
private:
    Digital_out* led;
};