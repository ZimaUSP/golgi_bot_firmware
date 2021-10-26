#include "PID.hpp"
#include "Encoder.hpp"
#include "Chave_fim_de_curso.hpp"


// Encoder pins
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
double kp = 0.3;
double ki = 0;
double kd = 2;
PID *pid;



double setPoint;

#include "serial_communication.hpp"
#include "config.hpp"

SerialCommunication *comu;

// Chave fim de curso

int chave_pin=34;
Chave_fim_de_curso *endstop;


int pot=35;
int input; 
 int output;
bool entrou= false;
void setup() {

  // Set point
  setPoint = 0;
  Serial.begin (9600);
  comu = new SerialCommunication("Posição SetPoint:");

  //Chave fim de curso
  endstop = new Chave_fim_de_curso(chave_pin,0);
  endstop->init();

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
}

void loop() {
  
  setPoint=-1110;
  if (!endstop->getBatente());
  {
  
    
  ledcWrite(0, outputR);
  ledcWrite(1, outputL);

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
  Serial.print(setPoint);
  Serial.print("   output: ");
  Serial.print(output);
  Serial.print("   counter :");
  Serial.println(encoder->getPulses());
  }}
