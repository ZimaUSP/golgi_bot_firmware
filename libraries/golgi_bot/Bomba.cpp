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
    pinMode(this->pin,OUTPUT);
    digitalWrite(this->pin, LOW);
}

void Bomba::turn_on() {
  digitalWrite(this->pin, HIGH);
}

void Bomba::turn_off() {
  digitalWrite(this->pin, LOW);
}


