#include "analog_out.h"

Analog_out::Analog_out(int pin_no)
: pin(pin_no)                     // the pin is initialized by passing pin_no
{                                 // to the Digital_out(int pin_no) constructor

}

void Analog_out::init(int period_ms)
{
  pin.init();
  timer.init(period_ms, 0);       // Starting with zero output
}

void Analog_out::set(int duty_cycle)
{
  timer.set_duty_cycle(duty_cycle);
}