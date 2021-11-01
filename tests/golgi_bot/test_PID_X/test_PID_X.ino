// Encoder X axis 
#include "Encoder.hpp"
int A_pin_X=12;
int B_pin_X=14;
Encoder *encoder_X;

// BTS X axis 
#include "H_bridge_controller.hpp"
int R_pin_X=32;
int L_pin_X=33;
int PWM_frequency = 40000;
int PWM_resolution = 10;
int R_channel_X=1;
int L_channel_X=2;
int PWM_R_X=0;
int PWM_L_X=0;
H_bridge_controller *BTS_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
int chave_L_X=34; 
int chave_R_X=27; 
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

//PID X axis constants
#include "PID.hpp"
double kp_x = 0.2;
double ki_x = 0;
double kd_x= 2;
int input; 
int output;
double setPoint;
PID *PID_X; 

//Count MAX
int MAX_PULSES =12543;

//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;

void setup() {
  // Set point
  setPoint = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0);
  encoder_X->init();

  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channelV);
  BTS_X->init();
  
  //PID
  PID_X = new PID(kp_x,ki_x,kd_x);

  //Set origin
  while (digitalRead(chave_R_X)==HIGH)
  {
    PWM_R_X = 0;
    PWM_L_X = 250;
    BTS_X->SetPWM_R(PWM_R_X);
    BTS_X->SetPWM_l(PWM_L_X);
  }
  PWM_R_X = 0;
  PWM_L_X = 0;
  BTS_X->SetPWM_R(PWM_R_X);
  BTS_X->SetPWM_l(PWM_L_X);
  encoder_X->setPulses(0);

}

void loop() {
  
  // Set point
  
  if(Serial.available()){
    comu->read_data();
    setPoint=atoi(string_to_char(comu->get_received_data()));
    if(setPoint>MAX_PULSES){
      setPoint=MAX_PULSES;
    }
    if(setPoint<0){
      setPoint=0;
    }
  }
  
  //Batente
  if(endstop_R_X->getBatente()||endstop_L_X->getBatente()){
      if(endstop_R_X->getBatente()){
      Serial.println("Batente Direita");
      }else{
      Serial.println("Batente Esquerda");
    }}
    

    // PID
    input = encoder_X->getPulses();
    output = PID_X->computePID(input,setPoint);

    // Setting direction of motion acording to output PID
    if (output > 255) {
      output = 255;
    }
    if (output < -255) {
      output = -255;
    }
    if (output < 0) {

      PWM_R_X = 0;
      PWM_L_X = -output;
    } else {
      PWM_L_X = 0;
      PWM_R_X = output;
    }
    //Setting BTS X axis PWM channel
    BTS_X->SetPWM_R(PWM_R_X);
    BTS_X->SetPWM_l(PWM_L_X);

    // Debug print
    Serial.print("Setpoint: ");
    Serial.println(setPoint);
    Serial.print("output: ");
    Serial.println(output);
    Serial.print("counter :");
    Serial.println((encoder_X->getPulses()));
    }

float pos(int rev) { //Retorna a posição em centimetros
  return rev * (42.5 / 12000);
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}
