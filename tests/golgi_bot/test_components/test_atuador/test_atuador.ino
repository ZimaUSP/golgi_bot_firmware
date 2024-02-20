// atuador axis 
#include "Atuador.hpp"
int Extend_pin=33; //IN4
int Contract_pin=25; //IN3
Atuador *Atuador_Y;
int comand;

//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;

void setup() {

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição setPoint_x:");

  // Atuador
  Atuador_Y= new Atuador( Extend_pin,Contract_pin);
  Atuador_Y->init();
  
}

void loop() {
  Atuador_Y->Extend();
  delay(5000);
  Atuador_Y->Contract();
  delay(5000);
  Serial.println(".");
    }


char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}
void read_command(){
  Serial.println(".");
   if(Serial.available()){
     comu->read_data();

    comand=atoi(string_to_char(comu->get_received_data()));
    if(comand==1){
    Atuador_Y->Extend();
    
    Serial.println("extend");
    }
    if(comand==2){
    Atuador_Y->Contract();
    
    Serial.println("contrac");
    }
    if(comand==3){
    Atuador_Y->Stop();
    
    Serial.println("stop");
    }
    
  }
}
