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

#include "config.hpp"
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