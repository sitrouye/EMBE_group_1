

#pragma once
#include <avr/io.h>

class Timer_msec
{
public:

    Timer_msec(){
        TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
        TCCR1B = 0;
        TCNT1 = 0; // initialize counter value to 0
    }

    void init(int period_ms, int duty_cycle){


        OCR1A = period_ms * uint32_t{16000} / 1024 - 1;
        OCR1B = OCR1A * static_cast<uint32_t>(duty_cycle) / 100; // assign target count to compare register A (must be less than 65536)
        TCCR1B |= (1 << WGM12); // clear the timer on compare match A
        TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
        TIMSK1 |= (1 << OCIE1B); // set interrupt on compare match B

        TCCR1B |= (1 << CS12) | (1 << CS10);

    }

    void set_duty_cycle(int duty_cycle){

        OCR1B = OCR1A * static_cast<uint32_t>(duty_cycle) / 100;

    }
};


