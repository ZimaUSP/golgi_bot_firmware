
// EIXO Z 
#include "config.hpp"

// Encoder Z axis 
#include "Encoder.hpp"
Encoder *encoder_Z;
int last_z_count;

//Axis Z
#include "Axis.hpp"
Axis *Axis_z;
int pos_z[Z_max_index]={80, 690};


// BTS Z axis 
#include "H_bridge_controller.hpp"
int PWM_R_Z=0;
int PWM_L_Z=0;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
#include "PID.hpp"
double kp_Z = 0.1;
double ki_Z = 0.000007;
double kd_Z= 5; 
int output_z;
double setPoint_z;
PID *PID_Z; 

//Count MAX
int MAX_PULSES_Z =0;


//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;

double Z_pos;
int counter;

// Estados
char STATE = 0 ; // 
#define STAND_BY 0
#define GOING 1
#define GETING_MEDICINE 2
#define DROPING_MEDICINE 3


void setup() {
  // Set point
  setPoint_z = 0;
  
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();


  BTS_Z= new H_bridge_controller(R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();


  encoder_Z =new Encoder(A_pin_Z,B_pin_Z,0,600,40,4);
  encoder_Z->init();

 // Creating Axis
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);
  

  //PID
  PID_Z = new PID(kp_Z,ki_Z,kd_Z,i_saturation_z);
 
  Axis_z->go_max();
  Axis_z->go_origin();
}

void loop() {
  switch(STATE) {
      case STAND_BY :
        Serial.println("STAND-BY");
        // Recive Set point
        read_setpoint();
        return;
      case GOING :
        // PID Z
        Serial.println("MOVE");
       
        // Setting direction of motion acording to output_z PID
        
        Axis_z->move();

        //Code does not work without this delay (?)
        delay(2);
        
        check_position();
        
        last_z_count=encoder_Z->getPulses();
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
      int setPoint_z=atoi(recived);
      Serial.println(setPoint_z);
      
             
      Serial.print("Z goal:");
      Serial.println(encoder_Z->getPosition());
      Axis_z->setGoal(setPoint_z);
  }

}



void check_position(){
  if( Axis_z->onGoal()){
    STATE=STAND_BY;
    Serial.println("STANDY-BY");
    Serial.println(Axis_z->position());
    Axis_z->stop();
  }
  }