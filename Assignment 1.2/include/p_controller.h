#pragma once
#include <avr/io.h>

class P_controller{
    public : 
    P_controller(double Kp){
        
        corr = Kp;
    }
    
    double update(double ref, double actual){
        double control = corr * (ref - actual); //divide by maximum speed to have a duty cycle between 0 and 100
        return control;
    }
    private : 
    double corr;
};

