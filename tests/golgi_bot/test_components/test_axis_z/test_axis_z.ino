
// EIXO Z 
#include "config.hpp"

// BTS Z axis 
#include "H_bridge_controller.hpp"


int R_channel_Z=3; //conferir na hora dos teste pq está diferente no config
int L_channel_Z=4;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"

Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);

  // Setup Batentes
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();

  // Setup H_bridge
  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();
  
  // Start at origin
  go_origin_z();

}

void loop(){
    go_max_z();

    delay(5000);
    
    go_origin_z();
}


void go_origin_z(){   
  while (digitalRead(chave_L_Z)!=HIGH){
    BTS_Z->Set_R(100);
    return;
  }   
  BTS_Z->SetPWM_R(0);
  Serial.println("Origin");
}

void go_max_z(){   
  while (digitalRead(chave_L_Z)!=HIGH){
    BTS_Z->Set_L(100);
    return;
  }   
  BTS_Z->SetPWM_L(0);
  Serial.println("Max Position");
}


/*/

Códigos do test_PID_Z para recolocar caso algo de errado

void loop() {
  switch(STATE) {
      case STAND_BY :
        Serial.println("STAND-BY");
        // Recive Set point
        read_setpoint();
        return;
      case GOING :
        // PID Z
        output_z = PID_Z->computePID(encoder_Z->getPulses(),setPoint_z);
        // Setting direction of motion acording to output_z PID
        move_Z();

        //Code does not work without this delay (?)
        delay(2);
        
        check_position();
        
        last_z_count=encoder_Z->getPulses();
        return;
   }
    
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void Set_origin_z(){
  while (digitalRead(chave_R_Z)==HIGH){
    BTS_Z->Set_R(100);
  }
  encoder_Z->setPulses(0);
  BTS_Z->SetPWM_R(0);
}

void Set_max_z(){
  while (digitalRead(chave_L_Z)==HIGH){
    BTS_Z->Set_L(100);
  }
  MAX_PULSES_Z = encoder_Z->getPulses();
  BTS_Z->SetPWM_L(0);
}

void move_Z(){   
    if (output_z < 0) {
      if (output_z < -255) {
        output_z = -255;
      }
      BTS_Z->Set_R(-output_z);
    } else {
        if (output_z > 255) {
        output_z = 255;
        }
        BTS_Z->Set_L(output_z);
    }
}

 void read_setpoint(){
    if(Serial.available()){
      
      STATE=GOING;

      char *ptr;
      comu->read_data();
      char* recived=string_to_char(comu->get_received_data());

      if (atoi((recived))>MAX_PULSES_Z){
       setPoint_z=MAX_PULSES_Z;
      }else{
      setPoint_z=atoi(string_to_char(ptr));
      }
      PID_Z->reset();
      last_z_count=-5000;
    }
}

void check_position(){
  if((encoder_Z->getPulses()==last_z_count )){
          BTS_Z->SetPWM_R(0);
          STATE=STAND_BY;
          Serial.print("counter Z :");
          Serial.println(encoder_Z->getPulses());
          return;
        }
}
/*/
