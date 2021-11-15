

// Bomba axis 
#include "Bomba.hpp"
int bomba_pin=32;//IN2
Bomba *Bomba_Y;
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

  // Bomba
  Bomba_Y= new Bomba( bomba_pin);
  Bomba_Y->init();
  Bomba_Y->turn_on();
  
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

    Serial.println("oi");
   if(Serial.available()){
     comu->read_data();
    comand=atoi(string_to_char(comu->get_received_data()));
    if(comand==1){
    Bomba_Y->turn_on();
    }
    else{
    Bomba_Y->turn_off();
    }
    
  }
}
