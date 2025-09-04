#include <util/delay.h>

#include <Arduino.h>
#include <digital.h>
#include <avr/io.h>
#include <avr/interrupt.h>


Digital_out led(5);     // D13
Digital_in_portd encoder_A(2); // (PD2)
Digital_in  encoder_B(2); // D10 (PB2)

volatile int count = 0;
volatile bool last = true;


int main() {

  Serial.begin(9600);


  led.init();
  encoder_A.init();
  encoder_B.init();

  sei(); // enable interrupts

 

  while (1) {


}

  return 0;
}

void loop() 
  {
    Serial.println(count);  
}

ISR(INT0_vect)
{
  // action to be done on logic change of encoder_A

  
  if (encoder_B.is_lo() && encoder_A.is_lo()) {
      count -= 1;    // one direction
      led.toggle();  // blink LED
  }
  else if (encoder_B.is_hi() && encoder_A.is_hi()) {
      count -= 1;    // one direction
      led.toggle();  // blink LED
  }
  else if (encoder_B.is_hi() && encoder_A.is_lo()) {
      count += 1;    // other direction
      led.toggle();  // blink LED
  }
  else if (encoder_B.is_lo() && encoder_A.is_hi()) {
    count += 1;    // other direction
    led.toggle();  // blink LED
  }

}



