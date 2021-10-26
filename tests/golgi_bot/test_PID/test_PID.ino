// Encoder pins
#include "Encoder.hpp"
int A_pin=12;
int B_pin=14;
Encoder *encoder;

// BTS PINS
int outputR = 0;
int outputL = 0;
#define PWMR                      32    //Pino do arduino que será ligado o BTS7960
#define PWML                      33    //Pino do arduino que será ligado o BTS7960
#define PWM1_Res   8
#define PWM1_Freq  40000

//PID constants
#include "PID.hpp"
double kp = 1;
double ki = 0;
double kd = 2;
PID *pid;
int input; 
int output;
double setPoint;

//Count MAX

int MAX_PULSES =12543;

// Chave fim de curso
#include "Chave_fim_de_curso.hpp"
int chave_L=34;
int chave_R=27;
Chave_fim_de_curso *endstop_L;
Chave_fim_de_curso *endstop_R;

//Serial comunication
#include "serial_communication.hpp"
#include "config.hpp"
SerialCommunication *comu;
#include <string>
#include <cstring>

bool entrou= false;
void setup() {
  // Set point
  setPoint = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop_L = new Chave_fim_de_curso(chave_L,0);
  endstop_L->init();
  endstop_R = new Chave_fim_de_curso(chave_R,1);
  endstop_R->init();

  // Encoder
  encoder = new Encoder(A_pin,B_pin,0);
  encoder->init();

  // BTS
  ledcAttachPin(PWMR, 0);
  ledcSetup(0, PWM1_Freq, PWM1_Res);

  ledcAttachPin(PWML, 1);
  ledcSetup(1, PWM1_Freq, PWM1_Res);

  //PID
  pid = new PID(kp,ki,kd);
  while (digitalRead(chave_R)==HIGH)
  {
    outputR = 0;
    outputL = 250;
    ledcWrite(0, outputR);
    ledcWrite(1, outputL);

  }
  outputR = 0;
  outputL = 0;
  encoder->setPulses(0);
}

void loop() {
  ledcWrite(0, outputR);
  ledcWrite(1, outputL);
  
  // Set point
  if(Serial.available()){
    comu->read_data();
    setPoint=atoi(string_to_char(comu->get_received_data()));
    if(setPoint>MAX_PULSES){
      setPoint=MAX_PULSES;
    }
    if(setPoint<0){
      setPoint=0;
    }
  }
  
  
  if(endstop_R->getBatente()||endstop_L->getBatente()){
      if(endstop_R->getBatente()){
      Serial.println("Batente Direita");
      }else{
      Serial.println("Batente Esquerda");
    }}
    

      // PID
    input = encoder->getPulses();
    output = pid->computePID(input,setPoint);
    if (output > 255) {
      output = 255;
    }
    if (output < -255) {
      output = -255;
    }
    if (output < 0) {

      outputR = 0;
      outputL = -output;
    } else {
      outputL = 0;
      outputR = output;
    }
      Serial.print("Setpoint: ");
    Serial.println(setPoint);
    Serial.print("output: ");
    Serial.println(output);
    Serial.print("counter :");
    Serial.println((encoder->getPulses()));
    }

float pos(int rev) { //Retorna a posição em centimetros
  return rev * (42.5 / 12000);
}
char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}
