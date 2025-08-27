// #include <avr/io.h>
// #include <util/delay.h>

// int main()
// {
//  // Initialize all task-specific resources.​
// DDRB |= (1<<5); // setting bit 5 as an output 
//  while (1) // Repeat indefinitely.​
//  {
//    // Wait for the event to happen.​
//     _delay_ms ( 250 );
    
//     PORTB ^= (1<<5); // toggling bit 5

//    // Perform the work for this task.

//  }

//  return 0;
// }










// #include <util/delay.h>
// #include <digital_out.h>

// int main()
// {
//   Digital_out led(5);   // PB5 Arduino Nano built-in LED 
  
//   led.init();

//   while (1)
//   {
    
//     _delay_ms(1000);
//     // led.set_hi();
//     // _delay_ms(2000);
//     // led.set_lo();
//     led.toggle();

//   }

//   return 0;
// }


// #include <util/delay.h>
// #include <digital_out.h>
// #include <digital_in.h>

// int main()
// {
//   Digital_out led(5);   // PB5 Arduino Nano built-in LED 
//   Digital_in button(5);
  
//   led.init();
//   button.init();


//   while (1)
//   {
    
//     _delay_ms(1000);
//     if (button.is_hi()){
//       led.set_lo();
//     }
//     else{
//       led.toggle();
//     }


//   }

//   return 0;
// }





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


  while (1)
  {
    
    _delay_ms(1000);
    


  }

  return 0;
}
