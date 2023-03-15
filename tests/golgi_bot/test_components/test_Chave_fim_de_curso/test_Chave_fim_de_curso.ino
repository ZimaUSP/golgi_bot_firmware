#include "Chave_fim_de_curso.hpp"
#include "config.hpp"

// Chave fim de curso X axis
Chave_fim_de_curso *endstop_R_master_X; 
Chave_fim_de_curso *endstop_L_master_X; 

// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_R_slave_X; 
Chave_fim_de_curso *endstop_L_slave_X; 



void setup() {     
  Serial.begin (9600);

  // batente master
  endstop_R_master_X = new Chave_fim_de_curso(chave_master_R_X,0);
  endstop_R_master_X->init();

  endstop_L_master_X = new Chave_fim_de_curso(chave_master_L_X,1);
  endstop_L_master_X->init();

  // Batente slave
  endstop_R_slave_X = new Chave_fim_de_curso(chave_slave_R_X,2);
  endstop_R_slave_X->init();

  endstop_L_slave_X = new Chave_fim_de_curso(chave_slave_L_X,3);
  endstop_L_slave_X->init();
  

  }
   
void loop() {
  if (endstop_L_slave_X->getBatente())
  {
    Serial.println("Batente L slave X");
  } if (endstop_R_slave_X->getBatente())
  {
    Serial.println("Batente R slave X");
  }else if (endstop_L_master_X->getBatente())
  {
    Serial.println("Batente L master Z");
  }else if (endstop_R_master_X->getBatente())
  {
    Serial.println("Batente R master Z");
  }
  }
