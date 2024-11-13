int sample = 4;
int setPoint_X = 0;
unsigned long previousMillis = 0;
const long interval = 25;

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

Bomba *Bomba_Y;
Atuador *Atuador_Y;

Controller *controller;

//Serial comunication
#include "serial_communication.hpp"
SerialCommunication *comu;
#include <string>
#include <cstring>
double setPoint_master_X = 0;
double setPoint_slave_X = 0;

// Estados
char STATE = 0 ; // 
// #define STAND_BY 0
// #define GOING 1

long sum;
long c_time;

void setup() {
  //Serial Comunication
  Serial.begin (SERIAL_VEL);
  delay(100);
  Serial.println("AAAAAAAAAAAAAAAAAA");
  comu = new SerialCommunication("Posição SetPoint:");
  
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


  controller = new Controller(Axis_master_X, Axis_slave_X, Axis_z, Bomba_Y, Atuador_Y);

  controller->reset_Y(DELAY_CONTRACT);
  

  sum = 0;
  c_time = millis();

//Seting initial conditions

  //delay(5000);
  double output1;
  while (true) {
    //Serial.println("AA");
    BTS_master_X->Set_L(125);
    Axis_slave_X->setPoint(Axis_master_X->position());
    output1=(PID_slave_X->computePID(encoder_slave_X->getPosition(),Axis_master_X->position(),2*5));
    Serial.println(output1);
  if (output1 <= 0) {
    if (output1 < -127) {
        output1 = -125;
    }
    output1 = -output1;
    Serial.println("B");
    Serial.println(output1);
    int aux = output1;
    BTS_slave_X->Set_L(aux);
  } else {
    
    if (output1 > 127) {
      output1 = 127;
    }

    Serial.println("C");
    Serial.println(output1);
    BTS_slave_X->Set_R(125);
  }
    
    Axis_slave_X->move();
    delay(2);
  // int aux = 125;
  //   BTS_slave_X->Set_L(aux);
    //Serial.println(encoder_slave_X->getPosition());
  }
  //Serial.println(";-;");
  controller->go_origin(true, true);
  controller->go_max(true, true, true);
  controller->go_origin(true, true);
  //Axis_master_X->go_max();
  //Axis_slave_X->go_max();
  //Axis_master_X->go_origin();
  //Axis_slave_X->go_origin();
  //STATE = STAND_BY;
  delay(10000);

}

void loop() {
  switch(STATE) {
      delay(10000);
      case STAND_BY:

        read_setpoint(); // Recive Set point
        return;

      case GOING:
        unsigned long currentMillis = millis();

        controller->move(); 

        //Axis_slave_X->move();
        Axis_slave_X->setGoal(Axis_master_X->position());

        //Serial.println("Posicao do master:");
        //Serial.println(Axis_master_X->position());
        //Serial.println("Posicao do slave:");
        //Serial.println(Axis_slave_X->position());
        // moving axis to setpoint
        //Serial.println(Axis_slave_X->position()); 
        //attachInterrupt (endstop_master_R_X->getPin(), ISR(), RISING);
        //data();
        

        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;

          // Serial.print(setPoint_X);
          // Serial.print(" , ");
          Serial.print(Axis_master_X->position());
          Serial.print(" , ");
          Serial.println(Axis_slave_X->position()); // Plotts the PID response
        }

        



        delay(1); //Code does not work without this delay (derivative part of PID needs a bit of time to work)

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

void read_setpoint(){
  if(Serial.available()){
      STATE=GOING;
      //Serial.println("GOING"); 
      
      comu->read_data(MAIN_SERIAL);
      char* recived=string_to_char(comu->get_received_data());

      setPoint_X = atoi(recived);
      controller->setGoal(setPoint_X, Axis_master_X->position(), 0);
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
      if (Axis_slave_X->onGoal()) {
        STATE=STAND_BY;
        controller->stop(true, true, true);
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
  
