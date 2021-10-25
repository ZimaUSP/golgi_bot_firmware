#include PID.hpp

// Encoder pins
volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int A_pin = 12;
int B_pin = 14;

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
PID *PID;
//Potenciometro
int count;
int temporary;
int a;
int potpin= 34;
int buff[10];

// Chave fim de curso

int chave=34;

bool entrou= false;
void setup() {
  // Set point
  setPoint = 0;
  Serial.begin (9600);

  // Encoder

  pinMode(A_pin, INPUT_PULLUP); // internal pullup input pin 2

  pinMode(B_pin, INPUT_PULLUP); // internalเป็น pullup input pin 3

  attachInterrupt(A_pin, ai0, RISING);//A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.

  attachInterrupt(B_pin, ai1, RISING);//B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.

  // BTS
  ledcAttachPin(PWMR, 0);
  ledcSetup(0, PWM1_Freq, PWM1_Res);

  ledcAttachPin(PWML, 1);
  ledcSetup(1, PWM1_Freq, PWM1_Res);

  // Chave fim de curso
  pinMode(chave, INPUT_PULLUP);
  attachInterrupt(chave, fim_curso, FALLING);

  //PID
  PID = new *PID(kp,ki,kd);
}

void loop() {
  ledcWrite(0, outputR);
  ledcWrite(1, outputL);
  
  // Set point
  count=0;
  temporary=0;
  while(Serial.available()){
    entrou= true;
    buff[count]=(Serial.read()-48);
    count=count+1;
  }
  if(entrou){
    for(int i=count;i>=0;i=i-1){
      temporary=buff[i]*pow(10, count-i-1)+temporary;
    }
  setPoint=temporary;
  entrou= false;
  }

 
  // count pulses Ecoder
  check_pulse();
  
  
  // PID
  input = temp;
  output = PID->computePID(input);
  if (output > 255) {
    output = 255;
  }
  if (output < -255) {
    output = -255;
  }
  if (output < 0) 
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
  Serial.print("error: ");
  Serial.println(lastError);
  Serial.print("counter :");
  Serial.println(temp);
}
float pos(int rev) { //Retorna a posição em centimetros
  return rev * (41 / 12000);
}
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(B_pin) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(A_pin) == LOW) {
    counter--;
  } else {
    counter++;
  }
}
// Ecoder
void check_pulse(){
  
  if( counter != temp ){ // detects motion by difference
  
  temp = counter;
  }

  }
  
void fim_curso() {
  temp=0;
  temp=0;
  setPoint=0;
}
double computePID(double inp) {

  currentTime = millis();                //get current time
  elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation

  error = setPoint - inp;                                // determine error
  cumError += error * elapsedTime;                // compute integral
  rateError = (error - lastError) / elapsedTime; // compute derivative

  double out = kp * error + ki * cumError + kd * rateError;          //PID output

  lastError = error;                                //remember current error
  previousTime = currentTime;                        //remember current time

  return out;                                        //have function return the PID output
}