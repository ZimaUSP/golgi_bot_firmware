/**
 * @file  PID.cpp
 *
 * @brief PID class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "PID.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

PID::PID(double k_p,double k_i,double k_d) {
    this->k_p = k_p;
    this->k_i = k_i;
    this->k_d = k_d; 
}

double PID::computePID(double input,float setpoint) {
  this->current_time= millis();                                              //get current time
  this->delta_time = (double)(this->current_time - this->previus_time);        //compute time elapsed from previous computation

  this->error = setpoint - input;                                      // determine error
  this->i_error +=  this->error *  this->delta_time;                           // compute integral
  this->d_error = (this->error - this->previus_error) / this->delta_time;             // compute derivative

  double out = this->k_p * this->error + this->k_i * this->i_error + this->k_d * this->d_error;  //PID output

  this->previus_error = this->error;                                         //remember current error
  this->previus_time =  this->current_time;                                //remember current time

  return out;                                                 //have function return the PID output
}
