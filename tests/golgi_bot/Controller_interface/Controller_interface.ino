unsigned long previousMillis1 = 0;
const long interval1 = 25;
bool insideError = false;
unsigned long primeira_chegada = 0;
double initial_time = 0;
int which_med = 0;
#include <chrono>

// Header file which contain Pin, constanst, states and etc...
#include "config.hpp"

//Controller lib

//#include "Controller.hpp"
#include "Controller.hpp"

#include "new_communication.hpp"
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

PID_incremental *PIDinc_master_X;
PID_incremental *PIDinc_slave_X;

Fuzzy_controller *Fuzzy_master_X;
Fuzzy_controller *Fuzzy_slave_X;

Sliding_controller *SMC_master_X;
Sliding_controller *SMC_slave_X;

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

PID_incremental *PIDinc_Z;

Fuzzy_controller *Fuzzy_Z;

NewCommunication *Communication;

const std::array<double, 4> error_NH_master = {-600, -600, -300, -100};
const std::array<double, 4> error_N_master  = {-200, -33, -33, 0};
const std::array<double, 4> error_Z_master  = {-50, 0, 0, 50};
const std::array<double, 4> error_P_master  = {0, 33, 33, 200};
const std::array<double, 4> error_PH_master = {100, 300, 600, 600};

const std::array<double, 4> error_NH_slave = {-800, -800, -300, -80};
const std::array<double, 4> error_N_slave  = {-200, -44, -44, 0};
const std::array<double, 4> error_Z_slave  = {-18, 0, 0, 18};
const std::array<double, 4> error_P_slave  = {0, 44, 44, 200};
const std::array<double, 4> error_PH_slave = {80, 300, 800, 800};

const std::array<double, 4> error_NH_Z = {-600, -600, -300, -150};
const std::array<double, 4> error_N_Z  = {-200, -45, -45, 0};
const std::array<double, 4> error_Z_Z  = {-50, 0, 0, 50};
const std::array<double, 4> error_P_Z  = {0, 45, 45, 200};
const std::array<double, 4> error_PH_Z = {150, 300, 600, 600};

Sliding_controller *SMC_Z;

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
int pos_x[X_max_index]={100,200,400,600};
int pos_z[Z_max_index]={55,180,355};
double X_pos;
double Z_pos;
int counter = 0;
int counter_X = 0;
int counter_Z = 0;

//STATE
char STATE = 0 ; 

#define led 2

void setup() {
   
  //Serial Comunication
  Serial.begin(SERIAL_VEL);
  delay(100);

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

  // PID incremental       teste

  PIDinc_master_X = new PID_incremental(80, 3.2, 10, 0.001, 0.01);
  PIDinc_slave_X = new PID_incremental(80, 18.2, 2, 0.001, 0.01);         //0.001  

  PIDinc_Z = new PID_incremental(80, 1.3, 100, 0.001, 0.01);

  // Fuzzy control
  Fuzzy_master_X = new Fuzzy_controller(0.01, error_NH_master, error_N_master, error_Z_master, error_P_master, error_PH_master);
  Fuzzy_slave_X = new Fuzzy_controller(0.01, error_NH_slave, error_N_slave, error_Z_slave, error_P_slave, error_PH_slave);
  Fuzzy_Z = new Fuzzy_controller(0.01, error_NH_Z, error_N_Z, error_Z_Z, error_P_Z, error_PH_Z);

  // Sliding mode control

  SMC_master_X = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_master, Load_inercia_param_master, Velocity_param_master, 1, gama_param, alpha_param, radius_param, 15.9, sampling_time_param);
  SMC_slave_X = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_slave, Load_inercia_param_slave, Velocity_param_slave, 1, gama_param, 0.1, radius_param, 15.7, sampling_time_param);
  SMC_Z = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_master, Load_inercia_param_master, Velocity_param_master, 1, gama_param, alpha_param, radius_param, 8.9, sampling_time_param);

  //Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, SMC_master_X, X_master_MAX_VEL, PWM_resolution_channel, 1.5, pwm_master_cte, false);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, SMC_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, 1.5, pwm_slave_cte, false);
  
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, SMC_Z, Z_MAX_VEL, PWM_resolution_channel, Z_tolerance, pwm_cte_Z, false);

  //Creating Controller
  Golgi_bot = new Controller(Axis_master_X, Axis_slave_X, Axis_z, Bomba_Y, Atuador_Y);

  //Setting up the right inicital state
  Golgi_bot->reset_Y(DELAY_CONTRACT);


  // int sempre = 1;
  // while (sempre = 1) {
  //   Golgi_bot->go_origin(true, true);
  //   delay(3000);
  //   Golgi_bot->go_max(true, true, true);
  //   delay(3000);
  //   //Serial.println(encoder_master_X->getPosition());
  //   // BTS_master_X->Set_R(127); 
  //   // Serial.println("AAAAAAAAAAAAAA");
  //   // Axis_z->move()
  //   // Golgi_bot->go_origin(true, true);
  //   // delay(2000);
  //   // Golgi_bot->go_max(true, true, true);      // test if it needs to go max, or if going once to orign works
  //   // delay(2000);
  //   // Golgi_bot->go_origin(true, true);
  //   // delay(2000);
  //   // BTS_slave_X->Set_R(127);
  //   // Axis_slave_X->setPoint(Axis_master_X->position());
  //   //Serial.println(Axis_slave_X->getOutput());
  //   // Serial.println(Axis_master_X->position()); //
  //   // Axis_slave_X->move(); //
  //   //int output1 = 125;
  //   //Serial.println(encoder_master_X->getPosition());
  // }


  Golgi_bot->go_origin(true, true);
  Golgi_bot->go_max(true, true, true);      // test if it needs to go max, or if going once to orign works
  Golgi_bot->go_origin(true, true);

  Communication = new NewCommunication(Golgi_bot);

  Serial.println("STAND-BY");

}

