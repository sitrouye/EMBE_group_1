#include <util/delay.h>

#include <Arduino.h>
#include <digital.h>



int main() {

  Serial.begin(9600);
  // Pins: LED PB5, Encoder A=PB1, B=PB2
  Digital_out led(5);     // D13
  Digital_in  encoder_A(1); // D9 (PB1)
  Digital_in  encoder_B(2); // D10 (PB2)

  led.init();
  encoder_A.init();
  encoder_B.init();

  volatile int count = 0;

  // const uint16_t TS_US = 182;

  bool last = encoder_A.is_hi();  // initial state of A

while (1) {
    _delay_ms(100);        // too slow
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
    Serial.println(count);
}

  return 0;
}


