
#pragma once
#include <stdint.h>
#include <avr/io.h>


class Digital_in
{
    public :
        Digital_in(int port_nb){
            pin_mask = (1 << port_nb );
        } //constructor
    void init();

    void set_hi();

    void set_lo();

    void toggle();
    bool is_lo();
    bool is_hi();

    
    private:
        int8_t pin_mask; //private so that nobody can fiddle with it
};

