// EIXO X
#include "config.hpp"
#include "Axis.hpp"
Axis *Axis_master_X;
Axis *Axis_slave_X;
int error_axis;

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

//PID Z axis constants
#include "PID.hpp"
PID *PID_master_X; 
PID *PID_slave_X; 

//Serial comunication
#include "serial_communication.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;
double setPoint_master_X;

// Estados
char STATE = 0 ; // 
#define STAND_BY 0
#define GOING 1


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
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_size, X_master_tolerance, pwm_master_cte);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_size, X_slave_tolerance, pwm_slave_cte);
  
  //PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_slave_X = new PID(kp_slave_x,ki_slave_x,kd_slave_x,i_saturation_slave_x);
 
}


void loop() {
  switch(STATE) {
      case STAND_BY :
        Serial.println("STAND-BY");
        // Recive Set point
        read_setpoint();
        return;

      case GOING :

        // Setting direction of motion acording to output_x PID
        Axis_slave_X->setGoal(Axis_master_X->position());
        Axis_slave_X->move();
        /*/ Talvez precise colocar um while para o master só se mexer quando o slave estiver na mesma posição
         pq se não o master pode dar um passo mais rápido e acabar travando o slave 
         isso pode ser reparado com os parametros?
        Integrar duas vezes a saida talvez reduza o erro a 0
        Resposta ao degrau no master
        Resposta a rampa no slave
        /*/

        Axis_master_X->move();

        //Code does not work without this delay (?)
        delay(2);
        
        check_position();
        return;
   }   
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

// colocar essas funções dentro do axis.cpp
// como eu coloco serial comunication dentro do 

void read_setpoint(){
  if(Serial.available()){
      STATE=GOING;
      Serial.println("GOING"); 
      comu->read_data(MAIN_SERIAL);
      char* recived=string_to_char(comu->get_received_data());
      int setPoint_master_X=atoi(recived);
      Serial.println("Set_pont_Master", setPoint_master_X);             
      Serial.print("X goal:");
      Serial.println(encoder_master_X->getPosition());
      Axis_master_X->setGoal(setPoint_master_X);
  }
}

void check_position(){
  if(Axis_master_X->onGoal()){
    STATE=STAND_BY;
    Serial.println("STANDY-BY");
    Serial.println(Axis_master_X->position());
    Serial.println(Axis_slave_X->position());
    Axis_master_X->stop();
    Axis_slave_X->stop();
  }
  }