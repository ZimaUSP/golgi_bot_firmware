#include "Chave_fim_de_curso.hpp"

int chave_pin=34;
Chave_fim_de_curso *endstop;

void setup() {     
  Serial.begin (9600);
  endstop = new Chave_fim_de_curso(chave_pin,0);
  endstop->init();
  }
   
void loop() {
  if (endstop->getBatente())
  {
    Serial.println("Batente");
  }else{
    Serial.println("sem batente");
  }
  
  }
