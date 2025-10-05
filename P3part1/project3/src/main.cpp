#include <Arduino.h>
#include "Controller.h"
#include "State.h"
#include "digital.h"

// LED on Arduino Nano is PORTB5 (pin 13)
Digital_out led(5);

// Pre-allocated states
Initialization initState(&led);
Operational operationalState(&led);

// MotorController starts in Initialization state
MotorController controller(&initState, &led);

void setup() {
    Serial.begin(9600);
    delay(2000);   // wait for Serial Monitor
    led.init();
    Serial.println("System starting...");

    // Now we can safely call enter()
    controller.setState(&initState);
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        controller.handleCommand(cmd);
    }
}
