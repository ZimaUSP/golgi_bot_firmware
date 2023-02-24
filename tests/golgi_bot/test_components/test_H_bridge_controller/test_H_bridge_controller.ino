
#include "H_bridge_controller.hpp"
H_bridge_controller *BTS_X;
int R_pin_X=12;
int L_pin_X=13;

int PWM_frequency = 40000;
int PWM_resolution = 10;

int R_channel_X=1;
int L_channel_X=2;

void setup() {
  Serial.begin(9600);
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channel_X);
  BTS_X->init();
}

void loop() {
  for(int i=0;i<1024;i++){
      BTS_X->Set_R(i);
      delay(10);
  }

  for(int i=0; i<1024;i++){
      BTS_X->Set_L(i);
      delay(10);
  }
}