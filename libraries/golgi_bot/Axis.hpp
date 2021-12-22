/**
 * @file  Axis.cpp
 *
 * @brief Axis class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 12/2021
 * 
 * @copyright MIT License
 */


#ifndef __AXIS_HPP__
#define __AXIS_HPP__

#include "PID.hpp"
#include "Encoder.hpp"
#include "H_bridge_controller.hpp"
#include "Chave_fim_de_curso.hpp"
#include <math.h> 

class Axis {
    protected:
        Encoder *encoder;
        H_bridge_controller *BTS;
        Chave_fim_de_curso *Chave_R;
        Chave_fim_de_curso *Chave_L;
        PID *Pid;
        int PWM_RESOLUTION;
        double output;
        double setpoint;
        int MAX_PWM;
    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         * @param k_p Proporcional constant 
         * @param k_i Integrative constant 
         * @param k_d Derivative constant 
         */
        Axis(Encoder *encoder, H_bridge_controller *BTS, Chave_fim_de_curso *Chave_R, Chave_fim_de_curso *Chave_L,PID *Pid, float max_vel,int PWM_RESOLUTION);

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void move();

        /**
         * @brief stop bts and reset PID
         * 
         */
        void stop();
        void reset();

        void setGoal(double setpoint);
        float position();
};

#endif  // __AXIS_HPP__