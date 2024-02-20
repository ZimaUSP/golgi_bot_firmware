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

// driver controller configuration
#define FREQUENCY 10e6
#define BIT_RESOLUTION 12

#define MIN_VEL -1
#define MAX_VEL 1
#define MIN_VEL_BITS 0
#define MAX_VEL_BITS pow(2,BIT_RESOLUTION)-1

#define MIN_ROT -1
#define MAX_ROT 1
#define MIN_ROT_BITS 0
#define MAX_ROT_BITS pow(2,BIT_RESOLUTION)-1

#define STOP_VALUE 0

//spi config
#define SPI_MODE SPI_MODE0 //SPI_MODE0 or SPI_MODE1 or SPI_MODE2 or SPI_MODE3
#define SPI_MASTER_FREQ SPI_MASTER_FREQ_8M
#define DMA_CHANNEL 1
#define QUEUE_SIZE 1
#define SPI_TYPE HSPI //HSPI or VSPI 
#define TARGET_SIZE 16
#define HEADER_SIZE 2
#define BUFFER_SIZE TARGET_SIZE + 4
#define FOOTER_SIZE HEADER_SIZE

// rosserial configuration
#define VEL_TOPIC "/cmd_vel"

//i2c configuration
    // set the max number of bytes the slave will send.
    // if the slave send more bytes, they will still be read
    // but the WireSlaveRequest will perform more reads
    // until a whole packet is read
#define max_i2c_slave_response 32

/// X axis ///

#define X_MAX_VEL 0.5
#define X_size 414
#define X_tolerance 20
#define X_max_index 3

//  X Encoder
#define A_pin_X_master 13 // Green cable
#define B_pin_X_master 15 // White cable
#define A_pin_X_slave 22 // Green cable
#define B_pin_X_slave 23 // White cable


// X BTS
#define R_pin_X 27 // L Bts
#define L_pin_X 26 // R Bts
#define R_channel_X 0
#define L_channel_X 1

// X CHAVE
#define chave_L_X_master 36 
#define chave_R_X_master 39
#define chave_L_X_slave 4
#define chave_R_X_slave 5

// X PID
#define kp_x  3.2
#define ki_x  0.01
#define kd_x  30


/// Z axis ///


#define Z_MAX_VEL 0.5
#define Z_size 270
#define Z_tolerance 8
#define Z_max_index 2

// Z Encoder 
#define A_pin_Z 19 // Green cable
#define B_pin_Z 21 // white cable

// Z BTS 
#define R_pin_Z 18 // R bts
#define L_pin_Z 17 // L bts
#define R_channel_Z 2
#define L_channel_Z 3

// Z Chave
#define chave_L_Z 34 
#define chave_R_Z 35 

// Z PID 
#define kp_z 3.2
#define ki_z 0.01
#define kd_z 8 


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

