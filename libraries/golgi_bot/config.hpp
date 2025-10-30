#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <array>

// Comum used
#define Nominal_pulses 360
#define pitch_pulley_master 44
#define pitch_pulley_slave 44
#define Mode 1
#define PWM_frequency_channel 40000
#define PWM_resolution_channel 8

// State
#define STAND_BY 0
#define GOING 1
#define GETING_MEDICINE 2
#define DROPING_MEDICINE 3

// Serial Comunication
/**
 * @file  config.hpp
 *
 * @brief firmware configuration
 * 
 * @copyright MIT License
 */

// SERIAL COMMUNICATION
#define MAIN_SERIAL_BAUDRATE 115200
#define RASPBERRY_SERIAL_BAUDRATE 115200
#define ACTIVE_RASPBERRY_COMMUNICATION


// serial data config

#define SERIAL_VEL 9600
#define max_string_length 100
#define end_char '\n'

//i2c configuration
    // set the max number of bytes the slave will send.
    // if the slave send more bytes, they will still be read
    // but the WireSlaveRequest will perform more reads
    // until a whole packet is read
#define max_i2c_slave_response 32

/// X axis ///

#define X_master_MAX_VEL 0.5
#define X_slave_MAX_VEL 0.5
#define X_master_tolerance 2
#define X_slave_tolerance 2
#define X_max_index 4

//  X Encoder
#define A_pin_master_X 13 // Green cable
#define B_pin_master_X 15 // White cable

#define A_pin_slave_X 22 // Green cable
#define B_pin_slave_X 23 // White cable

// X BTS
#define R_pin_master_X 27 // L Bts  
#define L_pin_master_X 26 // R Bts
#define R_channel_master_X 11
#define L_channel_master_X 10
#define pwm_master_cte 0.5 // fraction of pwm velocity

#define R_pin_slave_X 16 // pin
#define L_pin_slave_X 12 // pin
#define R_channel_slave_X 15                              
#define L_channel_slave_X 14
#define pwm_slave_cte 0.5          

// X CHAVE
#define chave_master_R_X 39
#define chave_master_L_X 36
#define chave_slave_R_X 5
#define chave_slave_L_X 4 

#define chave_channel_master_R_X 2
#define chave_channel_master_L_X 1          
#define chave_channel_slave_R_X 0
#define chave_channel_slave_L_X 3


// X PID Master
#define kp_master_x  14.4         
#define ki_master_x  0.05           
#define kd_master_x  100            
#define i_saturation_master_x 1000


// X PID Slave 
// aumentar kp para diminuir o tempo de resposta
#define kp_slave_x 16.8            
#define ki_slave_x 0.05           
#define kd_slave_x 100
#define i_saturation_slave_x 1000 // Diminuir saturação para ser mais sensivel ao erro

// X PID_incremental Master
#define N_inc_master_x  80
#define kp_inc_master_x 1.8
#define ti_inc_master_x 1
#define td_inc_master_x 0.001
#define ts_inc_master_x 0.01

// X PID_incremental Slave
#define N_inc_slave_x  80
#define kp_inc_slave_x 16.2
#define ti_inc_slave_x 1
#define td_inc_slave_x 0.001           
#define ts_inc_slave_x 0.01

/// Z axis ///

#define Z_MAX_VEL 0.5
#define Z_MAX_VEL 0.5
#define Z_size 963
#define Z_tolerance 2
#define Z_max_index 3

// Z Encoder 
#define A_pin_Z 21 // Green cable
#define B_pin_Z 19 // white cable

// Z BTS 
#define R_pin_Z 18 // R bts
#define L_pin_Z 17 // L bts
#define R_channel_Z 7
#define L_channel_Z 6
#define pwm_cte_Z 0.5 // fraction of pwm velocity

// Z Chave
#define chave_L_Z 35
#define chave_R_Z 34
#define chave_channel_R_Z 5
#define chave_channel_L_Z 4

// Z PID 
#define kp_z 2.2
#define ki_z 0.005
#define kd_z 60
#define i_saturation_z 1000

// Z PID_incremental
#define N_z  80
#define kp_inc_z 2.3    
#define ti_inc_z 10   
#define td_inc_z 0.001           
#define ts_inc_z 0.01


/// Y axis ///


// DELAY TIME
#define DELAY_EXTEND 1000
#define DELAY_CONTRACT 1000
// Y Bomba 
#define bomba_pin 32//IN2

// Y Atuador
#define Extend_pin 25 //IN4
#define Contract_pin 33 //IN3

// Sliding mode controller parameters
#define Elast_coef_param 2.5   
#define Torque_coef_param 0.27301136
#define Load_mass_param_master 3.8
#define Load_mass_param_slave 5.0
#define Load_inercia_param_master 0.0004598
#define Load_inercia_param_slave 0.000605
#define Velocity_param_master 0.4
#define Velocity_param_slave 0.3
#define Position_param 1
#define Resistence_master 15.9
#define Resistence_slave 15.7
#define Resistence_z 10.9
#define gama_param 0.00012098536077
#define alpha_param 0.1
#define radius_param 0.011
#define Motor_resistance 8.9
#define sampling_time_param 0.01

#endif  // __CONFIG_HPP__

