// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//Controller lib

//#include "Controller.hpp"
#include "Controller.hpp"
// EIXO X

// Encoder X axis 
Encoder *encoder_master_X;
Encoder *encoder_slave_X;

// Axis X
Axis *Axis_master_X;
Axis *Axis_slave_X;
int error_axis;

// BTS X axis 
H_bridge_controller *BTS_master_X;
H_bridge_controller *BTS_slave_X;

// Chave fim de curso X axis

// master
Chave_fim_de_curso *endstop_master_L_X; 
Chave_fim_de_curso *endstop_master_R_X;

// Slave
Chave_fim_de_curso *endstop_slave_L_X;
Chave_fim_de_curso *endstop_slave_R_X;

// PID X axis 
PID *PID_master_X;
PID *PID_slave_X;

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


// CONTROLLER

Controller *Golgi_bot;


//Serial comunication
#include "serial_communication.hpp"
SerialCommunication* comu;
#include <string>
#include <cstring>
int pos_x[X_max_index]={85,225,364,654};
int pos_z[Z_max_index]={55,190,279};
double X_pos;
double Z_pos;
int counter;

//STATE
char STATE = 0 ; 


#define led 2

void setup() {
   
  //Serial Comunication
  Serial.begin (SERIAL_VEL);

  //Chave fim de curso

  // Master X
  endstop_master_L_X = new Chave_fim_de_curso(chave_master_L_X,chave_channel_master_L_X);
  endstop_master_L_X->init();
  endstop_master_R_X = new Chave_fim_de_curso(chave_master_R_X,chave_channel_master_R_X);
  endstop_master_R_X->init();
    
  // Slave X
  endstop_slave_L_X = new Chave_fim_de_curso(chave_slave_L_X, chave_channel_slave_L_X);
  endstop_slave_L_X->init();
  endstop_slave_R_X = new Chave_fim_de_curso(chave_slave_R_X, chave_channel_slave_R_X);
  endstop_slave_R_X->init();

  // Z
  endstop_L_Z = new Chave_fim_de_curso(chave_L_Z, chave_channel_L_Z);
  endstop_L_Z->init();
  endstop_R_Z = new Chave_fim_de_curso(chave_R_Z, chave_channel_R_Z);
  endstop_R_Z->init();

  // BTS

  BTS_master_X = new H_bridge_controller(L_pin_master_X, R_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();
  
  BTS_slave_X = new H_bridge_controller(L_pin_slave_X, R_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

  BTS_Z= new H_bridge_controller(R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);      //esses canais talvez deem problema
  BTS_Z->init();

  // Encoder

  encoder_master_X =new Encoder(A_pin_master_X ,B_pin_master_X , 0, Nominal_pulses, pitch_pulley_master, 4);
  encoder_master_X->init();
  
  encoder_slave_X = new Encoder(B_pin_slave_X, A_pin_slave_X, 1, Nominal_pulses, pitch_pulley_slave, 4);
  encoder_slave_X->init();

  encoder_Z =new Encoder(A_pin_Z,B_pin_Z,2,600,40,4);
  encoder_Z->init();

   // Atuador
  Atuador_Y= new Atuador(Extend_pin,Contract_pin);
  Atuador_Y->init();
  Atuador_Y->Contract();
  delay(DELAY_EXTEND);
  Atuador_Y->Stop();

   // Bomba
  Bomba_Y= new Bomba(bomba_pin);
  Bomba_Y->init();

  //PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_slave_X = new PID(kp_slave_x, ki_slave_x, kd_slave_x, i_saturation_slave_x);
  
  PID_Z = new PID(kp_z,ki_z,kd_z,i_saturation_z);

  //Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_master_tolerance, pwm_master_cte, false);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_slave_tolerance, pwm_slave_cte, false);
  
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, PID_Z, Z_MAX_VEL, PWM_resolution_channel, Z_tolerance, pwm_cte_Z, false);

  //Creating Controller
  Golgi_bot = new Controller(Axis_master_X, Axis_slave_X, Axis_z, Bomba_Y, Atuador_Y);

  //Setting up the right inicital state
  Golgi_bot->reset_Y(DELAY_CONTRACT);


  //int sempre = 1;
  //while (sempre = 1) {
    //Serial.println(encoder_master_X->getPosition());
    //BTS_master_X->Set_R(125);
    //BTS_slave_X->Set_R(125);
    //Serial.println(encoder_master_X->getPosition());
  //}


  Golgi_bot->go_origin(true, true);
  Golgi_bot->go_max(true, true, true);      // test if it needs to go max, or if going once to orign works
  Golgi_bot->go_origin(true, true);


  Serial.println("STAND-BY");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop() {
  switch(STATE) {
      case STAND_BY :
        // Recive Set point
        read_setpoint();
        return;
      case GOING :
        //Moves Controller
        Golgi_bot->move();
        Axis_slave_X->setGoal(Axis_master_X->position());
        //Code does not work without this delay (?)
        delay(2);
        check_position();
        return;
      case GETING_MEDICINE :
        Golgi_bot->get_medicine(DELAY_EXTEND,DELAY_CONTRACT);
        STATE=DROPING_MEDICINE;
        Serial.println("DROPING_MEDICINE");
        return;
      case DROPING_MEDICINE :
        Golgi_bot->drop_medicine();
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
  String id_remedio;
  if(Serial.available()){

      STATE=GOING;
      Serial.println("GOING"); 
      
      //digitalWrite(led, HIGH);
      //delay(1000);
      //digitalWrite(led, LOW);
      
      id_remedio = Serial.readString();
      id_remedio.trim();
      int index_medicine = 0;
      if(id_remedio.equals("17424")){ 
        //digitalWrite(led, HIGH);
        //delay(2000);
        //digitalWrite(led, LOW);
        index_medicine = 0;

      }
      else if(id_remedio.equals("17292")){
        index_medicine = 1;
      }
      else if(id_remedio.equals("27198")){
        index_medicine = 2;
      }
      else if(id_remedio.equals("18899")){
        index_medicine = 3;
      }
      
      
      counter=0;
      for(int j=0; j< Z_max_index; j++){
        for(int i =0; i < X_max_index; i++){
          counter=counter+1;
          if (counter==index_medicine){
            Z_pos=pos_z[j];
            X_pos=pos_x[i];
          }
        }
      }
      
    Serial.print("X goal:");
    Serial.println(X_pos);      
    Serial.print("Z goal:");
    Serial.println(Z_pos);
    Golgi_bot->setGoal(X_pos, X_pos, Z_pos);
  }

}

void check_position(){
  if(Golgi_bot->onGoal()){
    STATE=GETING_MEDICINE;
    Serial.println("GETING_MEDICINE");
    Serial.print(Golgi_bot->positionPoint()[0]);
    Serial.print(",");
    Serial.println(Golgi_bot->positionPoint()[1]);
    Serial.print(",");
    Serial.println(Golgi_bot->positionPoint()[2]);
    Golgi_bot->stop(true,true,true);
  }
}