
#include "H_bridge_controller.hpp"
#include "Encoder.hpp"
#include "config.hpp"
#include "Chave_fim_de_curso.hpp"

H_bridge_controller *BTS_1;
Encoder *encoder_1;

Chave_fim_de_curso *endstop_R_1; 
Chave_fim_de_curso *endstop_L_1; 

 int MAX_PWM = (10240000-1);


void setup() {
  Serial.begin(9600);

  BTS_1= new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_1->init();

  encoder_1 = new Encoder(A_pin_master_X,B_pin_master_X,0,Nominal_pulses,pitch_pulley,4); 

  endstop_L_1 = new Chave_fim_de_curso(chave_slave_L_X,2);
  endstop_L_1->init();
  
  endstop_R_1 = new Chave_fim_de_curso(chave_slave_R_X,3);
  endstop_R_1->init();

/*/ while(digitalRead(endstop_L_1->getPin())==HIGH){
    BTS_1->Set_L(255*pwm_master_cte);
  }/*/ 

}

void loop() {
  //Go to origin
  while(digitalRead(endstop_R_1->getPin())==HIGH){
    BTS_1->Set_R(MAX_PWM*pwm_master_cte);
    Serial.print(encoder_1->getPulses());
    Serial.print(",");
  }

  BTS_1->Set_R(0);
  encoder_1->setPulses(0);
  delay(100);
  
  // Go to max
  while(digitalRead(endstop_R_1->getPin())==HIGH){
    BTS_1->Set_L(MAX_PWM*pwm_master_cte);
    Serial.print(encoder_1->getPulses());
    Serial.print(",");
}

BTS_1->Set_L(0);
encoder_1->setPulses(0);
delay(100);
} 

