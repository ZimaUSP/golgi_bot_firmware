// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//Axis lib

#include "Axis.hpp"

// EIXO X

Axis *Axis_x;

// Encoder X axis 
Encoder *encoder_X;

// BTS X axis 
H_bridge_controller *BTS_X;

// Chave fim de curso X axis
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

//PID X axis constants
PID *PID_X; 


// EIXO Z

Axis *Axis_z;

// Encoder Z axis 
Encoder *encoder_Z;

// BTS Z axis 
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
PID *PID_Z; 


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
unsigned long current_time;
unsigned long previus_time = 0;
double delta_time;

void setup() {
   
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("DEBUG");

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();


  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_X, L_channel_X);
  BTS_X->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0,Nominal_pulses,pitch_pulley,Mode);
  encoder_X->init();

  //PID
  PID_X = new PID(kp_x,ki_x,kd_x,i_saturation_x);
  

  //Creating Axis
  Axis_x= new Axis(encoder_X, BTS_X, endstop_R_X, endstop_R_X, PID_X, X_MAX_VEL, PWM_resolution_channel, X_size, X_tolerance);
  


  Serial.println("STAND-BY");

}

void loop() {
  switch(STATE) {
      case STAND_BY :
        // Recive Set point
        read_setpoint();
        return;

      case GOING :
        //Moves Controller
        Axis_x->move();
        //Code does not work without this delay (?)
        delay(5);
        check_position();
        return;
      
      case DROPING_MEDICINE :
        Axis_x->go_origin();
        STATE=STAND_BY;
        Serial.println("STAND-BY");
        return;
   }  
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void read_setpoint(){
    if(Serial.available()){
      int xpos = atoi(string_to_char(comu->get_received_data()));
      STATE=GOING;
      Serial.println("GOING"); 
      comu->read_data(MAIN_SERIAL);
      char* recived=string_to_char(comu->get_received_data());
      int index_medicine=atoi(recived);
      counter=0;
        for(int i =0; i < X_max_index; i++){
          counter=counter+1;
          if (counter==index_medicine){
            X_pos=pos_x[i];
          }
        }
      
      Serial.println(X_pos);
      Axis_x->setGoal(X_pos);
    }
}

void check_position(){
  if(Axis_x->onGoal()){
    STATE=DROPING_MEDICINE;
    Serial.println("DROPING_MEDICINE");
    Serial.println(Axis_x->position());
  }
}     
  

