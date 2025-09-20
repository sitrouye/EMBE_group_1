#include <util/delay.h>

#include <Arduino.h>
#include <digital.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <p_controller.h>
#include <analog_out.h>

Digital_out led(5);     // D13
Digital_in_portd encoder_A(2); //D2 (PD2)
Digital_in  encoder_B(2); // D10 (PB2)
Digital_out AIN1(1); //D9
Analog_out AIN2(3); //D11
P_controller Contr(10); //Kp value


volatile int count = 0;
volatile bool last = true;

volatile int count_last = 0;
int timer = 0;
int event = 0;
double ref = 0.0; //reference speed, here 1 rotation per second
double actual = 0.0; // actual speed

int main() {

  Serial.begin(9600);


  led.init();
  encoder_A.init();
  encoder_B.init();
  AIN1.init();
  AIN2.init(10);

  sei(); // enable interrupts

 
  AIN2.pin.off();
  AIN1.off();
  while (1) {

    if (Serial.available() > 0){
      // read the incoming byte:
      event = Serial.read();

      // say what you got:
      Serial.print("\n I received: ");
      Serial.println(event, DEC);

      // you can compare the value received to a character constant, like 'h'.
      if (event == 's'){
        AIN2.pin.off();
        AIN1.off();
      }
      if (event == 'r'){
        while (1){
          AIN2.set(20);

          _delay_ms(10);
          
          double speed = double(count - count_last) / double(0.01 * 1400); //speed in rot/s
          actual = speed;
          count_last = count;
          timer += 1;
          if (speed != 0){
            Serial.print("\n speed: ");
            Serial.println(speed);

            Serial.print("\n timer (*10 in ms): ");

            Serial.println(timer);
          }

          int duty_cycle = int(Contr.update(ref, actual));
          AIN2.set(duty_cycle); //change duty cycle for getting to the right speed
          Serial.println(duty_cycle);
        
        }
      }
      if (event == 'o'){
        AIN2.pin.off();
        AIN1.on();
      }
    }
    // AIN2.set(0);

    // _delay_ms(10);
    
    // double speed = double(count - count_last) / double(0.01 * 1400); //speed in rot/s
    // actual = speed;
    // count_last = count;
    // timer += 1;
    // if (speed != 0){
    //   Serial.print("\n speed: ");
    //   Serial.println(speed);
    //   // Serial.println(count);
      
    //   Serial.print("\n timer (*10 in ms): ");

    //   Serial.println(timer);
    // }

    // int duty_cycle = int(Contr.update(ref, actual));
    // AIN2.set(duty_cycle);

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


ISR(TIMER1_COMPA_vect)
{
  AIN2.pin.on();
}

ISR(TIMER1_COMPB_vect)
{
  AIN2.pin.off();
}


