unsigned long previousMillis1 = 0;
const long interval1 = 35;
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

//Fuzzy_controller *//Fuzzy_master_X;
//Fuzzy_controller *//Fuzzy_slave_X;

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

//Fuzzy_controller *//Fuzzy_Z;

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

  BTS_master_X = new H_bridge_controller(R_pin_master_X, L_pin_master_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_master_X, L_channel_master_X);
  BTS_master_X->init();
  
  BTS_slave_X = new H_bridge_controller(R_pin_slave_X, L_pin_slave_X, PWM_frequency_channel, PWM_resolution_channel, R_channel_slave_X, L_channel_slave_X);
  BTS_slave_X->init();

  BTS_Z= new H_bridge_controller(R_pin_Z, L_pin_Z, PWM_frequency_channel, PWM_resolution_channel, R_channel_Z, L_channel_Z);      //esses canais talvez deem problema
  BTS_Z->init();

  // Encoder

  encoder_master_X =new Encoder(A_pin_master_X ,B_pin_master_X , 0, Nominal_pulses, pitch_pulley_master, 4);
  encoder_master_X->init();
  
  encoder_slave_X = new Encoder(A_pin_slave_X, B_pin_slave_X, 1, Nominal_pulses, pitch_pulley_slave, 4);
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

  PIDinc_master_X = new PID_incremental(N_inc_master_x, kp_inc_master_x, ti_inc_master_x, td_inc_master_x, ts_inc_master_x);
  PIDinc_slave_X = new PID_incremental(N_inc_slave_x, kp_inc_slave_x, ti_inc_slave_x, td_inc_slave_x, ts_inc_slave_x);         //0.001  

  PIDinc_Z = new PID_incremental(N_z, kp_inc_z, ti_inc_z, td_inc_z, ts_inc_z);

  // //Fuzzy control
  //Fuzzy_master_X = new //Fuzzy_controller(0.01, error_NH_master, error_N_master, error_Z_master, error_P_master, error_PH_master);
  //Fuzzy_slave_X = new //Fuzzy_controller(0.01, error_NH_slave, error_N_slave, error_Z_slave, error_P_slave, error_PH_slave);
  //Fuzzy_Z = new //Fuzzy_controller(0.01, error_NH_Z, error_N_Z, error_Z_Z, error_P_Z, error_PH_Z);

  // Sliding mode control

  SMC_master_X = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_master, Load_inercia_param_master, Velocity_param_master, 1, gama_param, alpha_param, radius_param, 15.9, sampling_time_param);
  SMC_slave_X = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_slave, Load_inercia_param_slave, Velocity_param_slave, 1, gama_param, 0.1, radius_param, 15.7, sampling_time_param);
  SMC_Z = new Sliding_controller(Elast_coef_param, Torque_coef_param, Load_mass_param_master, Load_inercia_param_master, Velocity_param_master, 1, gama_param, alpha_param, radius_param, 8.9, sampling_time_param);

  //Creating Axis
  Axis_master_X = new Axis(encoder_master_X, BTS_master_X, endstop_master_R_X, endstop_master_L_X, PIDinc_master_X, X_master_MAX_VEL, PWM_resolution_channel, 1.5, pwm_master_cte, false);
  Axis_slave_X = new Axis(encoder_slave_X, BTS_slave_X, endstop_slave_R_X, endstop_slave_L_X, PIDinc_slave_X, X_slave_MAX_VEL, PWM_resolution_channel, 1.5, pwm_slave_cte, false);
  
  Axis_z= new Axis(encoder_Z, BTS_Z, endstop_R_Z, endstop_L_Z, PIDinc_Z, Z_MAX_VEL, PWM_resolution_channel, Z_tolerance, pwm_cte_Z, false);

  //Creating Controller
  Golgi_bot = new Controller(Axis_master_X, Axis_slave_X, Axis_z, Bomba_Y, Atuador_Y);

  //Setting up the right inicital state
  Golgi_bot->reset_Y(DELAY_CONTRACT);

  delay(1000);
  bool endstop_R_master_X_initial = endstop_master_R_X->getBatente();
  bool endstop_L_master_X_initial = endstop_master_L_X->getBatente();
  bool endstop_slave_R_X_initial = endstop_slave_R_X->getBatente();
  bool endstop_slave_L_X_initial = endstop_slave_L_X->getBatente();
  bool endstop_R_Z_initial = endstop_R_Z->getBatente();
  bool endstop_L_Z_initial = endstop_L_Z->getBatente();

  bool endstop_R_master_X_changed;
  bool endstop_L_master_X_changed;
  bool endstop_slave_R_X_changed;
  bool endstop_slave_L_X_changed;
  bool endstop_R_Z_changed;
  bool endstop_L_Z_changed;

  if (endstop_slave_L_X->getBatente())
  {
    Serial.println("Batente L slave X pressionado inicialmente");
  } 
  if (endstop_slave_R_X->getBatente())
  {
    Serial.println("Batente R slave X pressionado inicialmente");
  } 
  if (endstop_master_L_X->getBatente())
  {
    Serial.println("Batente L master X pressionado inicialmente");
  } 
  if (endstop_master_R_X->getBatente())
  {
    Serial.println("Batente R master X pressionado inicialmente");
  } 
  if (endstop_R_Z->getBatente())
  {
    Serial.println("Batente R Z pressionado inicialmente");
  } 
  if (endstop_L_Z->getBatente())
  {
    Serial.println("Batente L Z pressionado inicialmente");
  }

  int sempre = 1;
  while (sempre = 1) {
    endstop_R_master_X_changed = endstop_R_master_X_initial != endstop_master_R_X->getBatente();
    endstop_L_master_X_changed = endstop_L_master_X_initial != endstop_master_L_X->getBatente();
    endstop_slave_R_X_changed = endstop_slave_R_X_initial != endstop_slave_R_X->getBatente();
    endstop_slave_L_X_changed = endstop_slave_L_X_initial != endstop_slave_L_X->getBatente();
    endstop_R_Z_changed = endstop_R_Z_initial != endstop_R_Z->getBatente();
    endstop_L_Z_changed = endstop_L_Z_initial != endstop_L_Z->getBatente();

    if(endstop_R_master_X_changed || endstop_slave_R_X_changed) {
        BTS_slave_X->Set_R(160); 
        BTS_master_X->Set_R(160);
    }
    else if(endstop_L_master_X_changed || endstop_slave_L_X_changed) {
        BTS_slave_X->Set_L(160); 
        BTS_master_X->Set_L(160);
    }
    else if(endstop_L_Z_changed){
        BTS_Z->Set_L(160); 
    }
    else if(endstop_R_Z_changed) {
        BTS_Z->Set_R(160); 
    }
  }
}

void loop() {
  delay(1);
  return;
}