#include <Arduino.h>
#include "Controller.h"
#include "State.h"
#include "digital.h"

// Use Arduino’s onboard LED
Digital_out led(LED_BUILTIN);
MotorController controller(new Initialization(&led), &led);
bool flt = false;


void setup() {
    Serial.begin(9600);
    delay(2000);   // give Serial Monitor time to connect
    led.init();
    Serial.println("System starting...");
    sei();
}

void loop() {
    while (1){
        if (Serial.available() > 0) {
            char cmd = Serial.read();
            Serial.print("\n I received: ");
            Serial.println(cmd, DEC);
            controller.handleCommand(cmd);
        }
        if (flt){
            controller.handleCommand('s');
        }
    }
}


ISR(INT0_vect){
    //action to be done on logic change of FLT
    flt = true;

}