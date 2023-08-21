// EIXO X
#include "config.hpp"

#include "Axis.hpp"

Axis *Axis_1;
Axis *Axis_2;

// Encoder X axis 
Encoder *encoder_1;
Encoder *encoder_2;

// PWM_constante = Axis*PWM_constante

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_1; 
H_bridge_controller *BTS_2;

//PID X axis constants
#include "PID.hpp"
PID *PID_1; 
PID *PID_2; 

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
Chave_fim_de_curso *endstop_1_L; 
Chave_fim_de_curso *endstop_1_R; 

Chave_fim_de_curso *endstop_2_L; 
Chave_fim_de_curso *endstop_2_R;

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
char STATE = 0; 

//Tempo
int sample_time=500;

double delta_time;


void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("DEBUG");

  // Setup Batentes 

  // Batente master
  endstop_1_L = new Chave_fim_de_curso(chave_1_L,0);
  endstop_1_L->init();

  endstop_1_R = new Chave_fim_de_curso(chave_master_R_X,1);
  endstop_1_R->init();
  
  // Batente slave

  endstop_2_L = new Chave_fim_de_curso(chave_slave_L_X,2);
  endstop_2_L->init();
  
  endstop_2_R = new Chave_fim_de_curso(chave_slave_R_X,3);
  endstop_2_R->init();

  //PID
  PID_1 = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_2 = new PID(kp_slave_x,ki_slave_x,kd_slave_x,i_saturation_slave_x);

  //encoder 
  encoder_1 = new Encoder(A_pin_master_X,B_pin_master_X,0,Nominal_pulses,pitch_pulley,4); 
  encoder_1->init();
  encoder_2 = new Encoder(A_pin_slave_X,B_pin_slave_X,0,Nominal_pulses,pitch_pulley,4); 
  encoder_2->init();


  // Setup H_bridge
  BTS_1 = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_1->init();
  BTS_2 = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_2->init();

  // Creating Axis
  Axis_1 = new Axis(encoder_1, BTS_1, endstop_1_R, endstop_1_L, PID_1, X_master_MAX_VEL, PWM_resolution_channel, X_size, X_master_tolerance, pwm_master_cte);
  Axis_2 = new Axis(encoder_2, BTS_2, endstop_2_R, endstop_2_L, PID_2, X_slave_MAX_VEL, PWM_resolution_channel, X_size, X_slave_tolerance, pwm_slave_cte);
  
  unsigned long current_time;
  unsigned long previus_time = millis();
  
}

void loop(){
 /*/
  
  Jeito com a biblioteca ctime, talvez assim funcione já


  Axis_master_X->go_max();

  Matriz[2][1] = {encoder_master_X->getPosition(), time()};

  Axis_master_X->go_origin();
  Matriz[2][1] = {encoder_master_X->getPosition(), time()};
  /*/

  // Jeito utilizando a função millis()

  current_time = milis() - previus_time; 

  Axis_1->go_max(); /* Eu acho que assim ele vai rodar a função até o máx para depois printar a posição do jeito que está atualmente
  Sugestão: Colocar a função de debug no go max e no corpo do debug printar a posição no serial reduz os arquivos e  */

  Serial.print(encoder_1->getPosition());
  Serial.print(",");
  Serial.println(current_time);
  

  Axis_1->go_origin();
  Serial.print(encoder_1->getPosition());
  Serial.print(",");
  Serial.println(current_time);

}
