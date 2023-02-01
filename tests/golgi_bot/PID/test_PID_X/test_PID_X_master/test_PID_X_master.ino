// EIXO X
#include "config.hpp"

// Encoder X axis 
#include "Encoder.hpp"
Encoder *encoder_master_X;
Encoder *encoder_slave_X;
int last_x_count;

// Axis X
#include "Axis.hpp"
Axis *Axis_master_X;
Axis *Axis_slave_X;
int error_axis;

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_master_X; 
H_bridge_controller *BTS_slave_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"

  //Master
  Chave_fim_de_curso *endstop_master_L_X; 
  Chave_fim_de_curso *endstop_master_R_X; 

  // Slave
  Chave_fim_de_curso *endstop_slave_L_X; 
  Chave_fim_de_curso *endstop_slave_R_X;

// PID X axis 
#include "PID.hpp"
PID *PID_master_X; 
PID *PID_slave_X; 

// Count MAX
int MAX_PULSES_Z =0;


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
  // Set point
  setPoint_master_X = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");
  
// Setup Batentes 

  // Master
    endstop_master_L_X = new Chave_fim_de_curso(chave_master_L_X,1);
    endstop_master_L_X->init();
    endstop_master_R_X = new Chave_fim_de_curso(chave_master_R_X,2);
    endstop_master_R_X->init();

  // Slave
    endstop_slave_L_X = new Chave_fim_de_curso(chave_slave_L_X,3);
    endstop_slave_L_X->init();
    endstop_slave_R_X = new Chave_fim_de_curso(chave_slave_R_X,4);
    endstop_slave_R_X->init();


// Setup H_bridge

  BTS_master_X = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();

  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

// Setup encoder 
  encoder_master_X =new Encoder(A_pin_master_X,B_pin_master_X,0,600,40,4);
  encoder_master_X->init();

  encoder_slave_X =new Encoder(A_pin_slave_X,B_pin_slave_X,0,600,40,4);
  encoder_slave_X->init();

//PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_slave_X = new PID(kp_slave_x,ki_slave_x,kd_slave_x,i_saturation_slave_x);
 
  // Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_size, X_master_tolerance, pwm_master_cte, true);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_size, X_slave_tolerance, pwm_slave_cte, false);

  Axis_master_X->go_max(); 
  // Axis_slave_X->go_max(); 

  Axis_master_X->go_origin(); 
  // Axis_slave_X->go_origin();
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
        //Serial.println("Set goal slave");
        //Axis_slave_X->setGoal(Axis_master_X->position());
        
        //Axis_slave_X->move();
        
        Serial.println("move master");  
        Axis_master_X->move();
        
        //Code does not work without this delay (?)
        delay(2);
        check_position();

        last_x_count = encoder_master_X->getPulses();
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

      //Serial.println("Set_point_Master:"); 
      //Serial.println(setPoint_master_X);

      Axis_master_X->setGoal(setPoint_master_X);     
      //Axis_slave_X->setGoal(setPoint_master_X);     


  }
}

void check_position(){
  if(Axis_master_X->onGoal()){
    //if(Axis_slave_X->onGoal()){

    //Serial.println("Entrou no if on goal master x");
    STATE=STAND_BY;
    Serial.println("STANDY-BY");
    Serial.println(Axis_master_X->position());
    //Serial.println(Axis_slave_X->position());
     Axis_master_X->stop();
    //Axis_slave_X->stop();
  }
}
  
  
