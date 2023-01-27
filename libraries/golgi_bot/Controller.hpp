/**
 * @file  Controller.cpp
 *
 * @brief Controller class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 12/2021
 * 
 * @copyright MIT License
 */


#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

#include "Axis.hpp"
#include "Bomba.hpp"
#include "Atuador.hpp"

class Controller {
    protected:
        Axis *Axis_1;
        Axis *Axis_2;
        Bomba *Bomba_Y;
        Atuador *Atuador_Y;
        float position[2];
    public:
        /**
         * @brief Construct a new Controller object of a PID base class
         * 
         * @param Axis_1 Axis_X object pointer
         * @param Axis_2 Axis_Z object pointer 
         * @param Bomba_Y Bomb object pointer
         * @param Atuador_Y actuator object pointer 
         */
        Controller(Axis *Axis_1, Axis *Axis_2, Bomba *Bomba_Y, Atuador *Atuador_Y);

        /**
        * @brief Define the setpoint in milimeters
        * 
        * @param goal_axis_1 Setpoint Axis_X
        * @param goal_axis_2 Setpoint Axis_Z
        */
        void setGoal(double goal_axis_1,double goal_axis_2);

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void move();

        /**
         * @brief Moves axis with PID control, input position from enconder(mm)
         * 
         * @param DELAY_EX Delay to extend actuator
         * @param DELAY_CON Delay to contract actuator
         */
        void get_medicine(int DELAY_EX, int DELAY_CON);

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void drop_medicine();

        /**
         * @brief stop Motors
         * 
         * @param axis1 Boolean to Axis_X stop
         * @param axis2 Boolean to Axis_Z stop
         */
        void stop(bool axis1,bool axis2);

        /**
         * @brief reset both PID
         * 
         */
        void reset_PID();

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         * @param DELAY_CON Delay to contract actuator
         * 
         */
        void reset_Y(int DELAY_CON);


        /**
         * @brief set tolerance from edges
         * 
         * @param tolerance1 Tolerance Axis_X
         * @param tolerance2 Tolerance Axis_Z
         */
        void setEnvelope(float tolerance1,float tolerance2);

        /**
         * @brief get position of axis
         * 
         */
        float* positionPoint();

        /**
         * @brief get wether both axis is on goal or not
         * 
         */
        bool onGoal();
        

        /**
         * @brief Send the Axis to origin 
         * 
         * @param axis1 Boolean to send Axis_X to origin
         * @param axis2 Boolean to send Axis_Z to origin
         */
        void go_origin(bool axis1,bool axis2);

        /**
         * @brief Send the Axis to max 
         * 
         * @param axis1 Boolean to send Axis_X to max
         * @param axis2 Boolean to send Axis_Z to max
         */
        void go_max(bool axis1,bool axis2);

};

#endif  // __CONTROLLER_HPP__