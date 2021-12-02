// Encoder X axis 
#include "Encoder.hpp"
int A_pin_X=22; // Green cable
int B_pin_X=23; // White cable
Encoder *encoder_X;

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
double kd_x= 2;
int output_x;
double setPoint_x;
PID *PID_X; 

//Count MAX
int MAX_PULSES_X=12000;

//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu_x;

void setup() {
  // Set point
  setPoint_x = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu_x = new SerialCommunication("Posição setPoint_x:");

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0);
  encoder_X->init();

  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channel_X);
  BTS_X->init();
  
  //PID
  PID_X = new PID(kp_x,ki_x,kd_x);

  //Set origin
  Set_origin_x();

}

void loop() {
  
  // Recive Set point
  read_setpoint_x();
  
  
  //Batente
  if(endstop_R_X->getBatente()||endstop_L_X->getBatente()){
      if(endstop_R_X->getBatente()){
      Serial.println("Batente Direita");
      }else{
      Serial.println("Batente Esquerda");
    }}
    

    // PID X
    output_x = PID_X->computePID(encoder_X->getPulses(),setPoint_x);

    // Setting direction of motion acording to output_x PID
    if (output_x > 155) {
      output_x = 155;
    }
    if (output_x < -155) {
      output_x = -155;
    }
    if (output_x < 0) { 
       BTS_X->Set_R(-output_x);
    } else {
       BTS_X->Set_L(output_x);
    }

    // Debug print
    Serial.print("setPoint_x: ");
    Serial.println(setPoint_x);
    Serial.print("output_x: ");
    Serial.println(output_x);
    Serial.print("counter :");
    Serial.println(encoder_X->getPulses());
    }

float pos(int rev) { //Retorna a posição em centimetros
  return rev * (42.5 / 12000);
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void Set_origin_x(){
  while (digitalRead(chave_R_X)==HIGH)
  {
    BTS_X->Set_R(100);
  }
  
  encoder_X->setPulses(0);
 
  BTS_X->SetPWM_R(0);
  BTS_X->SetPWM_L(0);
}

void read_setpoint_x(){

   if(Serial.available()){

      comu_x->read_data();

      setPoint_x=atoi(string_to_char(comu_x->get_received_data()));

      if(setPoint_x>MAX_PULSES_X){
        setPoint_x=MAX_PULSES_X;
      }
    }
}