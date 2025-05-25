/**
 * @file  Fuzzy_controller.cpp
 *
 * @brief Fuzzy logic controler class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 12/2024
 * 
 * @copyright MIT License
 */

#ifndef __FUZZY_CONTROLLER_HPP__
#define __FUZZY_CONTROLLER_HPP__

#include "Fuzzy_member_param.hpp"
#include <Arduino.h>
#include <Fuzzy.h>
#include <array>

class Fuzzy_controller {
    protected:
        double sample_time;               
        double prev_time;
        double prev_time2;
        std::array<double, 5> prev_error;
        double prev_out;
        double error_vel;

        // Member Functions Values
        Fuzzy_member_param member_function_param;

        // Main controller
        Fuzzy *fuzzy;

        // Fuzzy Inputs: Error
        FuzzySet *error_negative_high;
        FuzzySet *error_negative;
        FuzzySet *error_zero;
        FuzzySet *error_positive;
        FuzzySet *error_positive_high;

        // Fuzzy Inputs: Error velocity
        FuzzySet *velocity_negative_high;
        FuzzySet *velocity_negative;
        FuzzySet *velocity_zero;
        FuzzySet *velocity_positive;
        FuzzySet *velocity_positive_high;

        // Fuzzy Output: PWM control
        FuzzySet *PWM_NVH;               // Negative Very High
        FuzzySet *PWM_NH;                // Negative High
        FuzzySet *PWM_NM;                // Negative Medium
        FuzzySet *PWM_NS;                // Negative Small
        FuzzySet *PWM_Z;                 // Zero
        FuzzySet *PWM_PS;                // Positive Small 
        FuzzySet *PWM_PM;                // Positive Medium
        FuzzySet *PWM_PH;                // Positive High
        FuzzySet *PWM_PVH;               // Positive Very High

        // Fuzzy Inputs: Error inputs
        FuzzyInput *distance;
        FuzzyInput *velocity;

        // Fuzzy Outputs: PWM output
        FuzzyOutput *PWM_output;

        // Fuzzy Rules
        FuzzyRuleAntecedent *ifErrorNegativeHighAndVelocityNegativeHigh;
        FuzzyRuleAntecedent *ifErrorNegativeHighAndVelocityNegative;
        FuzzyRuleAntecedent *ifErrorNegativeHighAndVelocityZero;
        FuzzyRuleAntecedent *ifErrorNegativeHighAndVelocityPositive;
        FuzzyRuleAntecedent *ifErrorNegativeHighAndVelocityPositiveHigh;
        FuzzyRuleAntecedent *ifErrorNegativeAndVelocityNegativeHigh;
        FuzzyRuleAntecedent *ifErrorNegativeAndVelocityNegative;
        FuzzyRuleAntecedent *ifErrorNegativeAndVelocityZero;
        FuzzyRuleAntecedent *ifErrorNegativeAndVelocityPositive;
        FuzzyRuleAntecedent *ifErrorNegativeAndVelocityPositiveHigh;
        FuzzyRuleAntecedent *ifErrorZeroAndVelocityNegativeHigh;
        FuzzyRuleAntecedent *ifErrorZeroAndVelocityNegative;
        FuzzyRuleAntecedent *ifErrorZeroAndVelocityZero;
        FuzzyRuleAntecedent *ifErrorZeroAndVelocityPositive;
        FuzzyRuleAntecedent *ifErrorZeroAndVelocityPositiveHigh;
        FuzzyRuleAntecedent *ifErrorPositiveAndVelocityNegativeHigh;
        FuzzyRuleAntecedent *ifErrorPositiveAndVelocityNegative;
        FuzzyRuleAntecedent *ifErrorPositiveAndVelocityZero;
        FuzzyRuleAntecedent *ifErrorPositiveAndVelocityPositive;
        FuzzyRuleAntecedent *ifErrorPositiveAndVelocityPositiveHigh;
        FuzzyRuleAntecedent *ifErrorPositiveHighAndVelocityNegativeHigh;
        FuzzyRuleAntecedent *ifErrorPositiveHighAndVelocityNegative;
        FuzzyRuleAntecedent *ifErrorPositiveHighAndVelocityZero;
        FuzzyRuleAntecedent *ifErrorPositiveHighAndVelocityPositive;
        FuzzyRuleAntecedent *ifErrorPositiveHighAndVelocityPositiveHigh;

        FuzzyRuleConsequent *thenRisPWM_NVH;
        FuzzyRuleConsequent *thenRisPWM_NH;
        FuzzyRuleConsequent *thenRisPWM_NM;
        FuzzyRuleConsequent *thenRisPWM_NS;
        FuzzyRuleConsequent *thenRisPWM_Z;
        FuzzyRuleConsequent *thenRisPWM_PS;
        FuzzyRuleConsequent *thenRisPWM_PM;
        FuzzyRuleConsequent *thenRisPWM_PH;
        FuzzyRuleConsequent *thenRisPWM_PVH;

        FuzzyRule *fuzzyRule1;
        FuzzyRule *fuzzyRule2;
        FuzzyRule *fuzzyRule3;
        FuzzyRule *fuzzyRule4;
        FuzzyRule *fuzzyRule5;
        FuzzyRule *fuzzyRule6;
        FuzzyRule *fuzzyRule7;
        FuzzyRule *fuzzyRule8;
        FuzzyRule *fuzzyRule9;
        FuzzyRule *fuzzyRule10;
        FuzzyRule *fuzzyRule11;
        FuzzyRule *fuzzyRule12;
        FuzzyRule *fuzzyRule13;
        FuzzyRule *fuzzyRule14;
        FuzzyRule *fuzzyRule15;
        FuzzyRule *fuzzyRule16;
        FuzzyRule *fuzzyRule17;
        FuzzyRule *fuzzyRule18;
        FuzzyRule *fuzzyRule19;
        FuzzyRule *fuzzyRule20;
        FuzzyRule *fuzzyRule21;
        FuzzyRule *fuzzyRule22;
        FuzzyRule *fuzzyRule23;
        FuzzyRule *fuzzyRule24;
        FuzzyRule *fuzzyRule25;

    public:
        /**
         * @brief Default constructor of a Fuzzy_controler base class
         * 
         * @param sample_time time between executions
         * @param correction_coeficient constant to match the master and slave motors
         */
        Fuzzy_controller(double sample_time, Fuzzy_member_param member_function_param);

        /**
         * @brief Computes output value
         * 
         * @param pos current position 
         * @param setpoint desired position
         */
        double CalculateOutput(double pos, double setpoint);

        /** 
        * @brief Resets param
        */
       void ResetFuzzy();

        /** 
        * @brief Returns error velocity
        */
       double getErrorVelocity();

        /** 
        * @brief Returns output
        */
       double getOutput();

};

#endif  // __FUZZY_CONTROLLER_HPP__