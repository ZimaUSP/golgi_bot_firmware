

// EIXO X
#include "config.hpp"
#include "Axis.hpp"
Axis *Axis_master_X;
Axis *Axis_slave_X;

// Encoder X axis 
Encoder *encoder_master_X;
Encoder *encoder_slave_X;

// PWM_constante = Axis*PWM_constante

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_master_X; 
H_bridge_controller *BTS_slave_X;

//PID Z axis constants
#include "PID.hpp"
PID *PID_master_X; 
PID *PID_slave_X; 

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
Chave_fim_de_curso *endstop_master_L_X; 
Chave_fim_de_curso *endstop_master_R_X; 

Chave_fim_de_curso *endstop_slave_L_X; 
Chave_fim_de_curso *endstop_slave_R_X;

//Serial comunication/
#include "serial_communication.hpp"

SerialCommunication* comu;
#include <string>
#include <cstring>
double X_pos;
double Z_pos;

// Position array in milimeters
int pos_x[X_max_index]={97,229,370};
int pos_z[Z_max_index]={70,201};
int counter;
int index_medicine;
//STATE
char STATE = 0 ; 

//Tempo
int sample_time=500;
unsigned long current_time;
unsigned long previus_time = 0;
double delta_time;

void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("DEBUG");

  // Setup Batentes 

  // Batente master
  endstop_master_L_X = new Chave_fim_de_curso(chave_master_L_X,0);
  endstop_master_L_X->init();

  endstop_master_R_X = new Chave_fim_de_curso(chave_master_R_X,1);
  endstop_master_R_X->init();
  
  // Batente slave

  endstop_slave_L_X = new Chave_fim_de_curso(chave_slave_L_X,2);
  endstop_slave_L_X->init();
  
  endstop_slave_R_X = new Chave_fim_de_curso(chave_slave_R_X,3);
  endstop_slave_R_X->init();

  //PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_slave_X = new PID(kp_slave_x,ki_slave_x,kd_slave_x,i_saturation_slave_x);

  //encoder 
  encoder_master_X = new Encoder(A_pin_master_X,B_pin_master_X,0,Nominal_pulses,pitch_pulley,4); 
  encoder_slave_X = new Encoder(A_pin_slave_X,B_pin_slave_X,0,Nominal_pulses,pitch_pulley,4); 


  // Setup H_bridge
  BTS_master_X = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();
  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

  // Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_size, X_master_tolerance, pwm_master_cte);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_size, X_slave_tolerance, pwm_slave_cte);
  
  Axis_master_X->go_max(); 
  Axis_slave_X->go_max();
}

void loop(){
   Axis_master_X->go_origin();
   Axis_master_X->go_max();
   Axis_slave_X->go_origin();
   Axis_slave_X->go_max();
}