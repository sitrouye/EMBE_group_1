
#include <util/delay.h>

#include <Arduino.h>
#include <digital.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <PI_controller.h>
#include <analog.h>
#include "Motor.h"
#include "p_controller.h"
#include "digital.h"

Digital_out led(5);     // D13
// Digital_in_portd encoderA(2);
Motor mot;
P_controller Contr(1); //Kp value to account for *100 for duty cycle in % and /25 for max speed = 25
Interrupt_pin flt_pin(2);


float Kp =1.125;
float Ki = 0.315;
float dt = 3.55;

PI_Controller pi(Kp, Ki, dt);

volatile int count = 0;
volatile bool last = true;

volatile int count_last = 0;
int timer = 0;
int event = 0;
double ref = 1.5; //reference speed, here 1.5 rotation per second
double actual = 0.0; // actual speed
int duty_cycle = 0;

bool flt = false; //to stop if fault detected


int main() {

    Serial.begin(9600);

    // encoderA.init();
    led.init();
    mot.init();
    sei(); // enable interrupts

 


    int i =0;
    while(i<25){
        ref = 0;
        int delay = 2; //2ms to respect update rate (350Hz)
        _delay_ms(delay);
        
        double speed = double(count - count_last) / double(delay * 0.001 * 1400); //speed in rot/s
        actual = speed;
        count_last = count;
        timer += 2;

        double u =  Contr.update(ref, speed);
        duty_cycle = duty_cycle + static_cast<int>(u);
        mot.set_pwm(duty_cycle);

        Serial.print(">duty_cycle:");
        Serial.print(duty_cycle);
        Serial.print(",u:");
        Serial.print(u);
        Serial.print(",speed:");
        Serial.print(speed);
        Serial.print(",ref:");
        Serial.print(ref);
        Serial.print(",time_ms:");
        Serial.print(timer);
        Serial.print("\r\n");

        i++;
    }
    
    // while(i <100){
    //     ref = 25;
    //     int delay = 2; //2ms to respect update rate (350Hz)
    //     _delay_ms(delay);
        
    //     double speed = double(mot.get_count() - count_last) / double(delay * 0.001 * 1400); //speed in rot/s

    //     count_last = mot.get_count();
    //     actual = speed;
    //     timer += 2;


    //     double u =  Contr.update(ref, speed);
    //     duty_cycle = duty_cycle + static_cast<int>(u);
    //     mot.set_pwm(duty_cycle);


    //     Serial.print(">duty_cycle:");
    //     Serial.print(duty_cycle);
    //     Serial.print(",u:");
    //     Serial.print(u);
    //     Serial.print(",speed:");
    //     Serial.print(speed);
    //     Serial.print(",ref:");
    //     Serial.print(ref);
    //     Serial.print(",time_ms:");
    //     Serial.print(timer);
    //     Serial.print("\r\n");
    //     i++;

    // }
    ref = 25;
    while(i <2000){


        int delay = 2; //2ms to respect update rate (350Hz)
        _delay_ms(delay);
        
        double speed = double(mot.get_count() - count_last) / double(delay * 0.001 * 1400); //speed in rot/s

        count_last = mot.get_count();
        actual = speed;

        timer += 2;


        double u =  pi.update(ref, speed);
        duty_cycle = static_cast<int>(u);
        mot.set_pwm(duty_cycle);


        Serial.print(">duty_cycle:");
        Serial.print(duty_cycle);
        Serial.print(",u:");
        Serial.print(u);
        Serial.print(",speed:");
        Serial.print(speed);
        Serial.print(",ref:");
        Serial.print(ref);
        Serial.print(",time_ms:");
        Serial.print(timer);
        Serial.print("\r\n");

        i++;

    }

    return 0;
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


// ISR(INT1_vect){
//     flt_pin.interr(); //triggered on logic change of pin FLT
// }
ISR(INT0_vect){
    mot.incr(); //increments count of motor
}