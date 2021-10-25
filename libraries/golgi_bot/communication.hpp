/**
 * @file  communication.hpp
 *
 * @brief communication class base
 *
 * @author Vanderson Santos <vanderson.santos@usp.br>
 *
 * @date 08/2021
 * 
 * @copyright MIT License
 */

#ifndef __COMMUNICATION_HPP__
#define __COMMUNICATION_HPP__

#include <iostream>
#include <string>
#include <stdlib.h>

class Communication {
    protected:
        std::string name;
        std::string received_data;
        std::string sent_data;

    public:
        /**
         * @brief Default constructor of a Communication base class
         * 
         * @param name communication name 
         */
        Communication(std::string name);

        /**
         * @brief Default desconstrutor of a Communication base class
         */
        ~Communication();

        /**
         * @brief get the communication name
         * 
         * @return return the communication name
         */
        std::string get_name();

        /**
         * @brief get the received data in the communication
         * 
         * @return get the variable "received_data"
         */
        std::string get_received_data();

         /**
         * @brief get the sent data in the communication
         * 
         * @return get the variable "sent_data"
         */
        std::string get_sent_data();

        /**
         * @brief virtual function to update the value of the variable "received_data" with the read data
         */
        virtual void read_data() = 0;

        /**
         * @brief virtual function to send data
         * 
         * @param data data that will be sent
         * 
         * @return true if it could be sent and false if its not
         */
        virtual bool send_data(std::string data) = 0;
};

#endif  // __COMMUNICATION_HPP__