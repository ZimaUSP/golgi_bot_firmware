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

// Encoder::Encoder(int A_pin,int B_pin, const byte which,int PulsesPerRev,int PitchPerRev,int mode):whichISR_(which) {
//     this-> A_pin = A_pin;
//     this-> B_pin = B_pin;
//     this->PitchPerRev=PitchPerRev;
//     this->mode=mode;
//     if(mode==1){
//       this->PulsesPerRev=PulsesPerRev;
//     }else if(mode==2){
//       this->PulsesPerRev=2*PulsesPerRev;
//     }else if(mode==4){
//       this->PulsesPerRev=4*PulsesPerRev;
//     }
    
//     pinMode(this->A_pin,INPUT_PULLUP);
//     pinMode(this->B_pin,INPUT_PULLUP);
// }

//Tirei o which
Encoder::Encoder(int A_pin,int B_pin, int PulsesPerRev,int PitchPerRev,int mode) {
    this-> A_pin = A_pin;
    this-> B_pin = B_pin;
    this->PitchPerRev=PitchPerRev;
    this->mode=mode;
    // if(mode==1){
    //   this->PulsesPerRev=PulsesPerRev;
    // }else if(mode==2){
    //   this->PulsesPerRev=2*PulsesPerRev;
    // }else if(mode==4){
    //   this->PulsesPerRev=4*PulsesPerRev;
    // }
    //Simplifica, e também geralmente só o modo 4 é útil, mas se quiser mudar esse valor não altera nada aqui
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

// Dos modes agora só o mode == 4 aparece, logo posso resumir apenas a ele. Fiz similar ao chave, já que é o mesmo princípio.
// Talvez deixar de alguma forma para se necessário usar outro "mode", o que acha? 
void Encoder::init() {
  if (this->mode == 4) {
    attachInterruptArg (this->A_pin, isrA, this, CHANGE);
    attachInterruptArg (this->B_pin, isrB, this, CHANGE);
  }
}
// void Encoder::init() {
//   switch (whichISR_) {
//       case 0: 
//         if(this->mode==1){
//           attachInterrupt (this-> A_pin, isr0A, RISING); 
//         }else if(this->mode==2){
//           attachInterrupt (this-> A_pin, isr0A, CHANGE); 
//         }else if(this->mode==4){
//           attachInterrupt (this-> A_pin, isr0A, CHANGE); 
//           attachInterrupt (this-> B_pin, isr0B, CHANGE); 
//         }
        
//         instance0_ = this;
//         break;

//       case 1: 
//         if(this->mode==1){
//           attachInterrupt (this-> A_pin, isr1A, RISING); 
//         }else if(this->mode==2){
//           attachInterrupt (this-> A_pin, isr1A, CHANGE); 
//         }else if(this->mode==4){
//           attachInterrupt (this-> A_pin, isr1A, CHANGE); 
//           attachInterrupt (this-> B_pin, isr1B, CHANGE); 
//         }
        
//         instance1_ = this;
//         break;

//       case 2:
//         if(this->mode==1){
//           attachInterrupt (this-> A_pin, isr2A, RISING); 
//         }else if(this->mode==2){
//           attachInterrupt (this-> A_pin, isr1A, CHANGE); 
//         }else if(this->mode==4){
//           attachInterrupt (this-> A_pin, isr2A, CHANGE); 
//           attachInterrupt (this-> B_pin, isr2B, CHANGE); 
//         }
//         instance2_ = this;
//         break;
//     } 
// }

int Encoder::getPulses() {
 return this->pulses;
}

float Encoder::getPosition() {
 return this->pulses*0.03;
}

void Encoder::setPulses(int num) {
 this->pulses= num;
}

// // for use by ISR glue routines
// Encoder * Encoder::instance0_;
// Encoder * Encoder::instance1_;
// Encoder * Encoder::instance2_;

// void Encoder::isr0A (){
//   instance0_->handleInterrupt('A');
// }

// void Encoder::isr0B (){
//   instance0_->handleInterrupt('B');
// }

// void Encoder::isr1A (){
//   instance1_->handleInterrupt('A');  
// }

// void Encoder::isr1B (){
//   instance1_->handleInterrupt('B');  
// }

// void Encoder::isr2A (){
//   instance2_->handleInterrupt('A');  
// }

// void Encoder::isr2B (){
//   instance2_->handleInterrupt('B');  
// }

// a partir das isr, puxar direto os trigger's ou fazer um handleInterrupt?
void Encoder::isrA(void* arg) {
  Encoder* instance_ = (Encoder*) arg;
  instance_->A_trigger();
}

void Encoder::isrB(void* arg) {
  Encoder* instance_ = (Encoder*) arg;
  instance_->B_trigger();
}
//Será que fazemos o static cast??

// Sefor tirar o handleInterrupt, tem que tirar do .hpp

// Manter esse já que tem que diferenciar os canais, ou puxar logo A_trigger?
// void Encoder::handleInterrupt(char sel){
//   switch (sel){
//     case 'A':
//       this->A_trigger();
//       break;

//     case 'B':
//       this->B_trigger();
//       break;
//   } 
// }