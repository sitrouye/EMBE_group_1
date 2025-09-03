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
  int state = 0;

  while (1)
  {
    double ts = 7*2*100/15000*60*1000; //maximum sampling rate
    _delay_ms(ts); //check state of the encoder every ts (ts not too big to not miss any change of state of the encoder)
    
    if (state == 1){
      if (encoder_1.is_lo()){
        count += 1;
        state = 0;
      }
    }
    if (state == 0){
      if (encoder_1.is_hi()){
        count += 1;
        state = 1;
      }
    }
  }

    


  

  return 0;
}
