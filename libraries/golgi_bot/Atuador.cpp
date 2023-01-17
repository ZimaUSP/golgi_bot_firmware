/**
 * @file  Atuador.cpp
 *
 * @brief Atuador class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Atuador.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Atuador::Atuador(int extend_pin,int contract_pin) {
    this->extend_pin = extend_pin;
    this->contract_pin = contract_pin; //teste
}
void Atuador::init(){
    pinMode(this->extend_pin,OUTPUT);
    pinMode(this->contract_pin,OUTPUT);

}

void Atuador::Extend() {
  digitalWrite(this->contract_pin, LOW);
  digitalWrite(this->extend_pin, HIGH);
}

void Atuador::Contract() {
  digitalWrite(this->extend_pin, LOW);
  digitalWrite(this->contract_pin, HIGH);
}

void Atuador::Stop() {
  digitalWrite(this->extend_pin, LOW);
  digitalWrite(this->contract_pin, LOW);
}

