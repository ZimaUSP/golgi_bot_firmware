// BTS Z axis 
#include "H_bridge_controller.hpp"
#include "config.hpp"
int R_channel_Z=1; // Na config.hpp o R_channel_Z=2 
int L_channel_Z=2;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp" 
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 


void setup() {
  //Serial Comunication
  Serial.begin (9600);

  //Chave fim de curso
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,0);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,1);
  endstop_R_Z->init();

  // BTS
  BTS_Z= new H_bridge_controller(R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
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
