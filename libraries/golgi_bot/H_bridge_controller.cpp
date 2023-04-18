/**
 * @file  H_bridge_controller.cpp
 *
 * @brief H_bridge_controller class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "H_bridge_controller.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

H_bridge_controller::H_bridge_controller(int R_pin, int L_pin, int PWM_frequency, int PWM_resolution, int R_channel, int L_channel) {
    this-> R_pin = R_pin;
    this-> L_pin = L_pin;
    this-> PWM_frequency = PWM_frequency;
    this-> PWM_resolution = PWM_resolution;
    this-> R_channel = R_channel;
    this-> L_channel = L_channel;

}
void H_bridge_controller::init(){
  ledcAttachPin(R_pin, this->R_channel);
  ledcSetup(this->R_channel, PWM_frequency, PWM_resolution);

  ledcAttachPin(L_pin, this->L_channel);
  ledcSetup(this->L_channel, PWM_frequency, PWM_resolution);
}

void H_bridge_controller::SetPWM_R(int PWM) {
  ledcWrite(this->R_channel, PWM);
}

void H_bridge_controller::SetPWM_L(int PWM) {
  ledcWrite(this->L_channel, PWM);
}

void H_bridge_controller::Set_R(int PWM) { //ideal  colocar Set_up para eixo Z
  ledcWrite(this->L_channel, 0);
  ledcWrite(this->R_channel, PWM);
}

void H_bridge_controller::Set_L(int PWM) {  //ideal colocar Set_down para eixo Z
  ledcWrite(this->R_channel, 0);
  ledcWrite(this->L_channel, PWM);
}



