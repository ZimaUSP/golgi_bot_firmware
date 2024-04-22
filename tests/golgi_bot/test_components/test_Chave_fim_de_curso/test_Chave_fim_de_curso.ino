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
  endstop_R_master_X = new Chave_fim_de_curso(chave_master_R_X,chave_channel_master_R_X);
  endstop_R_master_X->init();

  endstop_L_master_X = new Chave_fim_de_curso(chave_master_L_X,chave_channel_master_L_X);
  endstop_L_master_X->init();

  // Batente slave
  endstop_R_slave_X = new Chave_fim_de_curso(chave_slave_R_X,chave_channel_slave_R_X);
  endstop_R_slave_X->init();

  endstop_L_slave_X = new Chave_fim_de_curso(chave_slave_L_X,chave_channel_slave_L_X);
  endstop_L_slave_X->init();

  // Batente Z
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,chave_channel_R_Z);
  endstop_R_Z->init();


  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,chave_channel_L_Z);
  endstop_L_Z->init(); 

  }
   
void loop() {
  if (endstop_L_slave_X->getBatente())
  {
    Serial.println("Batente L slave X");
  } if (endstop_R_slave_X->getBatente())
  {
    Serial.println("Batente R slave X");
  } if (endstop_L_master_X->getBatente())
  {
    Serial.println("Batente L master X");
  } if (endstop_R_master_X->getBatente())
  {
    Serial.println("Batente R master X");
  } if (endstop_R_Z->getBatente())
  {
    Serial.println("Batente R Z");
  } if (endstop_L_Z->getBatente())
  {
    Serial.println("Batente L Z");
  }
  }
