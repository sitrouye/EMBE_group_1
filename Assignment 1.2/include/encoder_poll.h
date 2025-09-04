#pragma once
#include <digital.h>

class Encoder_Poll {
    public:
        Encoder_Poll(Digital_in &pinA, Digital_in &pinB, Digital_out &led):
            inputA(pinA), inputB(pinB), led(led) {}
       
        
        void begin() {
            inputA.init();
            inputB.init();
            led.init();
            lastState_A = inputA.is_hi();
            position_count = 0;
        }

        void update() {
            bool currentState_A = inputA.is_hi(); // reads current state of channel A
            if (currentState_A != lastState_A) { // Check if state has changed since last time- edge happened?
                if (!lastState_A && currentState_A) { // 
                    if (inputB.is_lo())
                        position_count++; // one direction - B is low at moment of A rising edge
                    else
                        position_count--; // otherwise - other direction
                }
            }
            lastState_A = currentState_A; //updates memory of A

        }

        int position() {
            return position_count; // returns current position
        }
        void reset() {
            position_count = 0; // resets position
        }    

    private:
        // references to the inputs. Store them so update() can access them
        Digital_in &inputA; 
        Digital_in &inputB; 
        Digital_out &led;
        bool lastState_A; 
        int position_count;

};