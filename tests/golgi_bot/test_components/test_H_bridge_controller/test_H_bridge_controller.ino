
#include "H_bridge_controller.hpp"
#include "config.hpp"
#include "Chave_fim_de_curso.hpp"
H_bridge_controller *BTS_X;
// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_R_master_X; 
Chave_fim_de_curso *endstop_L_master_X; 

void setup() {
  Serial.begin(9600);
  BTS_X= new H_bridge_controller( R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_X->init();
  endstop_R_master_X = new Chave_fim_de_curso(chave_master_R_X,2);
  endstop_R_master_X->init();
  endstop_L_master_X = new Chave_fim_de_curso(chave_master_L_X,3);
  endstop_L_master_X->init();
}

void loop(){
  while (!endstop_R_master_X->getBatente())
  {
      BTS_X->Set_R(150);
      Serial.println("Going R");
  }
  BTS_X->Set_R(0);  
  Serial.println("chave fim de curso R pressionada");
  delay(3000);
  
  
  while (!endstop_L_master_X->getBatente())
  {
      BTS_X->Set_L(150);
      Serial.println("Going L");

  }
  BTS_X->Set_L(0);  
  Serial.println("chave fim de curso L pressionada");
  delay(3000);
  
}
