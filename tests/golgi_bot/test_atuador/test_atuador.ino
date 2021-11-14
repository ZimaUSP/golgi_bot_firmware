// atuador axis 
#include "Atuador.hpp"
int Extend_pin=13;
int Contract_pin=12;
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
  read_command();
    }


char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}
void read_command(){
   if(Serial.available()){
     comu->read_data();

    comand=atoi(string_to_char(comu->get_received_data()));
    if(comand==1){
    Atuador_Y->Extend();
    }
    if(comand==2){
    Atuador_Y->Contract();
    }
    if(comand==3){
    Atuador_Y->Stop();
    }
    
  }
}
