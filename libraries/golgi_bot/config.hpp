#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

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
#define X_max_index 3

//  X Encoder
#define A_pin_master_X 13 // Green cable
#define B_pin_master_X 15 // White cable

#define A_pin_slave_X 22 // Green cable
#define B_pin_slave_X 23 // White cable

// X BTS
#define R_pin_master_X 27 // L Bts  
#define L_pin_master_X 26 // R Bts
#define R_channel_master_X 4
#define L_channel_master_X 5
#define pwm_master_cte 0.5 // fraction of pwm velocity

#define R_pin_slave_X 16 // pin
#define L_pin_slave_X 12 // pin
#define R_channel_slave_X 6
#define L_channel_slave_X 7
#define pwm_slave_cte 0.5

// X CHAVE
#define chave_master_R_X 39
#define chave_master_L_X 36
#define chave_slave_R_X 5
#define chave_slave_L_X 4 

#define chave_channel_master_R_X 0
#define chave_channel_master_L_X 1
#define chave_channel_slave_R_X 2
#define chave_channel_slave_L_X 3




// X PID Master
#define kp_master_x  4.5
#define ki_master_x  0.05
#define kd_master_x  60
#define i_saturation_master_x 1000


// X PID Slave 
// aumentar kp para diminuir o tempo de resposta
#define kp_slave_x  7
#define ki_slave_x  0.03
#define kd_slave_x  60
#define i_saturation_slave_x 1000 // Diminuir saturação para ser mais sensivel ao erro


/// Z axis ///

#define Z_MAX_VEL 0.5
#define Z_MAX_VEL 0.5
#define Z_size 963
#define Z_tolerance 4
#define Z_max_index 2

// Z Encoder 
#define A_pin_Z 19 // Green cable
#define B_pin_Z 21 // white cable

// Z BTS 
#define R_pin_Z 18 // R bts
#define L_pin_Z 17 // L bts
#define R_channel_Z  9
#define L_channel_Z 10
#define pwm_cte_Z 0.5 // fraction of pwm velocity

// Z Chave
#define chave_L_Z 34 
#define chave_R_Z 35 

// Z PID 
#define kp_z 3.2
#define ki_z 0.01
#define kd_z 8
#define i_saturation_z 1000


/// Y axis ///


// DELAY TIME
#define DELAY_EXTEND 2000
#define DELAY_CONTRACT 1000
// Y Bomba 
#define bomba_pin 32//IN2

// Y Atuador
#define Extend_pin 25 //IN4
#define Contract_pin 33 //IN3

#endif  // __CONFIG_HPP__