void loop() {
  unsigned long currentMillis1 = millis();
  switch(STATE) {
      case STAND_BY :
        delay(100);
       // Recive Set point
        Serial.println("STAND-BY");
        read_setpoint();
        //initial_time = millis();
        //Serial.println(initial_time);
        return;
      case GOING :
        //Moves Controller
        Golgi_bot->move();
        // Serial.println("BRUHHH");
        // Axis_master_X->move();        
        // Axis_slave_X->setGoal(Axis_master_X->position());
        // delay(2);

        // Axis_slave_X->move();
        // Axis_z->move();
        if (Axis_master_X->getOutput() <= 40 && !insideError) {     
          primeira_chegada = millis();
          // Serial.println("entrou");
          // Serial.println(primeira_chegada);
          insideError = true;
        }

          if (currentMillis1 - previousMillis1 >= interval1) {
          previousMillis1 = currentMillis1;

        //   // Serial.println(Axis_z->getOutput());
        //   // Serial.print(setPoint_X);
        //   // Serial.print(" , ");
        //   //Serial.println(Axis_master_X->getOutput() );
          // Serial.print(Axis_master_X->position());
          // Serial.print(", ");
          // Serial.println(Axis_slave_X->position()); // Plotts the PID response
        // //   // Serial.print(SMC_master_X->getVelocity(), 3);
        //   Serial.print(", ");
        //   Serial.println(millis());
        //   // Serial.println(SMC_slave_X->getVelocity(), 3); // Plotts the output response
        }

        check_position();
        return;
      case GETING_MEDICINE :
        Golgi_bot->get_medicine(DELAY_EXTEND,DELAY_CONTRACT);
        // Serial.print(Axis_master_X->position());
        // Serial.print(" , ");
        // Serial.print(Axis_slave_X->position()); 
        // Serial.print(", ");
        // Serial.println(millis());
        STATE=DROPING_MEDICINE;
        // Serial.println("DROPING_MEDICINE");
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
  String received;
  if(Serial.available()) {
      STATE=GOING;
      Serial.println("GOING");
      //delay(1000);
      //comu->read_data(MAIN_SERIAL);
      //char* received = string_to_char(comu->get_received_data());
      //int id_remedio = atoi(received);
      //int setPoint_z = 0;
      received = Serial.readString();
      received.trim();
      Communication->read_setpoint(received);
  }



  // Serial.println("GOING");  
  // Versão com posições sequenciais

  // if (counter_Z == 3 && counter_X == 4) {
  //   counter_Z = 0;
  //   counter_X = 0;
  // }     

  // if (counter_X == 4) {
  //   counter_X = 0;
  //   counter_Z++;
  // }

  // Z_pos=pos_z[counter_Z];
  // X_pos=pos_x[counter_X++];
  // Serial.print("counter_Z: ");
  // Serial.println(counter_Z);
  // Serial.print("counter_X: ");
  // Serial.println(counter_X);
  //Serial.print("X goal:");
  //Serial.println(X_pos);      
  //Serial.print("Z goal:");
  //Serial.println(Z_pos);

//  Versão com posições aleatórias
  /*
  srand((unsigned) time(NULL));
  int random = rand() % 13;

  counter = 0;
  for(int j=0; j< Z_max_index; j++){
    for(int i =0; i < X_max_index; i++){
      counter=counter+1;
      if (counter==random){
        Z_pos=pos_z[j];
        X_pos=pos_x[i];
      }
    }
  }
  Serial.println(random);  

  Golgi_bot->setGoal(X_pos, X_pos, Z_pos);
  */

}

void check_position(){
  if(Axis_master_X->onGoal() && Axis_slave_X->onGoal()){                      // Golgi_bot->onGoal()
    STATE=GETING_MEDICINE;
    // // Serial.println("GETING_MEDICINE");
    // Serial.print(Axis_master_X->position());
    // Serial.print(" , ");
    // Serial.println(Axis_slave_X->position()); 
    // double error = 0;
    // error = X_pos - Golgi_bot->positionPoint()[0];
    // Serial.print(error);
    // Serial.print(", ");
    // error = X_pos - Golgi_bot->positionPoint()[1];
    // Serial.print(error);
    // // Serial.print(",");
    // // // error = Z_pos - Golgi_bot->positionPoint()[2];
    // // // Serial.println(error);
    // unsigned long tempo_atual = millis();
    // // // Serial.print("atual: ");
    // // // Serial.println(tempo_atual);
    // unsigned long duracao = (tempo_atual - primeira_chegada);
    // Serial.print(", ");
    // Serial.println(duracao);
    // insideError = false;
    Golgi_bot->stop(true,true,true);
  }
}