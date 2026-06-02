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

Encoder::Encoder(int A_pin,int B_pin, int PulsesPerRev,int PitchPerRev,int mode) {
    this-> A_pin = A_pin;
    this-> B_pin = B_pin;
    this->PitchPerRev=PitchPerRev;
    this->mode=mode;

    this->PulsesPerRev = PulsesPerRev*mode;

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
  if (this->mode == 4) {
    attachInterruptArg (this->A_pin, isrA, this, CHANGE);
    attachInterruptArg (this->B_pin, isrB, this, CHANGE);
  }
}

int Encoder::getPulses() {
 return this->pulses;
}

float Encoder::getPosition() {
 return this->pulses*0.03;
}

void Encoder::setPulses(int num) {
 this->pulses= num;
}

void Encoder::isrA(void* arg) {
  Encoder* instance_ = (Encoder*) arg;
  instance_->A_trigger();
}

void Encoder::isrB(void* arg) {
  Encoder* instance_ = (Encoder*) arg;
  instance_->B_trigger();
}