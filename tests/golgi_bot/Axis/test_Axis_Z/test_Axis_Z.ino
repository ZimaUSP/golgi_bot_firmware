
// EIXO Z 
#include "config.hpp"
#include "Axis.hpp"
Axis *Axis_z;
int pos_z[Z_max_index]={80, 690};

//PID Z axis constants
PID *PID_Z; 

// Encoder Z axis 
Encoder *encoder_Z;


// PWM_constante = Axis*PWM_constante


// BTS Z axis 
#include "H_bridge_controller.hpp"



H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"

Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);

  // Setup Batentes
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z, chave_channel_L_Z);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z, chave_channel_R_Z);
  endstop_R_Z->init();

  // Setup H_bridge
  BTS_Z = new H_bridge_controller(R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();
  
  // Creating Axis
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance, false);
  
  //Axis_z->go_max(); //Se colocar pra começar na origem da erro
  //delay(5000);
}

void loop(){
    BTS_Z->Set_R(125);
    //Axis_z->go_L();
    //Axis_z->go_origin();
      
    //Axis_z->go_max();
}
