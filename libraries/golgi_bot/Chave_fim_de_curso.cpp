/**
 * @file  Chave_fim_de_curso.cpp
 *
 * @brief Chave_fim_de_curso class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Chave_fim_de_curso.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/
Chave_fim_de_curso::Chave_fim_de_curso(int pin, bool inverso) {
    this-> pin = pin;
    this-> inverso = inverso;
    pinMode(this->pin,INPUT_PULLUP);
    //attachInterrupt(this->A_pin, this->A_trigger, RISING);
    //attachInterrupt(this->B_pin, this->B_trigger, RISING);
}

void Chave_fim_de_curso::batente_trigger() {
  if (this->inverso ){
    this->batente = (digitalRead(this->pin)==HIGH);
  }else{
    this->batente = (digitalRead(this->pin)==LOW);
  }
}

void Chave_fim_de_curso::init() {
  attachInterruptArg (this->pin, isr, this, CHANGE);
  this->batente_trigger();
}

int Chave_fim_de_curso::getPin() {
 return this->pin;
}

bool Chave_fim_de_curso::getBatente() {
 return this->batente;
}

void IRAM_ATTR Chave_fim_de_curso::isr(void* arg) {
  Chave_fim_de_curso* instance_ = (Chave_fim_de_curso*) arg;
  instance_->batente_trigger();
}
