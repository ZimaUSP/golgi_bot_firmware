#include "Encoder.hpp"
#include "config.hpp"


Encoder *encoder_z;
Encoder *encoder_mas;
Encoder *encoder_slave;

void setup() {            
  Serial.begin (9600);
  encoder_mas = new Encoder(A_pin_master_X, B_pin_master_X, 0, Nominal_pulses, pitch_pulley_master, 4); 
  encoder_z = new Encoder(A_pin_Z,B_pin_Z,2,600,40,4);
  encoder_slave = new Encoder(A_pin_slave_X, B_pin_slave_X, 1, Nominal_pulses, pitch_pulley_slave, 4);
  encoder_mas->init();
  encoder_slave->init();
  encoder_z->init();
  }
   
void loop() {
  Serial.print(encoder_mas->getPosition());
  Serial.print(" ,");
  Serial.print(encoder_slave->getPosition());
  Serial.print(" ,");
  Serial.println(encoder_z->getPosition());
  
  //Serial.println(encoder->getPulses());
  }
