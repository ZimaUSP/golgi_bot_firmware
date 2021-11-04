// Encoder Z axis 
#include "Encoder.hpp"
int A_pin_Z=12;
int B_pin_Z=14;
Encoder *encoder_Z;

// BTS Z axis 
#include "H_bridge_controller.hpp"
int R_pin_Z=32;
int L_pin_Z=33;
int PWM_frequency = 40000;
int PWM_resolution = 10;
int R_channel_Z=1;
int L_channel_Z=2;
int PWM_R_Z=0;
int PWM_L_Z=0;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"
int chave_L_Z=34; 
int chave_R_Z=27; 
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
#include "PID.hpp"
double kp_Z = 0.2;
double ki_Z = 0;
double kd_Z= 2; 
int output_z;
double setPoint_z;
PID *PID_Z; 

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
  setPoint_z = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,0);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,1);
  endstop_R_Z->init();

  // Encoder
  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,0);
  encoder_Z->init();

  // BTS
  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency, PWM_resolution, R_channel_Z, L_channelV);
  BTS_Z->init();
  
  //PID
  PID_Z = new PID(kp_Z,ki_Z,kd_Z);

  //Set origin
  Set_origin();

}

void loop() {
  
  // Recive Set point
  read_setpoint();
 
  
  //Batente
  if(endstop_R_Z->getBatente()||endstop_L_Z->getBatente()){
      if(endstop_R_Z->getBatente()){
      Serial.println("Batente Direita");
      }else{
      Serial.println("Batente Esquerda");
    }}
    

    // PID Z
    output_z = PID_Z->computePID(encoder_Z->getPulses();,setPoint_z);

    // Setting direction of motion acording to output PID
    if (output_z > 255) {
      output_z = 255;
    }
    if (output_z < -255) {
      output_z = -255;
    }
    if (output_z < 0) {

      PWM_R_Z = 0;
      PWM_L_Z = -output_z;
    } else {
      PWM_L_Z = 0;
      PWM_R_Z = output_z;
    }
    //Setting BTS X axis PWM channel
    BTS_Z->SetPWM_R(PWM_R_Z);
    BTS_Z->SetPWM_l(PWM_L_Z);

    // Debug print
    Serial.print("Setpoint_z: ");
    Serial.println(setPoint_z);
    Serial.print("output: ");
    Serial.println(output_z);
    Serial.print("counter :");
    Serial.println((encoder_Z->getPulses()));
    }

float pos(int rev) { //Retorna a posição em centimetros
  return rev * (42.5 / 12000);
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}
void Set_origin(){
  while (digitalRead(chave_R_Z)==HIGH)
  {
    PWM_R_Z = 0;
    PWM_L_Z = 250;
    BTS_Z->SetPWM_R(PWM_R_Z);
    BTS_Z->SetPWM_l(PWM_L_Z);
  }
  PWM_R_Z = 0;
  PWM_L_Z = 0;
  BTS_Z->SetPWM_R(PWM_R_Z);
  BTS_Z->SetPWM_l(PWM_L_Z);
  encoder_Z->setPulses(0);
}
void read_setpoint(){
   if(Serial.available()){
    comu->read_data();
    setPoint_z=atoi(string_to_char(comu->get_received_data()));
    if(setPoint>MAX_PULSES){
      setPoint_z=MAX_PULSES;
    }
    if(setPoint_z<0){
      setPoint_z=0;
    }
  }
}