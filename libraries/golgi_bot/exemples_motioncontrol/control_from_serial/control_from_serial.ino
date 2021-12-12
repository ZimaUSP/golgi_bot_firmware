#include <Motion_control.h>

#define PIN1 2      // channel 1
#define PIN2 7      // channel 2
#define PULSES 600   // 600 pulses per revolution
#define PITCH 40    // [mm]
#define MODE 0 // x1 resolution

#define OUTPUT_PIN 6 // IO vque envia o sinal PWM
#define IN1_PIN 4 // IO 1 de controle do sentido
#define IN2_PIN 5 // IO 2 de controle do sentido

Position pos = {0, 0, 0};

Axis axis(OUTPUT_PIN, IN1_PIN, IN2_PIN);
Encoder encoder(PIN1, PIN2, PULSES, PITCH, MODE);

Controller controller(&axis, &encoder);

void setup() {

  Serial.begin(9600);
  
  axis.setOpMode(1); // Axis ON
  controller.setController(4); // PID
  controller.setOpMode(1); // Controller ON

}

void loop() {

  if(Serial.available() > 0){
    
    pos = readSerial();
    controller.setPosition(pos);
    
  }

  controller.runController();

}

Position readSerial(){

  Position temp;
  temp.Axis1 = Serial.parseFloat();

  return temp;

}
