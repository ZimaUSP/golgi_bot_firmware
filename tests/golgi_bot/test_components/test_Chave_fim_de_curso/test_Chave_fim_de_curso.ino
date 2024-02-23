#include "Chave_fim_de_curso.hpp"
#include "config.hpp"

// Chave fim de curso X axis master
Chave_fim_de_curso *endstop_R_master_X; 
Chave_fim_de_curso *endstop_L_master_X; 

// Chave fim de curso X axis slave
Chave_fim_de_curso *endstop_R_slave_X; 
Chave_fim_de_curso *endstop_L_slave_X; 

// Chave fim de curso Z axis 
Chave_fim_de_curso *endstop_R_Z;
Chave_fim_de_curso *endstop_L_Z;



void setup() {     
  Serial.begin (9600);

  // Batente master
  endstop_R_master_X = new Chave_fim_de_curso(chave_master_R_X,0);
  endstop_R_master_X->init();

  endstop_L_master_X = new Chave_fim_de_curso(chave_master_L_X,1);
  endstop_L_master_X->init();

  // Batente slave
  endstop_R_slave_X = new Chave_fim_de_curso(chave_slave_R_X,2);
  endstop_R_slave_X->init();

  endstop_L_slave_X = new Chave_fim_de_curso(chave_slave_L_X,3);
  endstop_L_slave_X->init();

  // Batente Z
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,8);
  endstop_R_Z->init();


  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,9);
  endstop_L_Z->init(); 

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
    Serial.println("Batente L master X");
  }else if (endstop_R_master_X->getBatente())
  {
    Serial.println("Batente R master X");
  }  /*else if (endstop_R_Z->getBatente())
  {
    Serial.println("Batente R Z");
  }else if (endstop_L_Z->getBatente())
  {
    Serial.println("Batente L Z");
  }
  */
  }
