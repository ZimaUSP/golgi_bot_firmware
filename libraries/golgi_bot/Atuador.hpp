/**
 * @file  Atuador.hpp
 *
 * @brief Atuador Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __ATUADOR_HPP__
#define __ATUADOR_HPP__

#include "Arduino.h"

class Atuador {
    private:

        int Extend_pin;
        int Contract_pin;

    public:

        /**
         * @brief Default constructor of a Bomba base class
         * 
         * @param Extend_pin If pin HIGH, atuador Extend stroke
         * @param Contract_pin If pin HIGH, atuador Contract stroke
         *
         */
        Atuador(int Extend_pin,int Contract_pin);

        /**
         * @brief Init digitial pins
         */
        void init();
    
        /**
         * @brief Extend stroke
         */
        void Extend();

        /**
         * @brief Contract stroke
         */
        void Contract();

        /**
         * @brief stop stroke
         */
        void Stop();

};

#endif  // __ATUADOR_HPP__