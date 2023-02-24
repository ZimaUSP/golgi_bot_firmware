/**
 * @file  communication.cpp
 *
 * @brief communication with serial
 *
 * @author Vanderson Santos <vanderson.santos@usp.br>
 *
 * @date 08/2021
 * 
 * @copyright MIT License
 */

#include "config.hpp"
#include "communication.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Communication::Communication(std::string name) {
    this->name = name;
    this->received_data = "0";
    this->sent_data = "0";
}

Communication::~Communication() {
    std::cout <<"Communication "<<this->get_name()<< " Destruido" << std::endl;
}

std::string Communication::get_name() {
    return this->name;
}

std::string Communication::get_received_data() {
    return this->received_data;
}

std::string Communication::get_sent_data() {
    return this->sent_data;
}