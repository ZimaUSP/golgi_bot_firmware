/**
 * @file  PID_incremental.cpp
 *
 * @brief PID_incremental class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 11/2024
 * 
 * @copyright MIT License
 */

#include "PID_incremental.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

PID_incremental::PID_incremental(double N, double k_p, double t_i, double t_d, double t_s) {
    this->N = N;
    this->K_P = k_p;
    this->T_I = t_i;
    this->T_D = t_d; 
    this->T_s = t_s;

    this->previous_u = 0;
    this->error = 0;
    this->previous_error1 = 0;
}

double PID_incremental::computePID_incremental(double input, float setpoint, double tolerance) {
  double u, delta_up, delta_ui, delta_ud; 

  this->current_time = millis();    

  if (this->current_time - this->previous_time >= this->T_s * 1000) {      // * 1000 to turn from s to ms                            
    this->delta_time = (double)(this->current_time - this->previous_time);  // depois testar, essa linha n serve de nada  

    this->error = setpoint - input;                                     

    delta_up = this->K_P * (this->error - this->previous_error1);

    delta_ui = ((this->K_P * this->T_s) / (2 * this->T_I)) * (this->error + this->previous_error1);
    // Serial.print("delta_ui: ");
    // Serial.println(delta_ui);

    delta_ud = ((this->T_D / (this->T_D + (this->N * this->T_s))) * this->previous_delta_ud) - (((this->K_P * this->N * this->T_D) / (this->T_D + (this->N * this->T_s))) * (this->error - 2 * this->previous_error1 + this->previous_error2));
    // Serial.print("delta_ud: ");
    // Serial.println(delta_ud);

    // Serial.print("delta_up: ");
    // Serial.println(delta_up);
    // Serial.print("delta_ui: ");
    // Serial.println(delta_ui);
    // Serial.print("delta_ud: ");
    // Serial.println(delta_ud);

    if (error >= 320) {
      delta_u = delta_up + delta_ud;
    } else {
      delta_u = delta_up + delta_ui + delta_ud;
    }

    u = this->previous_u + delta_u;

    this->previous_u = u;
    this->previous_delta_ud = delta_ud;
    this->previous_error2 = this->previous_error1;
    this->previous_error1 = this->error;
    this->previous_time = this->current_time;

    return u;     
  }    
  return this->previous_u;                                       
}

void PID_incremental::reset_incremental() {
    this->error = 0;
    this->previous_error1 = 0;
    this->previous_error2 = 0;
    this->delta_time = 0;
    this->previous_u = 0;
    this->previous_delta_ud = 0;
    this->previous_time = millis();
}
