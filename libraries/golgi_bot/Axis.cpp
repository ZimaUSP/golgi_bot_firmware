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

Axis::Axis(Encoder *encoder, H_bridge_controller *BTS, Chave_fim_de_curso *Chave_R, Chave_fim_de_curso *Chave_L,PID *Pid, float max_vel,int PWM_RESOLUTION,float size,float tolerance, float pwm_cte) {
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
    this->size = size;
    this->tolerance = tolerance;
    this->Max_pos = size;
    this->Min_pos = 0;
}
void Axis::setGoal(double setpoint){
  if (setpoint>this->Max_pos){
    this->setpoint=this->Max_pos;

  }else if(setpoint<this->Min_pos){
    this->setpoint=this->Min_pos;

  }else{
    this->setpoint=setpoint;
  } 
}
void Axis::move(){
  output=this->Pid->computePID(this->encoder->getPosition(),this->setpoint,this->tolerance*5);
  if (output < 0) {
        if (output < -(this->MAX_PWM)*this->pwm_cte) {
          output = -(this->MAX_PWM)*this->pwm_cte;
        }
        
        this->BTS->Set_R(-output);
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
  while (digitalRead(Chave_R->getPin())==HIGH){ 
    this->BTS->Set_R((this->MAX_PWM)*this->pwm_cte);
    Serial.println("go origin");
  }
  Serial.println("origin");
  this->encoder->setPulses(0);
  this->stop();
}

void Axis::go_max(){
  while (digitalRead(Chave_L->getPin())==HIGH){ /*/Próxima vez que for testar o robozão ver se desse jeito ele busca o pino certo/*/
    this->BTS->Set_L((this->MAX_PWM)*this->pwm_cte);
    Serial.println("go max");
  }
  Serial.println("max");
  this->encoder->setPulses(0);
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

void postionpertime(){

// go max
while (digitalRead(Chave_L->getPin())==HIGH){ 
    this->BTS->Set_L((this->MAX_PWM)*this->pwm_cte);
    Serial.Println(this->postion());
  }
  Serial.println("max");
  this->encoder->setPulses(0);
  this->stop();

}

// go origin 
while (digitalRead(Chave_R->getPin())==HIGH){ 
    this->BTS->Set_R((this->MAX_PWM)*this->pwm_cte);
    Serial.Println(this->postion());
  }
  Serial.println("origin");
  this->encoder->setPulses(0);
  this->stop();
 return;

}