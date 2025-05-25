/**
 * @file  Sliding_controller.cpp
 *
 * @brief Sliding Mode Controler Class
 *
 * @author Lucas Sposo Cavalari <lucassposocavalari@usp.br>
 *
 * @date 01/2025
 * 
 * @copyright MIT License
 */

#include "Sliding_controller.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Sliding_controller::Sliding_controller(double K, double Kt, double M, double J, double Kv, double Kp, double gama, double alpha, double R, double res, double sampling_time) {
    this->K = K;
    this->Kt = Kt;
    this->M = M;
    this->J = J;
    this->Kv = Kv;
    this->Kp = Kp;
    this->gama = gama;
    this->alpha = alpha;
    this->R = R;
    this->sampling_time = sampling_time;
    this->res = res;
    this->Teq = 0;
    this->prev_teq = 0;

    this->prev_position = {0};            
    this->prev_time = 0;
    this->prev_time2 = 0;
    this->prev_sigma = {0};
    this->prev_Dsigma = 0;
    this->prev_w = {0};
    this->prev_wder1 = {0};
    this->prev_vel = {0};
    this->prev_acel = 0;
    this->prev_output = 0;
}

double Sliding_controller::Compute_PWM_Output(double input, double setpoint) {
    double present_time = millis();

    if (present_time - this->prev_time >= this->sampling_time * 1000) { // * 1000 to turn from s to ms
        double vel, acel, phi, w, sigma, wder1, wder2, Dt, ac, Beta, FreqNat2, Dsigma, tansigmoid, Tu, vel_ang, Eb, V, PWM, pos, vel_influence;

        // parameters
        Dt = (present_time - this->prev_time) / 1000;    // ms to s        
        pos = input / 1000;                              // mm to m
        setpoint = setpoint / 1000;                      // mm to m
        vel = (this->prev_vel[0] * 0.6) + (0.4 * ((pos - this->prev_position[4]) / (5 * Dt)));
        acel = ((this->prev_acel * 0.2) + (0.8 * ((vel - this->prev_vel[4]) / (5 * Dt))));   
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.println(vel, 3);
        //     this->prev_time2 = present_time;
        // }        
        
        phi = pos / this->R;                              // angular position
        w = phi - pos;                                    // belt strech
        wder1 = (w - this->prev_w[4]) / (5 * Dt);                
        wder2 = (wder1 - this->prev_wder1[4]) / (5 * Dt);        

        if (setpoint - pos > 0) {
            ac = setpoint - ((this->Kv * vel) + ((this->Kp * pos) - 0.013));  
        } else {
            ac = setpoint - ((this->Kv * vel) + ((this->Kp * pos) + 0.013)); 
        }

        // calculation of equivalent torque
        Beta = this->K / (this->gama * this->M);
        FreqNat2 = this->K * ((1 / this->J) + (1 / this->M));
        this->Teq = ((Beta / FreqNat2) * (this->J + this->M) * ac) - (this->J * (this->alpha * wder1 + (Beta - FreqNat2) * w));

        // non linear factors
        sigma = (setpoint - ((this->Kv * vel) + (this->Kp * pos) + (this->gama * (wder2 + (this->alpha * wder1)))));   
        Dsigma = - ((sigma - this->prev_sigma[4]) / (5 * Dt)); 
        tansigmoid = tanh(sigma);                              

        // initial filter on the sigma derivate to prevent it from exploding
        if (this->prev_sigma[4] == 0){
            Dsigma = 0;                                 
        }

        // calculation of necessary torque
        Tu = this->Teq + (tansigmoid * ((Beta / FreqNat2) * (this->J + this->M) * Dsigma));

        // conversion of torque into PWM
        vel_ang = vel / R;
        Eb = vel_ang * this->Kt;
        V = ((Tu * this->res) / this->Kt) + Eb;   
        
        PWM = (V / 11.45) * 255; // V max and max PWM

        this->prev_position[0] = pos;   
        this->prev_sigma[0] = sigma;
        this->prev_w[0] = w;
        this->prev_wder1[0] = wder1;
        this->prev_vel[0] = vel;

        for (int i = 0; i < 4; i++) {
            this->prev_position[i+1] = this->prev_position[i];
            this->prev_sigma[i+1] = this->prev_sigma[i];   
            this->prev_w[i+1] = this->prev_w[i];   
            this->prev_wder1[i+1] = this->prev_wder1[i];   
            this->prev_vel[i+1] = this->prev_vel[i];   
        }
   
        this->prev_time = present_time;
        this->prev_Dsigma = Dsigma;
        this->prev_acel = acel;
        this->prev_output = PWM;
        this->prev_teq = this->Teq;

        return PWM;
    }
    return this->prev_output;
}

void Sliding_controller::reset_SMC_controller() {
    this->prev_position = {0};              
    this->prev_time = 0;
    this->prev_sigma = {0};
    this->prev_Dsigma = 0;
    this->prev_w = {0};
    this->prev_wder1 = {0};
    this->prev_vel = {0};
    this->prev_output = 0;
}

double Sliding_controller::getTeq() {
    return this->Teq;
}

double Sliding_controller::getOutputPWM() {
    return this->prev_output;
}

double Sliding_controller::getVelocity() {
    return this->prev_vel[0];
}

double Sliding_controller::getAcel() {
    return this->prev_acel;
}

