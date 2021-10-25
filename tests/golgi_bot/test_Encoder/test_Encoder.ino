#include "Encoder.hpp"

int A_pin=12;
int B_pin=14;
encoder *Encoder;

void setup() {     
  Serial.begin (9600);
  encoder = new Encoder(A_pin,B_pin,0);
  }
   
void loop() {
  Serial.println(encoder->getPulses());
  }

