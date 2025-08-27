

#include <util/delay.h>
#include <digital_out.h>
#include <digital_in.h>

int main()
{
  Digital_out led(5);   // PB5 Arduino Nano built-in LED 
  Digital_in encoder_1(1);
  Digital_in encoder_2(2);
  
  led.init();
  encoder_1.init();
  encoder_2.init();
  int count = 0;


  while (1)
  {
    
    _delay_ms(1000);
    if encoder_1.is_hi(){
      count += 1;
    }
    


  }

  return 0;
}
