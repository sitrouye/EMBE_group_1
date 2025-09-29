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

class Digital_in {
    public:
    Digital_in(uint8_t port_b) {
        pinmaskb = (1 <<port_b);
    }
    void init() {
        DDRB &= ~pinmaskb;
        PORTB |= pinmaskb;
    }
    bool is_hi() const {
        return (PINB & pinmaskb) != 0;
    }
    bool is_lo() const { 
        return !is_hi(); }
    private:
        uint8_t pinmaskb;
};

