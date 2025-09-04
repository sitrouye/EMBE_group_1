// #pragma once


// #include <stdint.h>
// #include <avr/io.h>

// class Digital_out
// {
//     public :
//         Digital_out(int port_nb){
//             pin_mask = (1 << port_nb );
//         } //constructor
//         void init(){
//             DDRB |= pin_mask;

//         }
//         void set_hi(){
//             PORTB |= pin_mask;

//         }
//         void set_lo(){
//             PORTB &= ~pin_mask;

//         }
//         void toggle(){
//             PORTB ^= pin_mask;

//         }

//     private:
//         int8_t pin_mask; //private so that nobody can fiddle with it
// };

