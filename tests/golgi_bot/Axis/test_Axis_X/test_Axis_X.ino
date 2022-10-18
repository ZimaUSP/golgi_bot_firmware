// EIXO X
#include "config.hpp"
#include "Axis.hpp"
Axis *Axis_master_X;
Axis *Axis_slave_X;

// Encoder X axis 
Encoder *encoder_master_X;
Encoder *encoder_slave_X
// PWM_constante = Axis*PWM_constante

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_master_z;
H_bridge_controller *BTS_master_X; // Primeira chamada não é reconhecida 
H_bridge_controller *BTS_slave_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
Chave_fim_de_curso *endstop_master_L_X; 
Chave_fim_de_curso *endstop_master_R_X; 
Chave_fim_de_curso *endstop_slave_L_X; 
Chave_fim_de_curso *endstop_slave_R_X;

void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);

  // Setup Batentes // tem só um batente por eixo


  endstop_master_L_X = new Chave_fim_de_curso(chave_L_X,2);
  endstop_master_L_X->init();
  endstop_master_R_X = new Chave_fim_de_curso(chave_R_X,2);
  endstop_master_R_X->init();
  endstop_slave_L_X = new Chave_fim_de_curso(chave_L_X,2);
  endstop_slave_L_X->init();
  endstop_slave_R_X = new Chave_fim_de_curso(chave_R_X,2);
  endstop_slave_R_X->init();


  // Setup H_bridge
  BTS_master_X = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();
  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

  // Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);
  
  Axis_master_z->go_max(); 
  Axis_slave_z->go_max();
}

void loop(){
    Axis_master_z->go_origin();
    Axis_master_z->go_max();
    Axis_slave_z->go_origin();
    Axis_slave_z->go_max();
}
