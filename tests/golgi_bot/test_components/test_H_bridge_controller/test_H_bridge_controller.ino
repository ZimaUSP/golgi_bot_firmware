
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_X;
#include "config.hpp";
#include "Chave_fim_de_curso.hpp";

Chave_fim_de_curso *endstop_R;
Chave_fim_de_curso *endstop_L;



void setup() {
  Serial.begin(9600);
  endstop_R = new Chave_fim_de_curso(chave_R_X_slave,0);
  endstop_R->init();
  endstop_L = new Chave_fim_de_curso(chave_L_X_slave,1);
  endstop_L->init();
  BTS_X= new H_bridge_controller( 16, 12, PWM_frequency_channel, PWM_resolution_channel, 4, 5);
  BTS_X->init();
  
}

void loop() {
   while (!endstop_R->getBatente()){
      BTS_X->Set_R(100);
  }   
      BTS_X->Set_R(0);
      delay(3000);
   while (!endstop_L->getBatente()){
      BTS_X->Set_L(100);
  }   
      BTS_X->Set_L(0);
        delay(3000);
  }
