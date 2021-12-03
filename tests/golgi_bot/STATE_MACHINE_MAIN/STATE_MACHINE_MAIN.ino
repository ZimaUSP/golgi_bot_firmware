// Encoder X axis 
#include "Encoder.hpp"
int A_pin_X=22; // Green cable
int B_pin_X=23; // White cable
Encoder *encoder_X;
int last_x_count;

// BTS X axis 
#include "H_bridge_controller.hpp"
int R_pin_X=26; // L Bts
int L_pin_X=27; // R Bts
int PWM_frequency = 40000;
int PWM_resolution = 8;
int R_channel_X=1;
int L_channel_X=2;
int PWM_R_X=0;
int PWM_L_X=0;
H_bridge_controller *BTS_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
int chave_L_X=36; 
int chave_R_X=39; 
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

//PID X axis constants
#include "PID.hpp"
double kp_x = 0.2;
double ki_x = 0;
double kd_x= 2.5;
int output_x;
double setPoint_x;
PID *PID_X; 

//Count MAX
int MAX_PULSES_X=12000;


// EIXO Z 

// Encoder Z axis 
#include "Encoder.hpp"
int A_pin_Z=19; // Green cable
int B_pin_Z=21; // white cable
Encoder *encoder_Z;
int last_z_count;

// BTS Z axis 
#include "H_bridge_controller.hpp"
int R_pin_Z=17; // R bts
int L_pin_Z=18; // L bts
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
int MAX_PULSES_Z =12000;



// EIXO Y 


// Bomba axis 
#include "Bomba.hpp"
int bomba_pin=32;//IN2
Bomba *Bomba_Y;

// atuador axis 
#include "Atuador.hpp"
int Extend_pin=33; //IN4
int Contract_pin=25; //IN3
Atuador *Atuador_Y;

// Ultrassonico
#include "Ultrasonico.hpp"
const int trigPin = 13;
const int echoPin = 14;
Ultrasonico *Ultrasonico_Y;

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
  setPoint_x = 0;

  setPoint_z = 0;
  
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();

  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channel_X);
  BTS_X->init();

  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency, PWM_resolution, R_channel_Z, L_channel_Z);
  BTS_Z->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0);
  encoder_X->init();

  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1);
  encoder_Z->init();

   // Atuador
  Atuador_Y= new Atuador( Extend_pin,Contract_pin);
  Atuador_Y->init();
  Atuador_Y->Contract();
  delay(1000);
  Atuador_Y->Stop();
  
   // Bomba
  Bomba_Y= new Bomba(bomba_pin);
  Bomba_Y->init();

  // Ultrassonico
  Ultrasonico_Y= new Ultrasonico(trigPin,echoPin);
  Ultrasonico_Y->init();

  //Set origin
  

  Set_origin_x();
  
  Set_origin_z();

  Set_max_x();
  
  Set_max_z();

  Set_origin_x();
  
  Set_origin_z();

  //PID
  PID_X = new PID(kp_x,ki_x,kd_x);
  
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
        
        // PID X
        output_x = PID_X->computePID(encoder_X->getPulses(),setPoint_x);
        // Setting direction of motion acording to output_x PID
        move_X(); 
        // PID Z
        output_z = PID_Z->computePID(encoder_Z->getPulses(),setPoint_z);
        // Setting direction of motion acording to output_z PID
        move_Z();

        //Code does not work without this delay (?)
        delay(2);
        
        check_position();
        
        last_x_count=encoder_X->getPulses();
        last_z_count=encoder_Z->getPulses();
        return;
      case GETING_MEDICINE :

        Serial.println("GETING_MEDICINE");
        Get_medicine();
        return;
      case DROPING_MEDICINE :
        Serial.println("DROPING_MEDICINE");
        Set_origin_x();
        Set_max_z();
        Drop_medicine();
        return;
   }
    
    // Debug print X
    //Serial.print("setPoint_x: ");
    //Serial.println(setPoint_x);
    //Serial.print("output_x: ");
    //Serial.println(output_x);
    

    // Debug print Z
    //Serial.print("Setpoint_z: ");
    //Serial.println(setPoint_z);
    //Serial.print("output: ");
    //Serial.println(output_z);
    
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void Set_origin_x(){
  while (digitalRead(chave_R_X)==HIGH){
    BTS_X->Set_R(100);
  }
  encoder_X->setPulses(0);
  BTS_X->SetPWM_R(0);
}

void Set_origin_z(){
  while (digitalRead(chave_R_Z)==HIGH){
    BTS_Z->Set_R(100);
  }
  encoder_Z->setPulses(0);
  BTS_Z->SetPWM_R(0);
}

void Set_max_x(){
  while (digitalRead(chave_L_X)==HIGH){
    BTS_X->Set_L(100);
  }
  MAX_PULSES_X = encoder_X->getPulses();
  BTS_X->SetPWM_L(0);
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
git 
      comu->read_data();
      char* recived=string_to_char(comu->get_received_data());

      ptr = strtok(recived, "-");
      if (atoi(string_to_char(ptr))>MAX_PULSES_X){
       setPoint_x=MAX_PULSES_X ;
      }else{
      setPoint_x=atoi(string_to_char(ptr)); 
      }
      ptr = strtok (NULL, "-");
      if (atoi(string_to_char(ptr))>MAX_PULSES_Z){
       setPoint_z=MAX_PULSES_Z;
      }else{
      setPoint_z=atoi(string_to_char(ptr));
      }
      PID_X->reset();
      PID_Z->reset();
      last_x_count=-5000;
      last_z_count=-5000;
    }
}
      
  

void move_X(){     
    if (output_x < 0) {
      if (output_x < -255) {
        output_x = -255;
      }
      BTS_X->Set_R(-output_x);
    } else {
      if (output_x > 255) {
        output_x = 255;
      }
      BTS_X->Set_L(output_x);
    }
}

void move_Z(){   
    if (output_z < 0) {
      if (output_z < -255) {
        output_z = -255;
      }
      BTS_Z->Set_R(-output_x);
    } else {
        if (output_z > 255) {
        output_z = 255;
        }
        BTS_Z->Set_L(output_x);
    }
}
    
void Get_medicine(){
  Bomba_Y->turn_on();
  Atuador_Y->Extend();
  delay(2500);
  Atuador_Y->Contract();
  delay(1000);
  Atuador_Y->Stop();
  STATE=DROPING_MEDICINE;
  /*if(Ultrasonico_Y->get_distance()<10){
    Serial.println(Ultrasonico_Y->get_distance());
    STATE=DROPING_MEDICINE;

    }*/

}

void Drop_medicine(){
  Bomba_Y->turn_off();
  STATE=STAND_BY;
}


void check_position(){
  if((encoder_X->getPulses()==last_x_count) && (encoder_Z->getPulses()==last_z_count )){
          BTS_X->SetPWM_R(0);
          BTS_Z->SetPWM_R(0);
          STATE=GETING_MEDICINE;
          Serial.print("counter X :");
          Serial.println(encoder_X->getPulses());
          Serial.print("    counter Z :");
          Serial.println(encoder_Z->getPulses());
          return;
        }
}

