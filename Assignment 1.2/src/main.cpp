#include <util/delay.h>

#include <Arduino.h>
#include <digital.h>
#include <timer_msec.h>

Digital_out led(5);     // D13
Digital_in  encoder_A(1); // D9 (PB1)
Digital_in  encoder_B(2); // D10 (PB2)
Timer_msec timer;
volatile int count = 0;
volatile bool last = true;


int main() {

  Serial.begin(9600);
  // Pins: LED PB5, Encoder A=PB1, B=PB2


  led.init();
  encoder_A.init();
  encoder_B.init();



  // const uint16_t TS_US = 182;
  double samp_rate = 1400*155/(60*1000);
  double ts = 1/samp_rate;
  timer.init(ts,100);
  sei();

 

  while (1) {


    Serial.println(count);
}

  return 0;
}


ISR(TIMER1_COMPA_vect)
{
  // action to be done every ts
  led.toggle();
  double samp_rate = 1400*155/(60*1000);
  double ts = 1/samp_rate;
  _delay_ms(ts);        
  bool state = encoder_A.is_hi();  // read channel A

  if (state != last) {   
      if (encoder_B.is_lo()) {
          count -= 1;    // one direction
          led.toggle();  // blink LED
      }
      if (encoder_B.is_hi()) {
          count += 1;    // other direction
          led.toggle();  // blink LED
      }
      last = state;      // update previous state
  }
}



