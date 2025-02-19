/**
 * @file  PID_incremental.cpp
 *
 * @brief PID_incremental class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 11/2024
 * 
 * @copyright MIT License
 */

#ifndef __PID_INCREMENTAL_HPP__
#define __PID_INCREMENTAL_HPP__

#include "Arduino.h"

class PID_incremental {
    protected:

        unsigned long current_time;
        unsigned long previous_time = 0;
        double delta_time;

        double N;
        double K_P;
        double T_I;
        double T_D;
        double T_s;

        double error;
        double previous_error1;
        double previous_error2;

        double delta_u;
        double previous_u;
        double previous_delta_ud;

    public:
        /**
         * @brief Default constructor of a incremental PID base class
         * 
         * @param N   filter coefficient
         * @param k_p Proporcional constant 
         * @param t_i Integrative constant 
         * @param t_d Derivative constant 
         * @param t_s sampling time
         */
        PID_incremental(double N, double k_p, double t_i, double t_d, double t_s);

        /**
         * @brief Computes PID value
         * 
         * @param Input input value 
         * @param setpoint Set goal, the value that input should be
         */
        double computePID_incremental(double input, float setpoint, double tolerance); 

        /**
         * @brief Reset PID values
         * 
         */
        void reset_incremental();


};

#endif  // __PID_INCREMENTAL_HPP__