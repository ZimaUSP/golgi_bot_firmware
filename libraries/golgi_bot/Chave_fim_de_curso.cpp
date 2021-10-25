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

Chave_fim_de_curso::Chave_fim_de_curso(int pin):whichISR_(which) {
    this-> pin = pin;
    pinMode(this->pin,INPUT_PULLUP);
    //attachInterrupt(this->A_pin, this->A_trigger, RISING);
    //attachInterrupt(this->B_pin, this->B_trigger, RISING);
}
void Chave_fim_de_curso::batente_trigger() {
  if (digitalRead(this->pin)==LOW){
    this->batente=true;
  }else{
    this->batente=false
  }
}

void Chave_fim_de_curso::init() {
  switch (whichISR_) {
      case 0: 
        attachInterrupt (0, isr0, CHANGE); 
        instance0_ = this;
        break;

      case 1: 
        attachInterrupt (1, isr1, CHANGE); 
        instance1_ = this;
        break;

      case 2:
        attachInterrupt (2, isr2, CHANGE); 
        instance2_ = this;
        break;
      
      case 3: 
        attachInterrupt (0, isr3, CHANGE); 
        instance3_ = this;
        break;
    } 
}

int Chave_fim_de_curso::getPin() {
 return this->pin;
}

bool Chave_fim_de_curso::getBatente() {
 return this->batente;
}

// for use by ISR glue routines
Chave_fim_de_curso * Chave_fim_de_curso::instance0_;
Chave_fim_de_curso * Chave_fim_de_curso::instance1_;
Chave_fim_de_curso * Chave_fim_de_curso::instance2_;
Chave_fim_de_curso * Chave_fim_de_curso::instance3_;

void Encoder::isr0 (){
  instance0_->handleInterrupt();
}

void Encoder::isr1 (){
  instance1_->handleInterrupt();  
}

void Encoder::isr2 (){
  instance2_->handleInterrupt();  
}

void Encoder::isr3 (){
  instance3_->handleInterrupt();  
}

void Encoder::handleInterrupt(){
    this->batente_trigger();
}