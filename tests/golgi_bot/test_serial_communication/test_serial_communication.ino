/**
 * @file  test_serial_communication.ino
 *
 * @brief test to serial communication class
 *
 * @author Vanderson Santos <vanderson.santos@usp.br>
 *
 * @date 08/2021
 * 
 * @copyright MIT License
 */

#include "serial_communication.hpp"
#include "config.hpp"
#include <cstring>

SerialCommunication *comu;

void setup() {
    Serial.begin(SERIAL_VEL);
    comu = new SerialCommunication("Posição SetPoint:");
};

void loop() {
    std::cout<< comu->get_name() <<std::endl;

    comu->read_data();
    std::cout<<comu->get_received_data()<<std::endl;
    double setPoint=atoi(string_to_char(comu->get_received_data()));
    std::cout<<setPoint<<std::endl;
    delay(1);
};
char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}