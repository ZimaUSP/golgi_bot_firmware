
// EIXO Z 

// Encoder Z axis 
#include "config.hpp"
#include "Controller.hpp"
Encoder *encoder_Z;
int last_z_count;

// BTS Z axis 
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
int output_z;
double setPoint_z;
PID *PID_Z; 
Axis *Axis_z;


//Count MAX
int MAX_POSITION_Z;


//Serial comunication
#include "serial_communication.hpp"
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


  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();


  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1,Nominal_pulses,pitch_pulley,Mode);
  encoder_Z->init();
    
  //PID

  PID_Z = new PID(kp_z,ki_z,kd_z);

  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);

  //Set origin
  

  
  Axis_z->go_origin();
  Serial.print("Origem: ");
  Serial.println(Axis_z->position());
  
  Axis_z->go_max();

  MAX_POSITION_Z = Axis_z->position();
  


  Axis_z->go_origin();
  Serial.print("Origem: ");
  Serial.println(Axis_z->position());
  if (endstop_R_Z->getBatente())
  {
    Serial.println("batente");
  }
  

  Serial.println("STAND-BY");
}

void loop() {
  switch(STATE) {
      case STAND_BY :
        
        // Recive Set point
        read_setpoint();
        return;
      case GOING :
        // PID Z
        // Setting direction of motion acording to output_z PID
        Axis_z->move();

        //Code does not work without this delay (?)
        delay(2);
        
        check_position();
        
        last_z_count=encoder_Z->getPosition();
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
      String received_str = Serial.readString();
      int received = received_str.toInt();


      if (received > MAX_POSITION_Z){
       setPoint_z=MAX_POSITION_Z;
      }else{
      setPoint_z=received;
      }

      Serial.print("setPoint: ");
      Serial.println(setPoint_z);

      Axis_z->setGoal(setPoint_z);

      PID_Z->reset();
    }
}

void check_position(){
  if((encoder_Z->getPosition()>= setPoint_z - Z_tolerance &&
                      encoder_Z->getPosition()<= setPoint_z + Z_tolerance )){
          BTS_Z->Set_R(0);
          STATE=STAND_BY;
          Serial.print("    counter Z :");
          Serial.println(encoder_Z->getPosition());
          Serial.println("STAND-BY");
          return;
        }
}
