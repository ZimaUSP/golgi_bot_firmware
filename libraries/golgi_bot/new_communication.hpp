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

#ifndef __NEW_COMMUNICATION_HPP
#define __NEW_COMMUNICATION_HPP

#include "Controller.hpp"
#include <Arduino.h>

class NewCommunication {
private:
    Controller *Golgi_bot;

public:
    NewCommunication(Controller *Golgi_bot);
    void go_origin();
    void go_max();
    void read_setpoint(String received);
    void go_position(double x_pos, double z_pos);
    void go_remedio(String id_remedio);

};

#endif //__NEW_COMMUNICATION_HPP
