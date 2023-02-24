
// EIXO Z 

// Encoder Z axis 
#include "Encoder.hpp"
int A_pin_Z=21; // Green cable
int B_pin_Z=19; // white cable
Encoder *encoder_Z;
int last_z_count;

// BTS Z axis 
#include "H_bridge_controller.hpp"
int R_pin_Z=17; // R bts
int L_pin_Z=18; // L bts
int PWM_frequency = 40000;
int PWM_resolution = 8;
int R_channel_Z=3;
int L_channel_Z=4;
int PWM_R_Z=0;
int PWM_L_Z=0;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"
int chave_L_Z=34; 
int chave_R_Z=35; 
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


  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency, PWM_resolution, R_channel_Z, L_channel_Z);
  BTS_Z->init();


  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1);
  encoder_Z->init();


  //Set origin
  

  
  Set_origin_z();
  
  Set_max_z();

  Set_origin_z();

  //PID
  
  PID_Z = new PID(kp_Z,ki_Z,kd_Z);

 

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
        output_z = PID_Z->computePID(encoder_Z->getPulses(),setPoint_z);
        // Setting direction of motion acording to output_z PID
        move_Z();

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

void Set_origin_z(){
  while (digitalRead(chave_R_Z)==HIGH){
    BTS_Z->Set_R(100);
  }
  encoder_Z->setPulses(0);
  BTS_Z->SetPWM_R(0);
}

void Set_max_z(){
  while (digitalRead(chave_L_Z)==HIGH){
    BTS_Z->Set_L(100);
  }
  MAX_PULSES_Z = encoder_Z->getPulses();
  BTS_Z->SetPWM_L(0);
}


void read_setpoint(){
    if(Serial.available()){
      
      STATE=GOING;

      char *ptr;
      comu->read_data();
      char* recived=string_to_char(comu->get_received_data());

      if (atoi((recived))>MAX_PULSES_Z){
       setPoint_z=MAX_PULSES_Z;
      }else{
      setPoint_z=atoi(string_to_char(ptr));
      }
      PID_Z->reset();
      last_z_count=-5000;
    }
}
      
void move_Z(){   
    if (output_z < 0) {
      if (output_z < -255) {
        output_z = -255;
      }
      BTS_Z->Set_R(-output_z);
    } else {
        if (output_z > 255) {
        output_z = 255;
        }
        BTS_Z->Set_L(output_z);
    }
}
void check_position(){
  if((encoder_Z->getPulses()==last_z_count )){
          BTS_Z->SetPWM_R(0);
          STATE=STAND_BY;
          Serial.print("    counter Z :");
          Serial.println(encoder_Z->getPulses());
          return;
        }
}

