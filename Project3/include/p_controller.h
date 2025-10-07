#pragma once
#include <avr/io.h>

class P_controller{
    public : 
    P_controller(double Kp){
        
        corr = Kp;
    }
    
    double update(double ref, double actual){
        double control = corr * (ref - actual); 
        return control;
    }
    private : 
    double corr;
};

