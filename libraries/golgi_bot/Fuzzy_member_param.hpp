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

#ifndef __FUZZY_MEMBER_PARAM_HPP__
#define __FUZZY_MEMBER_PARAM_HPP__

#include <array>

struct Fuzzy_member_param {

    // error
    std::array<double, 4> error_NH, error_N, error_Z, error_P, error_PH;

    // error velocity
    std::array<double, 4> error_vel_NH, error_vel_N, error_vel_Z, error_vel_P, error_vel_PH;

    // pwm
    std::array<double, 4> PWM_NVH, PWM_NH, PWM_NM, PWM_NS, PWM_Z, PWM_PS, PWM_PM, PWM_PH, PWM_PVH;

    Fuzzy_member_param();

    Fuzzy_member_param(
        std::array<double, 4> e_NH, std::array<double, 4> e_N, std::array<double, 4> e_Z, std::array<double, 4> e_P, 
        std::array<double, 4> e_PH, std::array<double, 4> ev_NH, std::array<double, 4> ev_N, std::array<double, 4> ev_Z, std::array<double, 4> ev_P, std::array<double, 4> ev_PH, 
        std::array<double, 4> pwm_NVH, std::array<double, 4> pwm_NH, std::array<double, 4> pwm_NM, std::array<double, 4> pwm_NS, std::array<double, 4> pwm_Z, std::array<double, 4> pwm_PS, std::array<double, 4> pwm_PM, std::array<double, 4> pwm_PH, std::array<double, 4> pwm_PVH);

};

#endif // __FUZZY_MEMBER_PARAM_HPP__