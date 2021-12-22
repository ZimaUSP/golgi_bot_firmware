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

Axis::Axis(Encoder *encoder, H_bridge_controller *BTS, Chave_fim_de_curso *Chave_R, Chave_fim_de_curso *Chave_L,PID *Pid, float max_vel,int PWM_RESOLUTION) {
    this->encoder = encoder;
    this->BTS= BTS;
    this->Chave_R= Chave_R;
    this->Chave_L= Chave_L;
    this->Pid= Pid;
    this->MAX_PWM = (pow(2,PWM_RESOLUTION)-1)*max_vel;
}
void Axis::setGoal(double setpoint){
  this->setpoint=setpoint;
}
void Axis::move(){
  output=this->Pid->computePID(this->encoder->getPosition(),this->setpoint);
  if (output < 0) {
        if (output < -this->MAX_PWM) {
          output = -this->MAX_PWM;
        }
        this->BTS->Set_R(-output);
        return;
      } else {
        if (output > this->MAX_PWM) {
          output = this->MAX_PWM;
        }
        this->BTS->Set_L(output);
        return;
      }
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