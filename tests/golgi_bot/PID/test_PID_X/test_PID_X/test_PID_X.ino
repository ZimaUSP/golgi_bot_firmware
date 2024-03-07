
int sample = 4;


// EIXO X
#include "config.hpp"
#include "Controller.hpp"

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

Controller *controller;

Bomba *Bomba_Y;
Atuador *Atuador_Y;


//Serial comunication
#include "serial_communication.hpp"
#include <string>
#include <cstring>
SerialCommunication *comu;
double setPoint_master_X;
double setPoint_slave_X;

// Estados
char STATE = 0 ; // 
#define STAND_BY 0
#define GOING 1

long sum;
long c_time;

void setup() {
  // Set point
  setPoint_master_X = 0;
  setPoint_slave_X = 0;

  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  comu = new SerialCommunication("Posição SetPoint:");
  
// Setup Batentes 

  // master
  endstop_master_L_X = new Chave_fim_de_curso(chave_master_L_X,chave_channel_master_L_X);
  endstop_master_L_X->init();
  endstop_master_R_X = new Chave_fim_de_curso(chave_master_R_X,chave_channel_master_R_X);
  endstop_master_R_X->init();
    
  // Slave
  endstop_slave_L_X = new Chave_fim_de_curso(chave_slave_L_X, chave_channel_slave_L_X);
  endstop_slave_L_X->init();
  endstop_slave_R_X = new Chave_fim_de_curso(chave_slave_R_X, chave_channel_slave_R_X);
  endstop_slave_R_X->init();


// Setup H_bridge

  BTS_master_X = new H_bridge_controller(L_pin_master_X, R_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();
  
  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

// Setup encoder 

  encoder_master_X =new Encoder(A_pin_master_X ,B_pin_master_X ,0,Nominal_pulses,pitch_pulley_master,4);
  encoder_master_X->init();
  
  encoder_slave_X = new Encoder(B_pin_slave_X, A_pin_slave_X, 1, Nominal_pulses, pitch_pulley_slave, 4);
  encoder_slave_X->init();

//PID
  PID_master_X = new PID(kp_master_x,ki_master_x,kd_master_x,i_saturation_master_x);
  PID_slave_X = new PID(kp_slave_x, ki_slave_x, kd_slave_x, i_saturation_slave_x);
 

  // Creating Axis
  // Doesn't work with debug true
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PID_master_X, X_master_MAX_VEL, PWM_resolution_channel, X_master_tolerance, pwm_master_cte, false);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PID_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, X_slave_tolerance, pwm_slave_cte, false);

  Bomba_Y = new Bomba(bomba_pin);
  Atuador_Y = new Atuador(Extend_pin,Contract_pin);

  controller = new Controller(Axis_master_X, Axis_slave_X, Bomba_Y, Atuador_Y);

  sum = 0;
  c_time = millis();

//Seting initial conditions

  //delay(5000);
  //int looping = 1;
  //while (looping==1) {
    //controller->go_origin(true, true);
    //BTS_master_X->Set_R(125);
    //BTS_slave_X->Set_R(125);
  //}
  controller->go_origin(true, true);
  controller->go_max(true, true);
  controller->go_origin(true, true);
  //Axis_master_X->go_max();
  //Axis_slave_X->go_max();
  //Axis_master_X->go_origin();
  //Axis_slave_X->go_origin();
  //STATE = STAND_BY;

}
//int firstTime = 0;

void loop() {
  switch(STATE) {
      case STAND_BY:
        //delay(2000);                                                  //for tests
        //controller->setGoal(200, Axis_master_X->position());          //for tests
        //STATE = GOING;                                                //for tests
        read_setpoint(); // Recive Set point
        return;

      case GOING:
        //Serial.println("Entrou");   
        //if (firstTime = 0) {
        controller->move(); 
          //Axis_master_X->go_L();
          //firstTime = 1;
        //}
        //Axis_slave_X->move();
        Axis_slave_X->setGoal(Axis_master_X->position());

        //Serial.println("Posicao do master:");
        //Serial.println(Axis_master_X->position());
        //Serial.println("Posicao do slave:");
        //Serial.println(Axis_slave_X->position());
        // moving axis to setpoint
        //Serial.println(Axis_slave_X->position()); 
        //attachInterrupt (endstop_master_R_X->getPin(), ISR(), RISING);
        data();
        delay(2); //Code does not work without this delay (?)

        check_position(); 
        return;
   }   
}
/*
void ISR(){
  BTS_master_X->Set_L(0);
  delay(1000);
}
*/
char* string_to_char(std::string str) {
   char* cstr = new char[str.size() + 1];
   strcpy(cstr, str.c_str());
   return cstr;
}


// colocar essas funções dentro do axis.cpp
// como eu coloco serial comunication dentro do 

void read_setpoint(){
  if(Serial.available()){
      STATE=GOING;
      //Serial.println("GOING"); 
      
      comu->read_data(MAIN_SERIAL);
      char* recived=string_to_char(comu->get_received_data());

      int setPoint_X = atoi(recived);
      controller->setGoal(setPoint_X, Axis_master_X->position());
      //controller->setGoal(setPoint_X, setPoint_X);
  }
}

void check_position(){
    //Serial.println(Axis_master_X->position());
    //Serial.println(Axis_master_X->onGoal());
    //Serial.println(Axis_slave_X->position());
    //Serial.println(Axis_master_X->onGoal());
    /*if (Axis_master_X->onGoal()) {
      //STATE=STAND_BY;
      //controller->stop(true, true); 
      Serial.println("DEU CERTO 1"); 
      //float *position = controller->positionPoint();
      //Serial.println("STANDY-BY");
    }*/

    /*if (Axis_slave_X->onGoal()) {
      //STATE=STAND_BY;
      //controller->stop(true, true); 
      Serial.println("DEU CERTO 2"); 
      //float *position = controller->positionPoint();
      //Serial.println("STANDY-BY");
    }*/

    if (Axis_master_X->onGoal()) {
      if (controller->onGoal()) {
        Serial.println("Entrou");                        //tests
        STATE=STAND_BY;
        controller->stop(true, true);
        float *position = controller->positionPoint();
        Serial.println("STANDY-BY");
        //controller->stop(true, true);
        //Serial.printf("Axis1: %f\n", position[0]);
        //Serial.printf("Axis2: %f\n", position[1]);
      }
    }
}
  
void data(){
  static long sum = 0;
  static long sum2 = 0;
  static unsigned long count = 0;
  static unsigned long ti = 0;
  static unsigned long p_time = 0;

  //Serial.println("position1(mm),position2(mm),time(ms),system input1(pwm),system input2(pwm)");

  sum += encoder_master_X->getPosition();
  sum2 += encoder_slave_X->getPosition();

  if (count == 0)
  {
    ti = millis(); // Ti 
  }

  if (count == sample) // agruping sample serie of position 
  {
    unsigned long c_time = millis(); // Tf
    unsigned long t = c_time - ti; // delta = Tf - Ti

    Serial.print(sum*0.2 ); // media of position 
    Serial.print(", ");
    Serial.print(sum2*0.2);
    Serial.print(", ");
    Serial.print(t); 
    Serial.print(", ");
    Serial.print(Axis_master_X->getOutput());
    Serial.print(", ");
    Serial.println(Axis_slave_X->getOutput());

    p_time= c_time;

    sum = 0;
    sum2 = 0;
    count = 0;
  }
}
  
