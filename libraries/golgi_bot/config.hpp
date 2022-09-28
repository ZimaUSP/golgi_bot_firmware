#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

// Comum used
#define Nominal_pulses 600
#define pitch_pulley 40
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
#define X_size 414
#define X_master_tolerance 2
#define X_slave_tolerance 2
#define X_max_index 3
#define X_PWM_cte 0.5 // fraction of pwm velocity

//  X Encoder
#define A_pin_master_X 22 // Green cable
#define B_pin_master_X 23  // White cable

#define A_pin_slave_X 00// pin // cor do cabo
#define B_pin_slave_X 00// pin // cor do cabo

// X BTS
#define R_pin_master_X 27 // L Bts
#define L_pin_master_X 26 // R Bts
#define R_channel_master_X 0
#define L_channel_master_X 1

#define R_pin_slave_X 00// pin
#define L_pin_slave_X 00// pin
#define R_channel_slave_X 0
#define L_channel_slave_X 1

// X CHAVE
#define chave_L_X 36 
#define chave_R_X 39

// X PID Master
#define kp_master_x  3.2
#define ki_master_x  0.01
#define kd_master_x  30
#define i_saturation_master_x 1000

// X PID Slave 
// aumentar kp para diminuir o tempo de resposta
#define kp_slave_x  3.2
#define ki_slave_x  0.01
#define kd_slave_x  30
#define i_saturation_slave_x 1000 // Diminuir saturação para ser mais sensivel ao erro


/// Z axis ///

#define Z_MAX_VEL 0.5
#define Z_MAX_VEL 0.5
#define Z_size 963
#define Z_tolerance 4
#define Z_max_index 2
#define Z_PWM_cte 0.5 // fraction of pwm velocity

// Z Encoder 
#define A_pin_Z 19 // Green cable
#define B_pin_Z 21 // white cable

// Z BTS 
#define R_pin_Z 17 // R bts
#define L_pin_Z 18 // L bts
#define R_channel_Z  2 
#define L_channel_Z 3 

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

