#include "Chave_fim_de_curso.hpp"

int chave_pin=12;
Chave_fim_de_curso *endstop;

void setup() {     
  Serial.begin (9600);
  pinMode(32,OUTPUT);
  digitalWrite(32,HIGH);
  endstop = new Chave_fim_de_curso(chave_pin,0);
  endstop->init();
  }
   
void loop() {
   Serial.println(endstop->getPin());
  if (endstop->getBatente())
  {
    Serial.println("Batente");
  }
  
  }

