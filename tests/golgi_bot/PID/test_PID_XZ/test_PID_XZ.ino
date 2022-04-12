// Encoder X axis 
#include "Encoder.hpp"
int A_pin_X=22; // Green cable
int B_pin_X=23; // White cable
Encoder *encoder_X;

// BTS X axis 
#include "H_bridge_controller.hpp"
int R_pin_X=26;
int L_pin_X=27;
int PWM_frequency = 40000;
int PWM_resolution = 8;
int R_channel_X=1;
int L_channel_X=2;
int PWM_R_X=0;
int PWM_L_X=0;
H_bridge_controller *BTS_X;

// Chave fim de curso X axis
#include "Chave_fim_de_curso.hpp"
int chave_L_X=36; 
int chave_R_X=39; 
Chave_fim_de_curso *endstop_L_X; 
Chave_fim_de_curso *endstop_R_X; 

//PID X axis constants
#include "PID.hpp"
double kp_x = 0.2;
double ki_x = 0;
double kd_x= 2.5;
int output_x;
double setPoint_x;
PID *PID_X; 

//Count MAX
int MAX_PULSES_X=12000;


// EIXO Z 

// Encoder Z axis 
#include "Encoder.hpp"
int A_pin_Z=19; // green cable
int B_pin_Z=21; // white cable
Encoder *encoder_Z;

// BTS Z axis 
#include "H_bridge_controller.hpp"
int R_pin_Z=17;
int L_pin_Z=18;
int R_channel_Z=3;
int L_channel_Z=4;
int PWM_R_Z=0;
int PWM_L_Z=0;
H_bridge_controller *BTS_Z;

// Chave fim de curso Z axis
#include "Chave_fim_de_curso.hpp"
int chave_L_Z=34; 
int chave_R_Z=35; 
Chave_fim_de_curso *endstop_L_Z; 
Chave_fim_de_curso *endstop_R_Z; 

//PID Z axis constants
#include "PID.hpp"
double kp_Z = 0.1;
double ki_Z = 0;
double kd_Z= 1.5; 
int output_z;
double setPoint_z;
PID *PID_Z; 

//Count MAX
int MAX_PULSES_Z =12000;


//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;

void setup() {
  // Set point
  setPoint_x = 0;

  setPoint_z = 0;
  

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
  BTS_X= new H_bridge_controller( R_pin_X, L_pin_X, PWM_frequency, PWM_resolution, R_channel_X, L_channel_X);
  BTS_X->init();

  BTS_Z= new H_bridge_controller( R_pin_Z, L_pin_Z, PWM_frequency, PWM_resolution, R_channel_Z, L_channel_Z);
  BTS_Z->init();

  // Encoder
  encoder_X = new Encoder(A_pin_X,B_pin_X,0);
  encoder_X->init();

  encoder_Z = new Encoder(A_pin_Z,B_pin_Z,1);
  encoder_Z->init();

  //Set origin
  Set_origin_x();
  
  Set_origin_z();

  //PID
  PID_X = new PID(kp_x,ki_x,kd_x);
  
  PID_Z = new PID(kp_Z,ki_Z,kd_Z);

}

void loop() {
    // Recive Set point
    read_setpoint();

    // PID X
    output_x = PID_X->computePID(encoder_X->getPulses(),setPoint_x);

    // PID Z
    output_z = PID_Z->computePID(encoder_Z->getPulses(),setPoint_z);


    // Setting direction of motion acording to output_x PID
    if (output_x > 255) {
        output_x = 255;
    }
    if (output_x < -255) {
        output_x = -255;
    }
    if (output_x < 0) {
        PWM_L_X = 0;
        PWM_R_X = -output_x;
    } else {
        PWM_R_X = 0;
        PWM_L_X = output_x;
    }
    //Setting BTS X axis PWM channel
    BTS_X->SetPWM_R(PWM_R_X);
    BTS_X->SetPWM_L(PWM_L_X);

    // Setting direction of motion acording to output_z PID
    if (output_z > 255) {
        output_z = 255;
    }
    if (output_z < -255) {
        output_z = -255;
    }
    if (output_z < 0) {
        PWM_L_Z = 0;
        PWM_R_Z = -output_z;
    } else {
        PWM_R_Z = 0;
        PWM_L_Z = output_z;
    }
    //Setting BTS Z axis PWM channel
    BTS_Z->SetPWM_R(PWM_R_Z);
    BTS_Z->SetPWM_L(PWM_L_Z);

    // Debug print X
    //Serial.print("setPoint_x: ");
    //Serial.println(setPoint_x);
    //Serial.print("output_x: ");
    //Serial.println(output_x);
    Serial.print("counter X :");
    Serial.println(encoder_X->getPulses());

    // Debug print Z
    //Serial.print("Setpoint_z: ");
    //Serial.println(setPoint_z);
    //Serial.print("output: ");
    //Serial.println(output_z);
    Serial.print("    counter Z :");
    Serial.println(encoder_Z->getPulses());
}
char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}

void Set_origin_x(){
  while (digitalRead(chave_R_X)==HIGH)
  {
    PWM_R_X = 100;
    PWM_L_X = 0;
    BTS_X->SetPWM_R(PWM_R_X);
    BTS_X->SetPWM_L(PWM_L_X);
  }
  encoder_X->setPulses(0);
  PWM_R_X = 0;
  PWM_L_X = 0;
  BTS_X->SetPWM_R(PWM_R_X);
  BTS_X->SetPWM_L(PWM_L_X);
}

void Set_origin_z(){
  while (digitalRead(chave_R_Z)==HIGH)
  {
    PWM_R_Z = 100;
    PWM_L_Z = 0;
    BTS_Z->SetPWM_R(PWM_R_Z);
    BTS_Z->SetPWM_L(PWM_L_Z);
  }
  encoder_Z->setPulses(0);
  PWM_R_Z = 0;
  PWM_L_Z = 0;
  BTS_Z->SetPWM_R(PWM_R_Z);
  BTS_Z->SetPWM_L(PWM_L_Z);
  
}
void read_setpoint(){
    char *ptr;
    
   if(Serial.available()){

      comu->read_data();
      char* recived=string_to_char(comu->get_received_data());
      ptr = strtok(recived, "-");
      setPoint_x=atoi(string_to_char(ptr)); 
      ptr = strtok (NULL, "-");
      setPoint_z=atoi(string_to_char(ptr));  
      
    }
    }
