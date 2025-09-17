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

class Digital_in_portd {
    public:
    Digital_in_portd(uint8_t port_d) {
        pinmaskd = (1 <<port_d);
    }
    void init() {
        DDRD &= ~(1 << DDD2); // set the PD2 pin as input
        PORTD |= (1 << PORTD2); // enable pull-up resistor on PD2
        EICRA |= (1 << ISC00); //select event sensitivity : here if any logic changes
        EIMSK |= (1 << INT0); // Turns on INT0

    }
    bool is_hi() const {
        return (PIND & pinmaskd) != 0;
    }
    bool is_lo() const { 
        return !is_hi(); 
    }
    private:
        uint8_t pinmaskd;
};


// class Encoder{
//     public:
//     Encoder(uint8_t port_d, uint8_t port_b);


//     void init() {
//         A.init();
//         B.init();
//         count = 0;

//     }


//     private:
//         Digital_in_portd A;
//         Digital_in B;
//         int count;


// }
