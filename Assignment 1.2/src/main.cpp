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

volatile int count_last = 0;
int timer = 0;

int main() {

  Serial.begin(9600);


  led.init();
  encoder_A.init();
  encoder_B.init();

  sei(); // enable interrupts

 

  while (1) {
    _delay_ms(10);
    
    double speed = double(count - count_last) / double(0.01 * 2*7*100); //speed in tr/s
    count_last = count;
    timer += 1;
    if (speed != 0){
      Serial.println(speed);
      // Serial.println(count);
      Serial.println(timer);
    }

  }

  return 0;
}

// void loop() 
//   {
//     Serial.println(count);  
// }

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



