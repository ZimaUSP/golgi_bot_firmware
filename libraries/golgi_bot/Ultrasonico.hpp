/**
 * @file  Ultrasonico.hpp
 *
 * @brief Ultrasonico Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __Ultrasonico_HPP__
#define __Ultrasonico_HPP__

#include "Arduino.h"

class Ultrasonico {
    private:
        int Trig_pin;
        int Echo_pin;
        long duration;
        int distance;

    public:

        /**
         * @brief Default constructor of ultrasonic sensor class
         * 
         * @param Trig_pin trig pin
         * @param Echo_pin echo pin
         *
         */
        Ultrasonico(int Trig_pin,int Echo_pin) ;

        /**
         * @brief Init digitial pins
         */
        void init();

        /**
         * @brief get distance in Centimeters
         */
        int get_distance();
    

};

#endif  // __Ultrasonico_HPP__