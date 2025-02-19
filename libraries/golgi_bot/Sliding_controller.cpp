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

    this->prev_position = 0;    
    this->prev_position1 = 0;  
    this->prev_position2 = 0;   
    this->prev_position3 = 0;   
    this->prev_position4 = 0;                
    this->prev_time = 0;
    this->prev_time2 = 0;
    this->prev_sigma = 0;
    this->prev_sigma1 = 0;
    this->prev_sigma2 = 0;
    this->prev_sigma3 = 0;
    this->prev_sigma4 = 0;
    this->prev_Dsigma = 0;
    this->prev_w = 0;
    this->prev_w1 = 0;
    this->prev_w2 = 0;
    this->prev_w3 = 0;
    this->prev_w4 = 0;
    this->prev_wder1 = 0;
    this->prev_wder1_1 = 0;
    this->prev_wder1_2 = 0;
    this->prev_wder1_3 = 0;
    this->prev_wder1_4 = 0;
    this->prev_vel = 0;
    this->prev_vel1 = 0;  
    this->prev_vel2 = 0;   
    this->prev_vel3 = 0;   
    this->prev_vel4 = 0; 
    this->prev_acel = 0;
    this->prev_output = 0;
}

double Sliding_controller::Compute_PWM_Output(double input, double setpoint) {
    double present_time = millis();

    if (present_time - this->prev_time >= this->sampling_time * 1000) { // * 1000 to turn from s to ms
        double vel, acel, phi, w, sigma, wder1, wder2, Dt, ac, Beta, FreqNat2, Dsigma, tansigmoid, Tu, vel_ang, Eb, V, PWM, pos, vel_influence;
        // const int window_size = 50;  // Número de amostras usadas na média
        // static double vel_buffer[window_size] = {0}; 
        // static int index = 0;

        Dt = (present_time - this->prev_time) / 1000;    // ms to s        
        pos = input / 1000;                              // mm to m
        // Serial.println(pos, 5);
        setpoint = setpoint / 1000;                      // mm to m

        // calculation of dependent variables
        // vel = ((pos - this->prev_position) / Dt);            
        // if (vel > this->prev_vel) {
        //     vel = this->prev_vel;
        // }
        // double raw_vel = (pos - this->prev_position) / Dt; 
        // vel_buffer[index] = raw_vel;
        // index = (index + 1) % window_size;

        // double vel = 0;
        // for (int i = 0; i < window_size; i++) {
        //     vel += vel_buffer[i];
        // }
        // vel /= window_size;  // Média das últimas 'window_size' amostras


        vel = (this->prev_vel * 0.6) + (0.4 * ((pos - this->prev_position4) / (5 * Dt)));
        acel = ((this->prev_acel * 0.2) + (0.8 * ((vel - this->prev_vel4) / (5 * Dt))));   
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.println(vel, 3);
        //     this->prev_time2 = present_time;
        // }        
        
        phi = pos / this->R;                              // angular position
        w = phi - pos;                                    // belt strech
        wder1 = (w - this->prev_w4) / (5 * Dt);                
        wder2 = (wder1 - this->prev_wder1_4) / (5 * Dt);        


        ac = setpoint - ((this->Kv * vel) + ((this->Kp * pos) - 0.02));  // -0.02

        // calculation of equivalent torque
        Beta = this->K / (this->gama * this->M);
        FreqNat2 = this->K * ((1 / this->J) + (1 / this->M));
        this->Teq = ((Beta / FreqNat2) * (this->J + this->M) * ac) - (this->J * (this->alpha * wder1 + (Beta - FreqNat2) * w));
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.print(Beta);
        //     Serial.print(", ");
        //     Serial.println(FreqNat2);
        //     this->prev_time2 = present_time;
        // }

        // non linear factors
        sigma = (setpoint - ((this->Kv * vel) + (this->Kp * pos) + (this->gama * (wder2 + (this->alpha * wder1)))));   // removi a acel
        Dsigma = - ((sigma - this->prev_sigma4) / (5 * Dt)); 
        tansigmoid = tanh(sigma);                               // preciso ver essa fórmula, acho q precisa colocar a biblioteca de matemática

        // if (present_time - this->prev_time2 >= 10) {
        //     Serial.println((Dsigma), 3);
        //     this->prev_time2 = present_time;
        // }

        // initial filter on the sigma derivate to prevent it from exploding
        if (this->prev_sigma4 == 0){
            Dsigma = 0;                                   // testar se isso é necessário no real
        }

        // calculation of necessary torque
        Tu = this->Teq + (tansigmoid * ((Beta / FreqNat2) * (this->J + this->M) * Dsigma));
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.println((tansigmoid * ((Beta / FreqNat2) * (this->J + this->M) * Dsigma)));
        //     this->prev_time2 = present_time;
        // }

        // conversion of torque into PWM
        vel_ang = vel / R;
        Eb = vel_ang * this->Kt;
        V = ((Tu * this->res) / this->Kt) + Eb;    // Preciso checar a resistência 
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.println(V);
        //     this->prev_time2 = present_time;
        // }
        
        PWM = (V / 11.45) * 255;
        // if (present_time - this->prev_time2 >= 30) {
        //     Serial.println(PWM);
        //     this->prev_time2 = present_time;
        // }

        this->prev_position = pos;   
        this->prev_position1 = this->prev_position;    
        this->prev_position2 = this->prev_position1; 
        this->prev_position3 = this->prev_position2; 
        this->prev_position4 = this->prev_position3;    
        this->prev_time = present_time;
        this->prev_sigma = sigma;
        this->prev_sigma1 = this->prev_sigma;
        this->prev_sigma2 = this->prev_sigma1;
        this->prev_sigma3 = this->prev_sigma2;
        this->prev_sigma4 = this->prev_sigma3;
        this->prev_Dsigma = Dsigma;
        this->prev_w = w;
        this->prev_w1 = this->prev_w;
        this->prev_w2 = this->prev_w1;
        this->prev_w3 = this->prev_w2;
        this->prev_w4 = this->prev_w3;
        this->prev_wder1 = wder1;
        this->prev_wder1_1 = this->prev_wder1;
        this->prev_wder1_2 = this->prev_wder1_2;
        this->prev_wder1_3 = this->prev_wder1_3;
        this->prev_wder1_4 = this->prev_wder1_4;
        this->prev_vel = vel;
        this->prev_vel1 = this->prev_vel;
        this->prev_vel2 = this->prev_vel1;
        this->prev_vel3 = this->prev_vel2;
        this->prev_vel4 = this->prev_vel3;
        this->prev_acel = acel;
        this->prev_output = PWM;
        this->prev_teq = this->Teq;

        return PWM;
    }
    return this->prev_output;
}

void Sliding_controller::reset_SMC_controller() {
    this->prev_position = 0;
    this->prev_position1 = 0;  
    this->prev_position2 = 0;   
    this->prev_position3 = 0;   
    this->prev_position4 = 0;               
    this->prev_time = 0;
    this->prev_sigma = 0;
    this->prev_sigma1 = 0;
    this->prev_sigma2 = 0;
    this->prev_sigma3 = 0;
    this->prev_sigma4 = 0;
    this->prev_Dsigma = 0;
    this->prev_w = 0;
    this->prev_w1 = 0;
    this->prev_w2 = 0;
    this->prev_w3 = 0;
    this->prev_w4 = 0;
    this->prev_wder1 = 0;
    this->prev_wder1_1 = 0;
    this->prev_wder1_2 = 0;
    this->prev_wder1_3 = 0;
    this->prev_wder1_4 = 0;
    this->prev_vel = 0;
    this->prev_output = 0;
}

double Sliding_controller::getTeq() {
    return this->Teq;
}

double Sliding_controller::getOutputPWM() {
    return this->prev_output;
}

double Sliding_controller::getVelocity() {
    return this->prev_vel;
}

double Sliding_controller::getAcel() {
    return this->prev_acel;
}

