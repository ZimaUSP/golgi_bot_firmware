#include "Encoder.hpp"

int A_pin=22;
int B_pin=23;
Encoder *encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0);
  encoder->init();
  }
   
void loop() {
  Serial.println(encoder->getPulses());
  }

