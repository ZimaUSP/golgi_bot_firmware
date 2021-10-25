/**
 * @file  serial_communication.hpp
 *
 * @brief serial communication class 
 *
 * @author Vanderson Santos <vanderson.santos@usp.br>
 *
 * @date 08/2021
 * 
 * @copyright MIT License
 */

#ifndef __SERIAL_COMMUNICATION_HPP__
#define __SERIAL_COMMUNICATION_HPP__

#include <iostream>
#include <string>
#include "communication.hpp"

#include <Arduino.h>
#include <stdlib.h>

class SerialCommunication: public Communication {
    public:
        /**
         * @brief Default constructor of a new Serial Communication class
         * 
         * @param name serial communication name 
         */
        SerialCommunication(std::string name);

        /**
         * @brief Default desconstrutor of a new serial communication class
         */
        ~SerialCommunication();

        /**
         * @brief send just one byte
         * 
         * @param byte byte that will be sent
         * 
         * @return true if it could be sent and false if its not
         */
        bool send_byte(byte byte);

        /**
         * @brief send a data with a plenty of bytes
         * 
         * @param data data that will be sent
         * 
         * @return true if it could be sent and false if its not
         */
        bool send_data(std::string data);

        /**
         * @brief read just one char from a serial communication
         * 
         * @return returns the char that was read
         */
        char read_char();

        /**
         * @brief update the variable "received_data" from the mother class
         */
        void read_data();
};

#endif  // __COMMUNICATION_HPP__