/**
 * @file  Encoder.cpp
 *
 * @brief Encoder class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Encoder.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Encoder::Encoder(int A_pin,int B_pin, byte which):whichISR_(which) {
    this-> A_pin = A_pin;
    this-> B_pin = B_pin;
    pinMode(this->A_pin,INPUT_PULLUP);
    pinMode(this->B_pin,INPUT_PULLUP);
}
void Encoder::A_trigger() {
 if(digitalRead(this->A_pin) == HIGH){
   if (digitalRead(this->B_pin) == LOW) {
    this->pulses++;
  } else {
    this->pulses--;
  }     
 }else{
   if (digitalRead(this->B_pin) == HIGH) {
    this->pulses++;
  } else {
    this->pulses--;
  }     
 } 
}
void Encoder::B_trigger() {
 if(digitalRead(this->B_pin) == HIGH){
   if (digitalRead(this->A_pin) == HIGH) {
    this->pulses++;
  } else {
    this->pulses--;
  }     
 }else{
   if (digitalRead(this->A_pin) == LOW) {
    this->pulses++;
  } else {
    this->pulses--;
  }     
 } 
}
void Encoder::init() {
  switch (whichISR_) {
      case 0: 
        attachInterrupt (this-> A_pin, isr0A, CHANGE); 
        attachInterrupt (this-> B_pin, isr0B, CHANGE); 
        instance0_ = this;
        break;

      case 1: 
        attachInterrupt (this-> A_pin, isr1A, CHANGE); 
        attachInterrupt (this-> B_pin, isr1B, CHANGE); 
        instance1_ = this;
        break;

      case 2:
        attachInterrupt (this-> A_pin, isr2A, CHANGE); 
        attachInterrupt (this-> B_pin, isr2B, CHANGE); 
        instance2_ = this;
        break;
    } 
}

int Encoder::getPulses() {
 return this->pulses;
}

void Encoder::setPulses(int num) {
 this->pulses= num;
}

// for use by ISR glue routines
Encoder * Encoder::instance0_;
Encoder * Encoder::instance1_;
Encoder * Encoder::instance2_;

void Encoder::isr0A (){
  instance0_->handleInterrupt('A');
}

void Encoder::isr0B (){
  instance0_->handleInterrupt('B');
}

void Encoder::isr1A (){
  instance1_->handleInterrupt('A');  
}

void Encoder::isr1B (){
  instance1_->handleInterrupt('B');  
}

void Encoder::isr2A (){
  instance2_->handleInterrupt('A');  
}

void Encoder::isr2B (){
  instance2_->handleInterrupt('B');  
}

void Encoder::handleInterrupt(char sel){
  switch (sel){
    case 'A':
      this->A_trigger();
      break;

    case 'B':
      this->B_trigger();
      break;
  } 
}