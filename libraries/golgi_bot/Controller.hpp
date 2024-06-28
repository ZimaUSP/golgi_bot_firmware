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


#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "Axis.hpp"
#include "Bomba.hpp"
#include "Atuador.hpp"

class Controller {
    protected:
        Axis *Axis_1;
        Axis *Axis_2;
        Axis *Axis_3;
        Bomba *Bomba_Y;
        Atuador *Atuador_Y;
        float position[3];
    public:
        /**
         * @brief Default constructor of a PID base class
         * 
         */
        Controller(Axis *Axis_1, Axis *Axis_2, Axis *Axis_3, Bomba *Bomba_Y, Atuador *Atuador_Y);

        /**
         * @brief set goal in mm
         * 
         */
        void setGoal(double goal_axis_1, double goal_axis_2, double goal_axis_3);

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void move();

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
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
         */
        void stop(bool axis1, bool axis2, bool axis3);

        /**
         * @brief reset both PID
         * 
         */
        void reset_PID();

        /**
         * @brief moves axis with PID control, input position from enconder(mm)
         * 
         */
        void reset_Y(int DELAY_CON);


        /**
         * @brief set tolerance from edges
         * 
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
         * @brief Go to origin on true axis
         * 
         */
        void go_origin(bool axis1,bool axis2);

        /**
         * @brief go to max on true axis
         * 
         */
        void go_max(bool axis1,bool axis2, bool axis3);

};

#endif  // _CONTROLLER_HPP_