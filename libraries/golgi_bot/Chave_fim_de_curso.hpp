/**
 * @file  Chave_fim_de_curso.cpp
 *
 * @brief Chave_fim_de_curso Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __Chave_fim_de_curso_HPP__
#define __Chave_fim_de_curso_HPP__

#include "Arduino.h"

class Chave_fim_de_curso {
    private:
        int pin;

        bool batente;
        
        //nickgammon ideia
        const byte whichISR_;

        static void isr0();

        static void isr1();
   
        static void isr2();

        static void isr3();

        static void isr4();

        static void isr5();
     
        static Chave_fim_de_curso* instance0_;
        static Chave_fim_de_curso* instance1_;
        static Chave_fim_de_curso* instance2_;
        static Chave_fim_de_curso* instance3_;
        static Chave_fim_de_curso* instance4_;
        static Chave_fim_de_curso* instance5_;

        void handleInterrupt();
        //

    public:
        /**
         * @brief Default constructor of a Chave_fim_de_curso base class
         * 
         * @param pin chave pin 
         */
        Chave_fim_de_curso(int pin, const byte which);

        void init();
    
        /**
         * @brief Triggered by falling sign, demarks endstop in a bool variable
         */
        void batente_trigger();
        
        /**
         * @brief get the pin number
         */
        int getPin();

        /**
         * @brief get state of movement
         */
        bool getBatente();


};

#endif  // __Chave_fim_de_curso_HPP__