#include <Arduino.h>
#include "Controller.h"
#include "State.h"
#include "digital.h"

// Use Arduino’s onboard LED
Digital_out led(LED_BUILTIN);
MotorController controller(new Initialization(&led), &led);

void setup() {
    Serial.begin(9600);
    delay(2000);   // give Serial Monitor time to connect
    led.init();
    Serial.println("System starting...");
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        controller.handleCommand(cmd);
    }
}
