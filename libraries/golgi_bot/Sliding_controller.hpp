/**
 * @file  Sliding_controller.cpp
 *
 * @brief Sliding Mode Controler Class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 01/2025
 * 
 * @copyright MIT License
 */
#ifndef __SLIDING_CONTROLLER_HPP__
#define __SLIDING_CONTROLLER_HPP__

#include "Arduino.h"
#include <array>

class Sliding_controller {
    private:
        double K;         
        double Kt;        
        double M;         
        double J;         
        double Kv;        
        double Kp;        
        double gama;      
        double alpha;     
        double R;   
        double res;
        double sampling_time;
        double Teq;

        std::array<double, 5> prev_position;
        std::array<double, 5> prev_sigma;
        std::array<double, 5> prev_w;
        std::array<double, 5> prev_wder1;        
        std::array<double, 5> prev_vel;        
        double prev_time;
        double prev_time2;
        double prev_Dsigma;
        double prev_output;
        double prev_acel;
        double prev_teq;

    public:
        /**
         * @brief Default constructor of a Sliding Mode Controler base class
         * 
         * @param K              Elasticity Coefficient
         * @param Kt             Torque Constant
         * @param M              Load Mass
         * @param J              Load Inercia (approximately)
         * @param Kv             Control Design Parameters
         * @param Kp             Control Design Parameters
         * @param gama           Control Design Parameters
         * @param alpha          Control Design Parameters
         * @param R              Pulleys Radius
         * @param res            Motor Resistence
         * @param sampling_time  Sampling Time
         */
        Sliding_controller(double K, double Kt, double M, double J, double Kv, double Kp, double gama, double alpha, double R, double res, double sampling_time);

        /**
         * @brief Default constructor of a incremental PID base class
         * 
         * @param input    current position
         * @param setpoint desired position
         */
        double Compute_PWM_Output(double input, double setpoint);

        void reset_SMC_controller();

        double getTeq();

        double getOutputPWM();

        double getVelocity();

        double getAcel();

};

# endif // __SLIDING_CONTROLLER_HPP__