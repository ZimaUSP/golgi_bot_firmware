int *count, *sum;

int a = 10;

unsigned long *p_time, *c_time, *d_time;

// EIXO X
#include "config.hpp"

// Encoder X axis 
#include "Encoder.hpp"
Encoder *encoder_master_X;

// Axis X
#include "Axis.hpp"
Axis *Axis_master_X;
int error_axis;

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_master_X; 

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"

  //Master
  Chave_fim_de_curso *endstop_master_L_X; 
  Chave_fim_de_curso *endstop_master_R_X; 

// PID X axis 
#include "PID.hpp"
PID *PID_master_X; 


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

// Setup H_bridge

  BTS_master_X = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();

// Setup encoder 
  encoder_master_X =new Encoder(A_pin_master_X,B_pin_master_X,0,Nominal_pulses,pitch_pulley,4);
  encoder_master_X->init();

//PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
 
  // Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_size, X_master_tolerance, pwm_master_cte, false);

  // initial condition and axis debug
  Axis_master_X->go_max(); 
  Axis_master_X->go_origin();
  sum = 0;
  c_time = millis();
}


void loop() {
  switch(STATE) {
      case STAND_BY:
        read_setpoint(); // Recive Set point
        return;

      case GOING:
        Axis_master_X->move(); // moving axis to setpoint 

        delay(2); //Code does not work without this delay (?)

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
      Axis_master_X->setGoal(setPoint_master_X);     
  }
}

void check_position(){
    if(Axis_master_X->onGoal()){

    STATE=STAND_BY;
    Serial.println("STANDY-BY");
    Serial.println(Axis_master_X->position());

    Axis_master_X->stop();
  }
}
  
void data(){
  sum += encoder_master_X->getPosition();

  if (count == 0)
  {
    p_time = c_time; // Ti 
  }

  if (count == a) // agruping a serie of position 
  {
    c_time = millis(); // Tf
    d_time = c_time - p_time; // delta = Tf - Ti

    Serial.print(sum/a); // media of position 
    Serial.print(", ");
    Serial.println(d_time); // delta time 

    sum = 0;
    count = 0;
  }
  count++;
        
}
  
