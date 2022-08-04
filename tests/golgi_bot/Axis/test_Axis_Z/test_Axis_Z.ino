// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//Axis lib

#include "Controller.hpp"


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

  //Chave fim de curso
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();


  // BTS
  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();

  // Encoder
  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1,Nominal_pulses,pitch_pulley,Mode);
  encoder_Z->init();

  //PID
  PID_Z = new PID(kp_z,ki_z,kd_z,i_saturation_z);
  

  //Creating Axis
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_R_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);
  

  Axis_z->go_origin();

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
        Axis_z->move();
        //Code does not work without this delay (?)
        delay(5);
        check_position();
        return;
      
      case DROPING_MEDICINE :
        Axis_z->go_origin();
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
      STATE=GOING;
      Serial.println("GOING"); 
      comu->read_data(MAIN_SERIAL);
      char* recived=string_to_char(comu->get_received_data());
      int index_medicine=atoi(recived);
      counter=0;
        for(int i =0; i < Z_max_index; i++){
          counter=counter+1;
          if (counter==index_medicine){
            Z_pos=pos_z[i];
          }
        }
      
      Serial.println(Z_pos);
      Axis_z->setGoal(Z_pos);
    }
}

void check_position(){
  if(Axis_z->onGoal()){
    STATE=DROPING_MEDICINE;
    Serial.println("DROPING_MEDICINE");
    Serial.println(Axis_z->position());
  }
}     
  
