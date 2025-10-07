#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <timer_msec.h>
#include <digital.h>

class Analog_out
{
public:
    Analog_out(int pin_no) : pin(pin_no)                     // the pin is initialized by passing pin_no
    {                                 // to the Digital_out(int pin_no) constructor

    }

    void init(int period_ms)
    {
        pin.init();
        timer.init(period_ms, 0);       // Starting with zero output
    }

    void set(int duty_cycle)
    {
        timer.set_duty_cycle(duty_cycle);
    }

private:
    Timer_msec timer;
public:    
    Digital_out pin;
};

#endif // ANALOG_OUT_H