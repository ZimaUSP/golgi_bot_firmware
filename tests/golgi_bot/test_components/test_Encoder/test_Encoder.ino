#include "Encoder.hpp"
#include "config.hpp"


Encoder *encoder;

void setup() {            
  Serial.begin (9600);
  encoder = new Encoder(A_pin_master_X, B_pin_master_X, 0, Nominal_pulses, pitch_pulley_master, 4); 
  //encoder = new Encoder(A_pin_Z,B_pin_Z,0,600,40,4);
  //encoder = new Encoder(B_pin_slave_X, A_pin_slave_X, 0, Nominal_pulses, pitch_pulley_slave, 4);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPosition());
  
  //Serial.println(encoder->getPulses());
  }
