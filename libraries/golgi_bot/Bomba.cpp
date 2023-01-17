/**
 * @file  Bomba.cpp
 *
 * @brief Bomba class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Bomba.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Bomba::Bomba(int pin) {
    this->pin = pin; 
}
void Bomba::init(){
    pinMode(this->pin,OUTPUT); // sets the pin as output
    digitalWrite(this->pin, LOW); // initially sets the bomba to off
}

void Bomba::turn_on() {
  digitalWrite(this->pin, HIGH); // sends signal to turn on
}

void Bomba::turn_off() {
  digitalWrite(this->pin, LOW); //sends signal to turn off
}


