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
#include "config.hpp"
#include "H_bridge_controller.hpp"
#include "Chave_fim_de_curso.hpp"
#include <math.h> 
#include "serial_communication.hpp"

class Axis {
    protected:
        Encoder *encoder;
        H_bridge_controller *BTS;
        H_bridge_controller *PWM_constant;
        Chave_fim_de_curso *Chave_R;
        Chave_fim_de_curso *Chave_L;
        PID *Pid;
        int PWM_RESOLUTION;
        double output;
        double setpoint;
        int MAX_PWM = 1;
        float size;
        float tolerance;
        float Max_pos;
        float Min_pos;
        float pwm_cte;
        bool debug;
    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         * @param k_p Proporcional constant 
         * @param k_i Integrative constant 
         * @param k_d Derivative constant 
         */
        Axis(Encoder *encoder, H_bridge_controller *BTS, Chave_fim_de_curso *Chave_R, Chave_fim_de_curso *Chave_L,PID *Pid, float max_vel,int PWM_RESOLUTION,float size,float tolerance, float pwm_cte, bool debug);

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void move();

        /**
         * @brief stop Motors
         * 
         */
        void stop();

        /**
         * @brief reset PID
         * 
         */
        void reset();

        /**
         * @brief set goal in mm
         * 
         */
        void setGoal(double setpoint);

        /**
         * @brief set tolerance from edges
         * 
         */
        void setEnvelope(float tolerance);

        /**
         * @brief get position of axis
         * 
         */
        float position();

        /**
         * @brief get wether the axis is o goal or not
         * 
         */
        bool onGoal();

        /**
         * @brief Go to origin
         * 
         */
        void go_origin();

        /**
         * @brief go to max
         * 
         */
        void go_max();


        


};

#endif  // __AXIS_HPP__