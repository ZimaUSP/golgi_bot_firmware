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

#include "serial_communication.hpp"

#include <iostream>

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

SerialCommunication::SerialCommunication(std::string name):Communication(name){};

SerialCommunication::~SerialCommunication(){
    std::cout <<"Serial Communication "<<this->get_name()<< " Destruido" << std::endl;
};

bool SerialCommunication::send_byte(byte byte) {
    Serial.write(byte);
    return true;
};

void SerialCommunication::read_data(){
    char inData[max_string_length]; 
    int i = 0;
    char c;
    if (Serial.available()){
        c = this->read_char();
        while(c != end_char){
            if(i >= max_string_length)
                break;

            inData[i] = c; 
            i++;
            c = this->read_char();
        }
        inData[i] = '\0';
        this->received_data = inData;
    }
    else{
        this->received_data = '\0';
    }

};

char SerialCommunication::read_char(){
  while(!Serial.available());
  return Serial.read();
};

bool SerialCommunication::send_data(std::string data){
    byte c = data[0];
    int i = 0;
    while(c != end_char){
        if(i >= max_string_length)
            return false;

        c = data[i];
        this->send_byte(c);

        i++;
    }
    return true;
};

#ifdef ACTIVE_DRIVER_COMMUNICATION
bool SerialCommunication::send_byte(byte byte,serial_ports serial_port) {
    switch(serial_port){
        case MAIN_SERIAL:
            Serial.write(byte);
            break;
        case DRIVER_SERIAL:
            Serial2.write(byte);
            break;
        default:
            std::cout <<"Porta serial não encontrada"<< std::endl;
            break;
    };
    return true;
};

bool SerialCommunication::send_data(std::string data,serial_ports serial_port){
    byte c = data[0];
    int i = 0;
    while(c != end_char){
        if(i >= max_string_length)
            return false;

        c = data[i];
        this->send_byte(c, serial_port);

        i++;
    }
    return true;
};


char SerialCommunication::read_char(serial_ports serial_port){
    switch(serial_port){
            case MAIN_SERIAL:
                while(!Serial.available());
                return Serial.read();
                break;
            case DRIVER_SERIAL:
                while(!Serial2.available());
                return Serial2.read();
                break;
            default:
                std::cout <<"Porta serial não encontrada"<< std::endl;
                break;
    };
};

void SerialCommunication::read_data(serial_ports serial_port){
    char inData[max_string_length];
    char inData_error[max_string_length];
    bool error = false;
     
    int i = 0;
    char c;
    bool serialAvaiable; 

    switch(serial_port){
        case MAIN_SERIAL:
            serialAvaiable = Serial.available();
            break;
        case DRIVER_SERIAL:
            serialAvaiable = Serial2.available();
            break;
        default:
            serialAvaiable = false; 
            std::cout <<"Porta serial não disponível"<< std::endl;
            break;
    };

    if (serialAvaiable){
        c = this->read_char(serial_port);
        while(c != end_char){
            if(i >= max_string_length){
                error = true;
                break;
            }
            inData[i] = c; 
            i++;
            c = this->read_char(serial_port);
        }
        if(error){
            this->received_data = '\0';     
        }
        else {
            inData[i] = '\0';
            this->received_data = inData;
        }
    }
    else{
        this->received_data = '\0';
    }

};
#endif