/**
 * @file  Fuzzy_member_param.cpp
 *
 * @brief Fuzzy struct for member functions
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 12/2024
 * 
 * @copyright MIT License
 */

 #include "Fuzzy_member_param.hpp"

 /**********************
  * Struct Definitions
  **********************/

Fuzzy_member_param::Fuzzy_member_param()
    : error_NH({0, 0, 0, 0}), error_N({0, 0, 0, 0}), error_Z({0, 0, 0, 0}), error_P({0, 0, 0, 0}), error_PH({0, 0, 0, 0}),
    error_vel_NH({0, 0, 0, 0}), error_vel_N({0, 0, 0, 0}), error_vel_Z({0, 0, 0, 0}), error_vel_P({0, 0, 0, 0}), error_vel_PH({0, 0, 0, 0}),
    PWM_NVH({0, 0, 0, 0}), PWM_NH({0, 0, 0, 0}), PWM_NM({0, 0, 0, 0}), PWM_NS({0, 0, 0, 0}), PWM_Z({0, 0, 0, 0}),
    PWM_PS({0, 0, 0, 0}), PWM_PM({0, 0, 0, 0}), PWM_PH({0, 0, 0, 0}), PWM_PVH({0, 0, 0, 0}) {}

    
Fuzzy_member_param::Fuzzy_member_param(
    std::array<double, 4> e_NH, std::array<double, 4> e_N, std::array<double, 4> e_Z, std::array<double, 4> e_P, 
    std::array<double, 4> e_PH, std::array<double, 4> ev_NH, std::array<double, 4> ev_N, std::array<double, 4> ev_Z, std::array<double, 4> ev_P, std::array<double, 4> ev_PH, 
    std::array<double, 4> pwm_NVH, std::array<double, 4> pwm_NH, std::array<double, 4> pwm_NM, std::array<double, 4> pwm_NS, std::array<double, 4> pwm_Z, std::array<double, 4> pwm_PS, std::array<double, 4> pwm_PM, std::array<double, 4> pwm_PH, std::array<double, 4> pwm_PVH
    )
    : error_NH(e_NH), error_N(e_N), error_Z(e_Z), error_P(e_P), error_PH(e_PH),
    error_vel_NH(ev_NH), error_vel_N(ev_N), error_vel_Z(ev_Z), error_vel_P(ev_P), error_vel_PH(ev_PH),
    PWM_NVH(pwm_NVH), PWM_NH(pwm_NH), PWM_NM(pwm_NM), PWM_NS(pwm_NS), PWM_Z(pwm_Z), PWM_PS(pwm_PS), PWM_PM(pwm_PM), PWM_PH(pwm_PH), PWM_PVH(pwm_PVH) {}
