#pragma once
#include <avr/io.h>

class Digital_out {
public:
    Digital_out(uint8_t port_no) {
        pin_mask = (1 << port_no);
    }
    void init() {
        DDRB |= pin_mask;
    }
    void on() {
        PORTB |= pin_mask;
    }
    void off() {
        PORTB &= ~pin_mask;
    }
    void toggle() {
        PORTB ^= pin_mask;
    }
private:
    uint8_t pin_mask;
};
