/**
 * @file  Controle_Fuzzy.cpp
 *
 * @brief Fuzzy logic controler class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 12/2024
 * 
 * @copyright MIT License
 */


#include "Fuzzy_controller.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Fuzzy_controller::Fuzzy_controller(double sample_time) {
    this->sample_time = sample_time;
    this->prev_time = 0;
    this->prev_error = 0;

    this->fuzzy = new Fuzzy();

    // Fuzzy Inputs
    this->error_negative_high = new FuzzySet(-600, -600, -300, -250);
    this->error_negative = new FuzzySet(-300, -250, -250, 0);
    this->error_zero = new FuzzySet(-300, 0, 0, 300);
    this->error_positive = new FuzzySet(0, 250, 250, 300);
    this->error_positive_high = new FuzzySet(250, 300, 600, 600);

    this->velocity_negative_high = new FuzzySet(-800, -800, -400, -135);
    this->velocity_negative = new FuzzySet(-135, -85, -85, 0);
    this->velocity_zero = new FuzzySet(-85, 0, 0, 85);
    this->velocity_positive = new FuzzySet(0, 85, 85, 135);
    this->velocity_positive_high = new FuzzySet(135, 400, 800, 800);

    // Fuzzy Outputs
    this->PWM_NVH = new FuzzySet(-200, -200, -200, -184);
    this->PWM_NH = new FuzzySet(-200, -184, -184, -146);
    this->PWM_NM = new FuzzySet(-184, -146, -146, -10);
    this->PWM_NS = new FuzzySet(-146, -10, -10, 0);
    this->PWM_Z = new FuzzySet(-120, 0, 0, 120);
    this->PWM_PS = new FuzzySet(0, 10, 10, 146);
    this->PWM_PM = new FuzzySet(10, 146, 146, 184);
    this->PWM_PH = new FuzzySet(146, 184, 184, 200);
    this->PWM_PVH = new FuzzySet(184, 200, 200, 200);

    // Fuzzy Inputs
    this->distance = new FuzzyInput(1);
    this->distance->addFuzzySet(this->error_negative_high);
    this->distance->addFuzzySet(this->error_negative);
    this->distance->addFuzzySet(this->error_zero);
    this->distance->addFuzzySet(this->error_positive);
    this->distance->addFuzzySet(this->error_positive_high);
    this->fuzzy->addFuzzyInput(this->distance);

    this->velocity = new FuzzyInput(2);
    this->velocity->addFuzzySet(this->velocity_negative_high);
    this->velocity->addFuzzySet(this->velocity_negative);
    this->velocity->addFuzzySet(this->velocity_zero);
    this->velocity->addFuzzySet(this->velocity_positive);
    this->velocity->addFuzzySet(this->velocity_positive_high);
    this->fuzzy->addFuzzyInput(this->velocity);

    // Fuzzy Outputs
    this->PWM_output = new FuzzyOutput(1);
    this->PWM_output->addFuzzySet(this->PWM_NVH);
    this->PWM_output->addFuzzySet(this->PWM_NH);
    this->PWM_output->addFuzzySet(this->PWM_NM);
    this->PWM_output->addFuzzySet(this->PWM_NS);
    this->PWM_output->addFuzzySet(this->PWM_Z);
    this->PWM_output->addFuzzySet(this->PWM_PS);
    this->PWM_output->addFuzzySet(this->PWM_PM);
    this->PWM_output->addFuzzySet(this->PWM_PH);
    this->PWM_output->addFuzzySet(this->PWM_PVH);
    this->fuzzy->addFuzzyOutput(this->PWM_output);

    // Fuzzy Rules
    this->ifErrorNegativeHighAndVelocityNegativeHigh = new FuzzyRuleAntecedent();
    this->ifErrorNegativeHighAndVelocityNegative = new FuzzyRuleAntecedent();
    this->ifErrorNegativeHighAndVelocityZero = new FuzzyRuleAntecedent();
    this->ifErrorNegativeHighAndVelocityPositive = new FuzzyRuleAntecedent();
    this->ifErrorNegativeHighAndVelocityPositiveHigh = new FuzzyRuleAntecedent();
    this->ifErrorNegativeAndVelocityNegativeHigh = new FuzzyRuleAntecedent();
    this->ifErrorNegativeAndVelocityNegative = new FuzzyRuleAntecedent();
    this->ifErrorNegativeAndVelocityZero = new FuzzyRuleAntecedent();
    this->ifErrorNegativeAndVelocityPositive = new FuzzyRuleAntecedent();
    this->ifErrorNegativeAndVelocityPositiveHigh = new FuzzyRuleAntecedent();
    this->ifErrorZeroAndVelocityNegativeHigh = new FuzzyRuleAntecedent();
    this->ifErrorZeroAndVelocityNegative = new FuzzyRuleAntecedent();
    this->ifErrorZeroAndVelocityZero = new FuzzyRuleAntecedent();
    this->ifErrorZeroAndVelocityPositive = new FuzzyRuleAntecedent();
    this->ifErrorZeroAndVelocityPositiveHigh = new FuzzyRuleAntecedent();
    this->ifErrorPositiveAndVelocityNegativeHigh = new FuzzyRuleAntecedent();
    this->ifErrorPositiveAndVelocityNegative = new FuzzyRuleAntecedent();
    this->ifErrorPositiveAndVelocityZero = new FuzzyRuleAntecedent();
    this->ifErrorPositiveAndVelocityPositive = new FuzzyRuleAntecedent();
    this->ifErrorPositiveAndVelocityPositiveHigh = new FuzzyRuleAntecedent();
    this->ifErrorPositiveHighAndVelocityNegativeHigh = new FuzzyRuleAntecedent();
    this->ifErrorPositiveHighAndVelocityNegative = new FuzzyRuleAntecedent();
    this->ifErrorPositiveHighAndVelocityZero = new FuzzyRuleAntecedent();
    this->ifErrorPositiveHighAndVelocityPositive = new FuzzyRuleAntecedent();
    this->ifErrorPositiveHighAndVelocityPositiveHigh = new FuzzyRuleAntecedent();

    this->ifErrorNegativeHighAndVelocityNegativeHigh->joinWithAND(this->error_negative_high, this->velocity_negative_high);
    this->ifErrorNegativeHighAndVelocityNegative->joinWithAND(this->error_negative_high, this->velocity_negative);
    this->ifErrorNegativeHighAndVelocityZero->joinWithAND(this->error_negative_high, this->velocity_zero);
    this->ifErrorNegativeHighAndVelocityPositive->joinWithAND(this->error_negative_high, this->velocity_positive);
    this->ifErrorNegativeHighAndVelocityPositiveHigh->joinWithAND(this->error_negative_high, this->velocity_positive_high);
    this->ifErrorNegativeAndVelocityNegativeHigh->joinWithAND(this->error_negative, this->velocity_negative_high);
    this->ifErrorNegativeAndVelocityNegative->joinWithAND(this->error_negative, this->velocity_negative);
    this->ifErrorNegativeAndVelocityZero->joinWithAND(this->error_negative, this->velocity_zero);
    this->ifErrorNegativeAndVelocityPositive->joinWithAND(this->error_negative, this->velocity_positive);
    this->ifErrorNegativeAndVelocityPositiveHigh->joinWithAND(this->error_negative, this->velocity_positive_high);
    this->ifErrorZeroAndVelocityNegativeHigh->joinWithAND(this->error_zero, this->velocity_negative_high);
    this->ifErrorZeroAndVelocityNegative->joinWithAND(this->error_zero, this->velocity_negative);
    this->ifErrorZeroAndVelocityZero->joinWithAND(this->error_zero, this->velocity_zero);
    this->ifErrorZeroAndVelocityPositive->joinWithAND(this->error_zero, this->velocity_positive);
    this->ifErrorZeroAndVelocityPositiveHigh->joinWithAND(this->error_zero, this->velocity_positive_high);
    this->ifErrorPositiveAndVelocityNegativeHigh->joinWithAND(this->error_positive, this->velocity_negative_high);
    this->ifErrorPositiveAndVelocityNegative->joinWithAND(this->error_positive, this->velocity_negative);
    this->ifErrorPositiveAndVelocityZero->joinWithAND(this->error_positive, this->velocity_zero);
    this->ifErrorPositiveAndVelocityPositive->joinWithAND(this->error_positive, this->velocity_positive);
    this->ifErrorPositiveAndVelocityPositiveHigh->joinWithAND(this->error_positive, this->velocity_positive_high);
    this->ifErrorPositiveHighAndVelocityNegativeHigh->joinWithAND(this->error_positive_high, this->velocity_negative_high);
    this->ifErrorPositiveHighAndVelocityNegative->joinWithAND(this->error_positive_high, this->velocity_negative);
    this->ifErrorPositiveHighAndVelocityZero->joinWithAND(this->error_positive_high, this->velocity_zero);
    this->ifErrorPositiveHighAndVelocityPositive->joinWithAND(this->error_positive_high, this->velocity_positive);
    this->ifErrorPositiveHighAndVelocityPositiveHigh->joinWithAND(this->error_positive_high, this->velocity_positive_high);

    this->thenRisPWM_NVH = new FuzzyRuleConsequent();
    this->thenRisPWM_NH = new FuzzyRuleConsequent();
    this->thenRisPWM_NM = new FuzzyRuleConsequent();
    this->thenRisPWM_NS = new FuzzyRuleConsequent();
    this->thenRisPWM_Z = new FuzzyRuleConsequent();
    this->thenRisPWM_PS = new FuzzyRuleConsequent();
    this->thenRisPWM_PM = new FuzzyRuleConsequent();
    this->thenRisPWM_PH = new FuzzyRuleConsequent();
    this->thenRisPWM_PVH = new FuzzyRuleConsequent();

    this->thenRisPWM_NVH->addOutput(this->PWM_NVH);
    this->thenRisPWM_NH->addOutput(this->PWM_NH);
    this->thenRisPWM_NM->addOutput(this->PWM_NM);
    this->thenRisPWM_NS->addOutput(this->PWM_NS);
    this->thenRisPWM_Z->addOutput(this->PWM_Z);
    this->thenRisPWM_PS->addOutput(this->PWM_PS);
    this->thenRisPWM_PM->addOutput(this->PWM_PM);
    this->thenRisPWM_PH->addOutput(this->PWM_PH);
    this->thenRisPWM_PVH->addOutput(this->PWM_PVH);

    this->fuzzyRule1  = new FuzzyRule(1, ifErrorNegativeHighAndVelocityNegativeHigh, thenRisPWM_NVH);   // esse número inicail tá certo?
    this->fuzzyRule2  = new FuzzyRule(2, ifErrorNegativeHighAndVelocityNegative, thenRisPWM_NVH);
    this->fuzzyRule3  = new FuzzyRule(3, ifErrorNegativeHighAndVelocityZero, thenRisPWM_NVH);
    this->fuzzyRule4  = new FuzzyRule(4, ifErrorNegativeHighAndVelocityPositive, thenRisPWM_NH);
    this->fuzzyRule5  = new FuzzyRule(5, ifErrorNegativeHighAndVelocityPositiveHigh, thenRisPWM_NS);
    this->fuzzyRule6  = new FuzzyRule(6, ifErrorNegativeAndVelocityNegativeHigh, thenRisPWM_NVH);
    this->fuzzyRule7  = new FuzzyRule(7, ifErrorNegativeAndVelocityNegative, thenRisPWM_NVH);
    this->fuzzyRule8  = new FuzzyRule(8, ifErrorNegativeAndVelocityZero, thenRisPWM_NH);
    this->fuzzyRule9  = new FuzzyRule(9, ifErrorNegativeAndVelocityPositive, thenRisPWM_NM);
    this->fuzzyRule10 = new FuzzyRule(10, ifErrorNegativeAndVelocityPositiveHigh, thenRisPWM_NS);
    this->fuzzyRule11 = new FuzzyRule(11, ifErrorZeroAndVelocityNegativeHigh, thenRisPWM_NM);
    this->fuzzyRule12 = new FuzzyRule(12, ifErrorZeroAndVelocityNegative, thenRisPWM_NS);
    this->fuzzyRule13 = new FuzzyRule(13, ifErrorZeroAndVelocityZero, thenRisPWM_Z);
    this->fuzzyRule14 = new FuzzyRule(14, ifErrorZeroAndVelocityPositive, thenRisPWM_PS);
    this->fuzzyRule15 = new FuzzyRule(15, ifErrorZeroAndVelocityPositiveHigh, thenRisPWM_PM);
    this->fuzzyRule16 = new FuzzyRule(16, ifErrorPositiveAndVelocityNegativeHigh, thenRisPWM_PS);
    this->fuzzyRule17 = new FuzzyRule(17, ifErrorPositiveAndVelocityNegative, thenRisPWM_PM);
    this->fuzzyRule18 = new FuzzyRule(18, ifErrorPositiveAndVelocityZero, thenRisPWM_PH);
    this->fuzzyRule19 = new FuzzyRule(19, ifErrorPositiveAndVelocityPositive, thenRisPWM_PVH);
    this->fuzzyRule20 = new FuzzyRule(20, ifErrorPositiveAndVelocityPositiveHigh, thenRisPWM_PVH);
    this->fuzzyRule21 = new FuzzyRule(21, ifErrorPositiveHighAndVelocityNegativeHigh, thenRisPWM_PS);
    this->fuzzyRule22 = new FuzzyRule(22, ifErrorPositiveHighAndVelocityNegative, thenRisPWM_PH);
    this->fuzzyRule23 = new FuzzyRule(23, ifErrorPositiveHighAndVelocityZero, thenRisPWM_PVH);
    this->fuzzyRule24 = new FuzzyRule(24, ifErrorPositiveHighAndVelocityPositive, thenRisPWM_PVH);
    this->fuzzyRule25 = new FuzzyRule(25, ifErrorPositiveHighAndVelocityPositiveHigh, thenRisPWM_PVH);

    this->fuzzy->addFuzzyRule(this->fuzzyRule1);
    this->fuzzy->addFuzzyRule(this->fuzzyRule2);
    this->fuzzy->addFuzzyRule(this->fuzzyRule3);
    this->fuzzy->addFuzzyRule(this->fuzzyRule4);
    this->fuzzy->addFuzzyRule(this->fuzzyRule5);
    this->fuzzy->addFuzzyRule(this->fuzzyRule6);
    this->fuzzy->addFuzzyRule(this->fuzzyRule7);
    this->fuzzy->addFuzzyRule(this->fuzzyRule8);
    this->fuzzy->addFuzzyRule(this->fuzzyRule9);
    this->fuzzy->addFuzzyRule(this->fuzzyRule10);
    this->fuzzy->addFuzzyRule(this->fuzzyRule11);
    this->fuzzy->addFuzzyRule(this->fuzzyRule12);
    this->fuzzy->addFuzzyRule(this->fuzzyRule13);
    this->fuzzy->addFuzzyRule(this->fuzzyRule14);
    this->fuzzy->addFuzzyRule(this->fuzzyRule15);
    this->fuzzy->addFuzzyRule(this->fuzzyRule16);
    this->fuzzy->addFuzzyRule(this->fuzzyRule17);
    this->fuzzy->addFuzzyRule(this->fuzzyRule18);
    this->fuzzy->addFuzzyRule(this->fuzzyRule19);
    this->fuzzy->addFuzzyRule(this->fuzzyRule20);
    this->fuzzy->addFuzzyRule(this->fuzzyRule21);
    this->fuzzy->addFuzzyRule(this->fuzzyRule22);
    this->fuzzy->addFuzzyRule(this->fuzzyRule23);
    this->fuzzy->addFuzzyRule(this->fuzzyRule24);
    this->fuzzy->addFuzzyRule(this->fuzzyRule25);
}

double Fuzzy_controller::CalculateOutput(double pos, double setpoint) {
    double current_time = millis();
    if (current_time - this->prev_time >= this->sample_time * 1000) {    // * 1000 to turn from s to ms
        double error = setpoint - pos;
        double error_velocity = (error - this->prev_error) / (current_time - this->prev_time);

        this->fuzzy->setInput(1, error);
        this->fuzzy->setInput(2, error_velocity);

        this->fuzzy->fuzzify();

        double output = fuzzy->defuzzify(1);

        this->prev_time = current_time;
        this->prev_out = output;
        return output;
    }
    return this->prev_out;
}

