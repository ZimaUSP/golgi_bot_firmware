
// BTS X axis 
#include "H_bridge_controller.hpp"
int R_pin_X=27; // L Bts
int L_pin_X=26; // R Bts
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

void setup() {
  //Serial Comunication
  Serial.begin (9600);

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channel_X);
  BTS_X->init();
  


}

void loop() {

  shake();
    }


void shake(){
  while (digitalRead(chave_R_X)==HIGH)
  {
    BTS_X->Set_R(150);
  }
  BTS_X->SetPWM_R(0);
  BTS_X->SetPWM_L(0);
  while (digitalRead(chave_L_X)==HIGH)
  {
    BTS_X->Set_L(150);
  }
  BTS_X->SetPWM_R(0);
  BTS_X->SetPWM_L(0);
}
