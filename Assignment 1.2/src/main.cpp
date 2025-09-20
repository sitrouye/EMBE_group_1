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
P_controller Contr(0.1); //Kp value to account for *100 for duty cycle in % and /25 for max speed = 25


volatile int count = 0;
volatile bool last = true;

volatile int count_last = 0;
int timer = 0;
int event = 0;
double ref = 1.5; //reference speed, here 1.5 rotation per second
double actual = 0.0; // actual speed
int duty_cycle = 0;



int main() {

  Serial.begin(9600);


  led.init();
  encoder_A.init();
  encoder_B.init();
  AIN1.init();
  AIN2.init(1); //period of the PWM

  sei(); // enable interrupts

 
  AIN2.pin.off();
  AIN1.off();
  // while (1) {

    // if (Serial.available() > 0){

    //   // read the incoming byte:
    //   event = Serial.read();

    //   // say what you got:
    //   Serial.print("\n I received: ");
    //   Serial.println(event, DEC);

    //   // you can compare the value received to a character constant, like 'h'.
    //   if (event == 's'){
    //     AIN2.set(0);
    //     AIN1.off();
    //   }
    //   // if (event == 'r'){
    //   //   AIN1.off();
    //   //   AIN2.set(duty_cycle);
    //   //   while (actual != ref){
          

    //   //     _delay_ms(10);
          
    //   //     double speed = double(count - count_last) / double(0.01 * 1400); //speed in rot/s
    //   //     actual = speed;
    //   //     count_last = count;
    //   //     timer += 1;
    //   //     if (speed != 0){
    //   //       Serial.print("\n speed: ");
    //   //       Serial.println(speed);

    //   //       Serial.print("\n timer (*10 in ms): ");

    //   //       Serial.println(timer);
    //   //     }

    //   //     duty_cycle += int(Contr.update(ref, actual));
    //   //     AIN2.set(duty_cycle); //change duty cycle for getting to the right speed
          
    //   //     Serial.print("\n duty cycle : ");
    //   //     Serial.println(duty_cycle);
        
    //   //   }
    //   // }
    //   if (event == 'o'){
    //     AIN1.off();
    //     AIN2.set(0);

    //   }

    //   if (event == 'h'){
    //     AIN1.off();
    //     AIN2.set(50);
    //     Serial.print("\n duty cycle : 50");

    //   }
    //   if (event == 'q'){
    //     AIN1.off();
    //     AIN2.set(25);
    //     Serial.print("\n duty cycle : 25");
    //   }
  
    //   if (event == 'f'){
    //     AIN2.set(99);
    //     AIN1.off();
    //   }
      
    // }

    


  duty_cycle = 99;
  AIN1.off();
  AIN2.set(duty_cycle);

  _delay_ms(100); //time for the motor to accelerate

  int count_1 = count;
  int delay = 2; //2ms to respect update rate (350Hz)
  _delay_ms(delay);
  double speed = double(count_1 - count_last) / double(delay * 0.001 * 1400); //first computing of the speed to have an idea for the reference speed
  ref = speed/2;
  Serial.print("\n speed: ");
  Serial.println(speed);
  Serial.print("\n ref: ");
  Serial.println(ref);
  int i =0;
  while(i<100){
    ref = 0;
    int delay = 2; //2ms to respect update rate (350Hz)
    _delay_ms(delay);
    
    double speed = double(count - count_last) / double(delay * 0.001 * 1400); //speed in rot/s
    actual = speed;
    count_last = count;
    timer += 1;
    // if (speed != 0 && (timer%1000 == 0)){
    //   Serial.print("\n speed: ");
    //   Serial.println(speed);
    // }
    double u =  Contr.update(ref, speed);
    duty_cycle = duty_cycle + static_cast<int>(u);
    AIN2.set(duty_cycle);
    // Serial.print("\n duty cycle: ");
    // Serial.println(duty_cycle);
    // Serial.print("\n correction: ");
    // Serial.println(u);
    // Serial.print("\n actual: ");
    // Serial.println(speed);
    // Serial.print("\n ref: ");
    // Serial.println(ref);
    Serial.print(" ");
    Serial.println(duty_cycle);
    Serial.print(" ");
    Serial.println(u);
    Serial.print(" ");
    Serial.println(speed);
    Serial.print(" ");
    Serial.println(ref);
    i++;
  }
  
  while(1){
    ref = 25;
    int delay = 2; //2ms to respect update rate (350Hz)
    _delay_ms(delay);
    
    double speed = double(count - count_last) / double(delay * 0.001 * 1400); //speed in rot/s
    actual = speed;
    count_last = count;
    timer += 1;
    // if (speed != 0 && (timer%1000 == 0)){
    //   Serial.print("\n speed: ");
    //   Serial.println(speed);
    // }

    double u =  Contr.update(ref, speed);
    duty_cycle = duty_cycle + static_cast<int>(u);
    AIN2.set(duty_cycle);


    Serial.print(" ");
    Serial.println(duty_cycle);
    Serial.print(" ");
    Serial.println(u);
    Serial.print(" ");
    Serial.println(speed);
    Serial.print(" ");
    Serial.println(ref);

  }

  return 0;
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

//PWM
ISR(TIMER1_COMPA_vect)
{
  AIN2.pin.on();
}

ISR(TIMER1_COMPB_vect)
{
  AIN2.pin.off();
}


