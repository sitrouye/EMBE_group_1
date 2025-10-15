#include <Arduino.h>
#include "Controller.h"
#include "State.h"
#include "digital.h"
#include "PI_Controller.h" 
#include "Motor.h"

Motor mot; //motor to be controlled
Digital_out led(5);
Interrupt_pin flt_pin(2);
Initialization initState(&led);
Operational operationalState(&led);
PreOperational preopState(&led);

PI_Controller pi(0.5, 0.05, 0.1);

MotorController controller(&initState, &led, &pi);


bool flt = false; //to stop if fault detected.

float measurement = 0;
float setpoint = 25;
unsigned long lastUpdate = 0;

double Kp;
double Ti;// parameters to be initialized in pre-operational state
double dt;
float pwm = 0;

int count_last = 0;


void setup() {
    Serial.begin(9600);
    while (!Serial); 
    led.init();
    flt_pin.init();
    mot.init();
    Serial.println("System starting...");

    controller.setState(&initState); 

    sei();

}

void loop() {
    
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        Serial.print("Received command: ");
        Serial.println(cmd);
        controller.handleCommand(cmd);

    }
    if (flt){
        Serial.println("flt");
        controller.handleCommand('f');
    }
    if (millis() - lastUpdate >= 100) {
        
        int delay = millis() - lastUpdate;
        lastUpdate = millis();
        
        
        
        double speed = double(mot.get_count() - count_last) / double(delay * 0.001 * 1400); //speed in rot/s

        count_last = mot.get_count();

        if (controller.getCurrentState() == &operationalState) {
            measurement = speed;
            pwm = controller.getController()->update(setpoint, measurement);
            // measurement += (pwm - measurement) * 0.05;
            mot.set_pwm(static_cast<int>(pwm)); //new speed for the motor 
            Serial.print(">duty_cycle:");
            Serial.print(pwm);
            Serial.print(",speed:");
            Serial.print(speed);
            Serial.print(",ref:");
            Serial.print(setpoint);
            Serial.print(",time_ms:");
            Serial.print(millis());
            Serial.print("\r\n");
        }

        // Serial.print("Current State: ");
        // Serial.print((controller.getCurrentState() == &operationalState) ? "Operational" : "Initialization");
        // Serial.print(" | PWM: ");
        // Serial.print(pwm);
        // Serial.print(" | Measurement: ");
        // Serial.println(measurement);




       
    }
}




//PWM
ISR(TIMER1_COMPA_vect)
{
  mot.pwm_on();
}

ISR(TIMER1_COMPB_vect)
{
  mot.pwm_off();
}

ISR(INT1_vect){
    flt = true;
}
ISR(INT0_vect){
    mot.incr();
    Serial.println("incremented !");
}



