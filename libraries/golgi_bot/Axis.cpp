/**
 * @file  Axis.cpp
 *
 * @brief Axis class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 12/2021
 * 
 * @copyright MIT License
 */

#include "Axis.hpp"


/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Axis::Axis(Encoder *encoder, H_bridge_controller *BTS, Chave_fim_de_curso *Chave_R, Chave_fim_de_curso *Chave_L,PID *Pid, float max_vel,int PWM_RESOLUTION,float tolerance, float pwm_cte, bool debug) {
    this->encoder = encoder;
    this->BTS= BTS;
    this->Chave_R= Chave_R;
    this->Chave_L= Chave_L;
    this->Pid= Pid;
    for(int i=0;i<PWM_RESOLUTION;i++){
      MAX_PWM=MAX_PWM*2;
    }
    this->MAX_PWM = (MAX_PWM-1);
    this->pwm_cte = pwm_cte;
    this->tolerance = tolerance;
    this->Max_pos;
    this->Min_pos = 0;
    this->debug = debug;
}
void Axis::setGoal(double setpoint){
  if (setpoint > this->Max_pos){
    this->setpoint=this->Max_pos;
  }else if(setpoint < this->Min_pos){
    this->setpoint=this->Min_pos;
  }else{
    this->setpoint=setpoint;
  }
  if (this->debug)
    {
      Serial.print("setpoint:");
      Serial.println(setpoint);
    }
}

void Axis::setPoint(double setpoint) {
  this->setpoint = setpoint;
}

void Axis::move(){
  
    Serial.print("encoder position");
    Serial.println(this->encoder->getPosition());
    Serial.print("setpoint");
    Serial.println(this->setpoint);
    Serial.print("PWM:");
    Serial.println(this->output);
  
  
  this->output=-(this->Pid->computePID(this->encoder->getPosition(),this->setpoint,this->tolerance*5));
  Serial.print("output");
  Serial.println(this->output);
  if (this->debug)
    {
      Serial.print("output:");
      Serial.println(output);
    }
  if (output < 0) {
    if (output < -(this->MAX_PWM)*this->pwm_cte) {
      output = -(this->MAX_PWM)*this->pwm_cte;
    } 
    this->output = -output;
    this->BTS->Set_R(output);
    
    return;
  } else {
    
    if (output > (this->MAX_PWM)*this->pwm_cte) {
      output = (this->MAX_PWM)*this->pwm_cte;
    }
    this->BTS->Set_L(output);
    
    return;
  }
}

void Axis::go_origin(){
  while (onOrigin() == LOW){ 
    go_R();
    if (this->debug)
    {
      Serial.print("position to origin");
      Serial.println(encoder->getPosition());
    }
  }
  if (debug){
    Serial.println("origin");
  }
  resetOrigin();
  this->stop();
}

void Axis::go_max(){
  while (onMax() == LOW){ /*/Próxima vez que for testar o robozão ver se desse jeito ele busca o pino certo/*/
    go_L();
    if (this->debug)
    {
      Serial.print("position to max:");
      Serial.println(encoder->getPosition());
    }
  }
   if (debug){
    Serial.println("max");
  }
  resetMax();
  Serial.println(this->Max_pos);
  this->stop();
}

void Axis::stop(){
   this->BTS->Set_R(0);
}

void Axis::reset(){
   this->Pid->reset();
}

float Axis::position(){
  return this->encoder->getPosition();
}

void Axis::setEnvelope(float tolerance){
   this->Max_pos= this->size - tolerance ;
   this->Min_pos= this->size + tolerance ;
}

bool Axis::onGoal(){
  if((this->position()>this->setpoint - this->tolerance) && (this->position()<this->setpoint + this->tolerance)){
    return true;
  }else{
    return false;
  }
}

bool Axis::onOrigin() {
  return digitalRead(Chave_R->getPin()) == LOW;
}

bool Axis::onMax() {
  return digitalRead(Chave_L->getPin()) == LOW;
}

void Axis::resetOrigin() {
  this->encoder->setPulses(0);
}

void Axis::resetMax() {
  this->Max_pos = this->encoder->getPosition();
}
void Axis::setMax(int max){
  this->Max_pos = max;
}

void Axis::go_R() {
    this->BTS->Set_R((this->MAX_PWM)*this->pwm_cte);
}

void Axis::go_L() {
  this->BTS->Set_L((this->MAX_PWM)*this->pwm_cte);
}

double Axis::getOutput() {
  return this->output;
}
