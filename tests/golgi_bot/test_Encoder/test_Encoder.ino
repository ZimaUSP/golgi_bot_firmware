#include "Encoder.hpp"
int A_pin=22; // green cable
int B_pin=23; // white cable

Encoder *encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0,600,40,4);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPosition());
  Serial.println(encoder->getPulses());
  }

