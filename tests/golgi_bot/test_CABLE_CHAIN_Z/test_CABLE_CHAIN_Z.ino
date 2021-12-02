// BTS Z axis 
#include "H_bridge_controller.hpp"
int R_pin_Z=17; // R bts
int L_pin_Z=18; // L bts
int PWM_frequency = 40000;
int PWM_resolution = 8;
int R_channel_Z=1;
int L_channel_Z=2;
int PWM_R_Z=0;
int PWM_L_Z=0;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"
int chave_L_Z=34; 
int chave_R_Z=35; 
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 


void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);

  //Chave fim de curso
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,0);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,1);
  endstop_R_Z->init();

  // BTS
  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency, PWM_resolution, R_channel_Z, L_channel_Z);
  BTS_Z->init();


}

void loop() {
  
  shake();
    }


void shake(){
  while (digitalRead(chave_R_Z)==HIGH)
  {
    BTS_Z->Set_R(100);
  }
  BTS_Z->SetPWM_R(0);
  BTS_Z->SetPWM_L(0);
  while (digitalRead(chave_L_Z)==HIGH)
  {
    BTS_Z->Set_L(100);
  }
  BTS_Z->SetPWM_R(0);
  BTS_Z->SetPWM_L(0);
}
