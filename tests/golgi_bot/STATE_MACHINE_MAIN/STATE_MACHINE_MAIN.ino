// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//Controller lib

#include "Controller.hpp"
Controller *Golgi_BOT;

// EIXO X

Axis *Axis_x;

// Encoder X axis 
Encoder *encoder_X;

// BTS X axis 
H_bridge_controller *BTS_X;

// Chave fim de curso X axis
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

//PID X axis constants
PID *PID_X; 


// EIXO Z

Axis *Axis_z;

// Encoder Z axis 
Encoder *encoder_Z;

// BTS Z axis 
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
PID *PID_Z; 


// EIXO Y 


// Bomba axis 
Bomba *Bomba_Y;

// atuador axis 
Atuador *Atuador_Y;


//Serial comunication
SerialCommunication *comu;
double X_pos;
double Z_pos;

//STATE
char STATE = 0 ; 

//Tempo
int sample_time=500;
unsigned long current_time;
unsigned long previus_time = 0;
double delta_time;

void setup() {
   
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop_L_X = new Chave_fim_de_curso(chave_L_X,0);
  endstop_L_X->init();
  endstop_R_X = new Chave_fim_de_curso(chave_R_X,1);
  endstop_R_X->init();

  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z,2);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z,3);
  endstop_R_Z->init();

  // BTS
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_X, L_channel_X);
  BTS_X->init();

  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);
  BTS_Z->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0,Nominal_pulses,pitch_pulley,Mode);
  encoder_X->init();

  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1,Nominal_pulses,pitch_pulley,Mode);
  encoder_Z->init();

   // Atuador
  Atuador_Y= new Atuador( Extend_pin,Contract_pin);
  Atuador_Y->init();
  
   // Bomba
  Bomba_Y= new Bomba(bomba_pin);
  Bomba_Y->init();


  //PID
  PID_X = new PID(kp_x,ki_x,kd_x);
  
  PID_Z = new PID(kp_z,ki_z,kd_z);

  //Creating Axis
  Axis_x= new Axis(encoder_X, BTS_X, endstop_R_X, endstop_R_X, PID_X, X_MAX_VEL, PWM_resolution_channel, X_size, X_tolerance);
  
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_R_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_size, Z_tolerance);

  Golgi_BOT = new Controller(Axis_x, Axis_z, Bomba_Y, Atuador_Y);

  Golgi_BOT->reset_Y();

  Golgi_BOT->go_origin();

  Serial.println("STAND-BY");

}

void loop() {
  switch(STATE) {
      case STAND_BY :
        // Recive Set point
        read_setpoint();
        return;
      case GOING :
        //Moves Controller
        Golgi_BOT->move();
        //Code does not work without this delay (?)
        delay(2);
        check_position();
        return;
      case GETING_MEDICINE :
        Golgi_BOT->get_medicine();
        STATE=DROPING_MEDICINE;
        Serial.println("DROPING_MEDICINE");
        return;
      case DROPING_MEDICINE :
        Golgi_BOT->drop_medicine();
        STATE=STAND_BY;
        Serial.println("STAND-BY");
        return;
   }  
}

char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void read_setpoint(){
    if(Serial.available()){
      STATE=GOING;
      Serial.println("GOING");

      comu->read_data();
      char* recived=string_to_char(comu->get_received_data());
      char *ptr;

      ptr = strtok(recived, "-");
      X_pos=atoi(string_to_char(ptr);
     
      ptr = strtok (NULL, "-");
      Z_pos=atoi(string_to_char(ptr);
      
      Golgi_BOT->setGoal(X_pos,Z_pos);
    }
}

void check_position(){
  if(Golgi_BOT->onGoal){
    STATE=GETING_MEDICINE;
    Serial.println("GETING_MEDICINE");
    Serial.println(Golgi_BOT->positionPoint());
  }
}     
  

