int count, sum;

int sample = 10;

unsigned long p_time, c_time, d_time;

// EIXO X
#include "config.hpp"

// Encoder X axis 
#include "Encoder.hpp"
Encoder *encoder_slave_X;

// Axis X
#include "Axis.hpp"
Axis *Axis_slave_X;
int error_axis;

// BTS X axis 
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_slave_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"

  // Slave
  Chave_fim_de_curso *endstop_slave_L_X; 
  Chave_fim_de_curso *endstop_slave_R_X;

// PID X axis 
#include "PID.hpp"
PID *PID_slave_X; 


//Serial comunication
#include "serial_communication.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;
double setPoint_slave_X;

// Estados
char STATE = 0 ; // 
#define STAND_BY 0
#define GOING 1


void setup() {
  // Set point
  setPoint_slave_X = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");
  
// Setup Batentes 

  // Slave
    endstop_slave_L_X = new Chave_fim_de_curso(chave_slave_L_X,3);
    endstop_slave_L_X->init();
    endstop_slave_R_X = new Chave_fim_de_curso(chave_slave_R_X,4);
    endstop_slave_R_X->init();


// Setup H_bridge

  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

// Setup encoder 

  encoder_slave_X =new Encoder(A_pin_slave_X,B_pin_slave_X,0,Nominal_pulses,pitch_pulley_slave,4);
  encoder_slave_X->init();

//PID
  PID_slave_X = new PID(kp_slave_x,ki_slave_x,kd_slave_x,i_saturation_slave_x);
 

  // Creating Axis
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_slave_tolerance, pwm_slave_cte, true);

  sum = 0;
  c_time = millis();

//Seting initial conditions

  Axis_slave_X->go_origin();
  Axis_slave_X->go_max();
  Axis_slave_X->go_origin();

}


void loop() {
  switch(STATE) {
      case STAND_BY:
        read_setpoint(); // Recive Set point
        return;

      case GOING:
        Axis_slave_X->move(); // moving axis to setpoint 

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

      int setPoint_slave_X=atoi(recived);
      Axis_slave_X->setGoal(setPoint_slave_X);     
  }
}

void check_position(){
    if(Axis_slave_X->onGoal()){

    STATE=STAND_BY;
    Serial.println("STANDY-BY");
    Serial.println(Axis_slave_X->position());

    Axis_slave_X->stop();
  }
}
  
void data(){
  sum += encoder_slave_X->getPosition();

  if (count == 0)
  {
    p_time = c_time; // Ti 
  }

  if (count == sample) // agruping sample serie of position 
  {
    c_time = millis(); // Tf
    d_time = c_time - p_time; // delta = Tf - Ti

    Serial.print(sum/sample); // media of position 
    Serial.print(", ");
    Serial.println(d_time); // delta time 

    sum = 0;
    count = 0;
  }

  count++;
        
}
  
