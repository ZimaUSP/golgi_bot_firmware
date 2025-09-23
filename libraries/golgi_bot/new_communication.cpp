/**
 * @file  serial_communication.cpp
 *
 * @brief serial communication class 
 *
 * @author Vanderson Santos <vanderson.santos@usp.br>
 *
 * @date 08/2021
 * 
 * @copyright MIT License
 */
#include "new_communication.hpp"

NewCommunication::NewCommunication(Controller *Golgi_bot){
    this -> Golgi_bot = Golgi_bot;
}

void NewCommunication::go_origin(){
    Golgi_bot->go_origin(true, true);
}

void NewCommunication::go_max(){
    Golgi_bot->go_max(true, true, true);
}

void NewCommunication::read_setpoint(String received) {
    String id_remedio, coordinates, mode;
    Serial.println(received);
    mode = received.substring(0, received.indexOf(':'));
    if(mode == "id") {
        id_remedio = received.substring(received.indexOf(':') + 1);
        id_remedio.trim();
        go_remedio(id_remedio); //ainda nao implementado
    }
    else if (mode == "coord") {
        String strX, strZ;
        coordinates = received.substring(received.indexOf(':') + 1);
        strX = coordinates.substring(0, coordinates.indexOf(','));
        strZ = coordinates.substring(coordinates.indexOf(',') + 1);
        double x_pos = strX.toFloat();
        double z_pos = strZ.toFloat();
        go_position(x_pos, z_pos); 
    }
}

void NewCommunication::go_position(double x_pos, double z_pos){
    Golgi_bot->setGoal(x_pos, x_pos, z_pos);
}

void NewCommunication::go_remedio(String id_remedio){
    double x_pos, z_pos;
    //pegar as coordenadas com base no id_remedio
    go_position(x_pos, z_pos);
}
