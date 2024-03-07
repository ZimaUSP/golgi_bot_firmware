/**
 * @file  PID.cpp
 *
 * @brief PID Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __PID_HPP__
#define __PID_HPP__

#include "Arduino.h"

class PID {
    protected:

        unsigned long current_time;
        unsigned long previus_time = 0;
        double delta_time;

        
        double i_error;
        double d_error;

        double k_p;
        double k_i;
        double k_d;

        double error;
        double previus_error;

        int i_saturation;
    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         * @param k_p Proporcional constant 
         * @param k_i Integrative constant 
         * @param k_d Derivative constant 
         */
        PID(double k_p,double k_i,double k_d,int i_saturation);

        /**
         * @brief Computes PID value
         * 
         * @param Input input value 
         * @param setpoint Set goal, the value that input should be
         */
        double computePID(double input,float setpoint,float tolerance); 

        /**
         * @brief Reset PID values
         * 
         */
        void reset();


};

#endif  // __PID_HPP__