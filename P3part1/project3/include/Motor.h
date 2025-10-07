#pragma once
#include "digital.h"
#include "analog.h"

class Motor {
public :
    Motor() : encoder_A(2), encoder_B(2),AIN1(1), AIN2(3) {

    }
    void init(){
        encoder_A.init();
        encoder_B.init();
        AIN1.init();
        AIN2.init(1); //period of the PWM

        AIN2.pin.off();
        AIN1.off();

        count = 0;
    }
    void set_pwm(int pwm){
        AIN2.set(pwm);
    }
    void incr(){
        if (encoder_B.is_lo() && encoder_A.is_lo()) {
            count -= 1;    // one direction
        }
        else if (encoder_B.is_hi() && encoder_A.is_hi()) {
            count -= 1;    // one direction
        }
        else if (encoder_B.is_hi() && encoder_A.is_lo()) {
            count += 1;    // other direction
        }
        else if (encoder_B.is_lo() && encoder_A.is_hi()) {
            count += 1;    // other direction
        }

    }

    int get_count(){
        return count;
    }
    void pwm_on(){
        AIN2.pin.on();
    }
    void pwm_off(){
        AIN2.pin.off();
    }
private :
    Digital_in_portd encoder_A; //D2 (PD2)
    Digital_in  encoder_B; // D10 (PB2)
    Digital_out AIN1; //D9
    Analog_out AIN2; //D11
    int count;
    
};