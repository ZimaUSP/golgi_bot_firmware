#include "Chave_fim_de_curso.hpp"

int chave_pin=12;
Chave_fim_de_curso *Chave_fim_de_curso;

void setup() {     
  Serial.begin (9600);
  Chave_fim_de_curso = new Chave_fim_de_curso(chave_pin,0);
  Chave_fim_de_curso->init()
  }
   
void loop() {
  Serial.println(Chave_fim_de_curso->getBatente());
  }

