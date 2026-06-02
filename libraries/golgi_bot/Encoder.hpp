/**
 * @file  Encoder.cpp
 *
 * @brief Encoder Class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */

#ifndef __Encoder_HPP__
#define __Encoder_HPP__

#include "Arduino.h"

class Encoder {
    private:
        int A_pin;
        int B_pin;
        int PulsesPerRev;
        int PitchPerRev;
        int pulses=0;
        int mode;
        
        static void isrA(void* arg);
        static void isrB(void* arg);

    public:
        /**
         * @brief Default constructor of a encoder base class
         * 
         * @param A_pin A phase pin 
         * @param B_pin B phase pin 
         */
        Encoder(int A_pin,int B_pin,int PulsesPerRev,int PitchPerRev,int mode);

        void init();
    
        /**
         * @brief Computes A phase pulse
         */
        void A_trigger();

        /**
         * @brief Computes B phase pulse
         */
        void B_trigger();

        
        /**
         * @brief get number of pulses
         */
        int getPulses();

        /**
         * @brief set number of pulses
         * 
         * @param num number of pulses to be set 
         */
        void setPulses(int num);
        
        /**
         * @brief get position in mm
         */
        float getPosition();


};

#endif  // __ENCODER_HPP__