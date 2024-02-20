#include "Encoder.hpp"
#include "config.hpp"
int A_pin=13; // green cable
int B_pin=15; // white cable

Encoder *encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin_X_slave,B_pin_X_slave,0,600,40,4);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPosition());
  //Serial.println(encoder->getPulses());
  }

