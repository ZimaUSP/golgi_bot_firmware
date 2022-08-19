#include "Chave_fim_de_curso.hpp"
#include "config.hpp"

// Chave fim de curso X axis
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 



void setup() {     
  Serial.begin (9600);
  endstop = new Chave_fim_de_curso(chave_pin,0);
  endstop->init();

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();

  }
   
void loop() {
  if (endstop_L_X->getBatente())
  {
    Serial.println("Batente L X");
  }else if (endstop_R_X->getBatente())
  {
    Serial.println("Batente R X");
  }else if (endstop_L_Z->getBatente())
  {
    Serial.println("Batente L Z");
  }else if (endstop_R_Z->getBatente())
  {
    Serial.println("Batente R Z");
  }else{
    Serial.println("sem batente");
  }
  }
