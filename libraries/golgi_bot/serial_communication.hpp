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
#include "config.hpp"
#include "pins_and_channels.hpp"

#include <Arduino.h>
#include <stdlib.h>

#ifdef ACTIVE_RASPBERRY_COMMUNICATION
enum serial_ports{
    MAIN_SERIAL,
    RASPBERRY_SERIAL,

    SERIAL_AMOUNT
};
#endif
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

        #ifdef ACTIVE_RASPBERRY_COMMUNICATION
            /**
             * @brief send just one byte
             * 
             * @param byte byte that will be sent
             * @param serial_port chosen serial port
             * 
             * @return true if it could be sent and false if its not
             */
            bool send_byte(byte byte,serial_ports serial_port);

            /**
            * @brief send a data with a plenty of bytes
            * 
            * @param data data that will be sent
            * @param serial_port chosen serial port
            * 
            * @return true if it could be sent and false if its not
            */
            bool send_data(std::string data,serial_ports serial_port);

            /**
            * @brief read just one char from a serial communication
            *
            * @param serial_port chosen serial port 
            *  
            * @return returns the char that was read
            */
            char read_char(serial_ports serial_port);

            /**
            * @brief update the variable "received_data" from the mother class
            * 
            * @param serial_port chosen serial port
            */
            void read_data(serial_ports serial_port);

        #endif
};

#endif  // __COMMUNICATION_HPP__