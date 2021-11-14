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

Atuador::Atuador(int Extend_pin,int Contract_pin) {
    this->Extend_pin = Extend_pin;
    this->Contract_pin = Contract_pin;
}
void Atuador::init(){
    pinMode(this->Extend_pin,OUTPUT);
    pinMode(this->Contract_pin,OUTPUT);

}

void Atuador::Extend() {
  digitalWrite(this->Contract_pin, LOW);
  digitalWrite(this->Extend_pin, HIGH);
}

void Atuador::Contract() {
  digitalWrite(this->Extend_pin, LOW);
  digitalWrite(this->Contract_pin, HIGH);
}

void Atuador::Stop() {
  digitalWrite(this->Extend_pin, LOW);
  digitalWrite(this->Contract_pin, LOW);
}

