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

Chave_fim_de_curso::Chave_fim_de_curso(int pin, byte which):whichISR_(which) {
    this-> pin = pin;
    pinMode(this->pin,INPUT_PULLDOWN);
    //attachInterrupt(this->A_pin, this->A_trigger, RISING);
    //attachInterrupt(this->B_pin, this->B_trigger, RISING);
}
void Chave_fim_de_curso::batente_trigger() {
  if (digitalRead(this->pin)==LOW){
    this->batente=true;
  }else{
    this->batente=false;
  }
}

void Chave_fim_de_curso::init() {
  switch (whichISR_) {
      case 0: 
        attachInterrupt (this->pin, isr0, CHANGE); 
        instance0_ = this;
        this->batente_trigger();
        break;

      case 1: 
        attachInterrupt (this->pin, isr1, CHANGE); 
        instance1_ = this;
        this->batente_trigger();
        break;

      case 2:
        attachInterrupt (this->pin, isr2, CHANGE); 
        instance2_ = this;
        this->batente_trigger();
        break;
      
      case 3: 
        attachInterrupt (this->pin, isr3, CHANGE); 
        instance3_ = this;
        this->batente_trigger();
        break;

      case 4: 
        attachInterrupt (this->pin, isr4, CHANGE); 
        instance4_ = this;
        this->batente_trigger();
        break;

      case 5: 
        attachInterrupt (this->pin, isr5, CHANGE); 
        instance5_ = this;
        this->batente_trigger();
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
Chave_fim_de_curso * Chave_fim_de_curso::instance4_;
Chave_fim_de_curso * Chave_fim_de_curso::instance5_;

void Chave_fim_de_curso::isr0 (){
  instance0_->handleInterrupt();
}

void Chave_fim_de_curso::isr1 (){
  instance1_->handleInterrupt();  
}

void Chave_fim_de_curso::isr2 (){
  instance2_->handleInterrupt();  
}

void Chave_fim_de_curso::isr3 (){
  instance3_->handleInterrupt();  
}

void Chave_fim_de_curso::isr4 (){
  instance4_->handleInterrupt();  
}

void Chave_fim_de_curso::isr5 (){
  instance5_->handleInterrupt();  
}

void Chave_fim_de_curso::handleInterrupt(){
    this->batente_trigger();
}