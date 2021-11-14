/**
 * @file  Bomba.hpp
 *
 * @brief Bomba Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __BOMBA_HPP__
#define __BOMBA_HPP__

#include "Arduino.h"

class Bomba {
    private:

        int pin;

    public:

        /**
         * @brief Default constructor of a Bomba base class
         * 
         * @param pin Bomba digital pin 
         *
         */
        Bomba(int pin);

        /**
         * @brief Init digitial pin
         */
        void init();
    
        /**
         * @brief Turn on bomba de vácuo
         */
        void turn_on();

        /**
         * @brief Turn on bomba de vácuo
         */
        void turn_off();

};

#endif  // __BOMBA_HPP__