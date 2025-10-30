/**
 * @file  Axis.cpp
 *
 * @brief Axis class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 12/2021
 * 
 * @copyright MIT License
 */

#include "Controller.hpp"
#include <chrono>


/***************
 * Class Methods Bodies Definitions
 ***************/

Controller::Controller(Axis *Axis_1, Axis *Axis_2, Axis *Axis_3, Bomba *Bomba_Y, Atuador *Atuador_Y){
    this->Axis_1 = Axis_1;                   // master x
    this->Axis_2 = Axis_2;                   // slave x
    this->Axis_3 = Axis_3;                   // z
    this->Bomba_Y= Bomba_Y;
    this->Atuador_Y= Atuador_Y;
}

void Controller::setGoal(double goal_axis_1, double goal_axis_2, double goal_axis_3){
  this->Axis_1->setGoal(goal_axis_1);
  this->Axis_2->setGoal(goal_axis_2);
  this->Axis_3->setGoal(goal_axis_3);
}

void Controller::move(){
   this->Axis_1->move( );
   this->Axis_2->move( );
   this->Axis_3->move( );
}

void Controller::get_medicine(int DELAY_EX, int DELAY_CON){
  this->Bomba_Y->turn_on();
  this->Atuador_Y->Extend();
  delay(DELAY_EX);
  this->Atuador_Y->Stop();
  delay(1000);
  this->Atuador_Y->Contract();
  delay(DELAY_CON);
  this->Atuador_Y->Stop();
}

void Controller::drop_medicine(){
  this->go_origin(true, true);          // may not reach endstop
  delay(1000);
  this->Bomba_Y->turn_off();
  this->reset_PID();
}

void Controller::go_origin(bool axis1,bool axis2){
  bool going = true;
  bool axis1OnOrigin = false; 
  bool axis2OnOrigin = false;
  bool axis3OnOrigin = false;

  bool xOnOrigin = false;
  bool zOnOrigin = false;

  while (going) {

    axis1OnOrigin = this->Axis_1->onOrigin();
    axis2OnOrigin = this->Axis_2->onOrigin();
    axis3OnOrigin = this->Axis_3->onOrigin();

    if (axis1OnOrigin && axis2OnOrigin && (!xOnOrigin)) {  
      //Serial.println("ENTROU X");
      this->Axis_1->resetOrigin();
      this->Axis_1->stop();
      axis2OnOrigin = true;
    
      this->Axis_2->resetOrigin();
      this->Axis_2->stop();
      axis1OnOrigin = true;

      xOnOrigin = true;
    } else if (xOnOrigin == false) {
      this->Axis_1->go_R();
      this->Axis_2->go_R();
    } else {
      delay(1);                           // get possible exceptions
    }

    if (axis3OnOrigin && (!zOnOrigin)) {
      //Serial.println("ENTROU Z");
      this->Axis_3->resetOrigin();
      this->Axis_3->stop();
      zOnOrigin = true;
    } else if (zOnOrigin == false){
      this->Axis_3->go_R();
    }

    if (xOnOrigin && zOnOrigin) {
      going = false;
    }
  }
  //Serial.println("going origin no more");
}

void Controller::go_origin_suavizado(){  // doesn't reset origin
  this->Axis_1->setPoint(0);
  this->Axis_2->setPoint(0);
  bool zOnMax = false;

  while (!((this->Axis_1->onGoal() && this->Axis_2->onGoal()) || (this->Axis_1->onOrigin() || this->Axis_2->onOrigin()) && this->Axis_3->onOrigin())) {
    bool axis3OnMax = this->Axis_3->onOrigin();

    if ((this->Axis_1->onGoal() && this->Axis_2->onGoal()) || (this->Axis_1->onOrigin() || this->Axis_2->onOrigin())) {
      this->Axis_1->stop();
      this->Axis_2->stop();
    } else {
      this->Axis_1->move();
      this->Axis_2->move();
    } 
    
    if (axis3OnMax && (!zOnMax)) {
      this->Axis_3->stop();
      zOnMax = true;
      delay(10);
    } else if (zOnMax == false) {
      this->Axis_3->go_R();
    }
  }
  this->Axis_1->stop();
  this->Axis_2->stop();
  this->Axis_3->stop();
}

void Controller::go_max(bool axis1,bool axis2, bool axis3){
  bool going = true;
  bool axis1OnMax = false; 
  bool axis2OnMax = false;
  bool axis3OnMax = false;
  //Serial.println("going max");

  bool xOnMax = false;
  bool zOnMax = false;

  while (going) {
    //Serial.println("going max");
    axis1OnMax = this->Axis_1->onMax();
    axis2OnMax = this->Axis_2->onMax();
    axis3OnMax = this->Axis_3->onMax();

    this->Axis_1->go_L();
    // this->Axis_2->setPoint(this->Axis_1->position());

if (axis1OnMax && axis2OnMax && (!xOnMax)) {  
      //Serial.println("ENTROU X");
      this->Axis_1->resetMax();
      this->Axis_1->stop();
      axis2OnMax= true;
    
      this->Axis_2->resetMax();
      this->Axis_2->stop();
      axis1OnMax = true;

      xOnMax = true;
    } else {
      //this->Axis_2->move();
      //delay(2);
      this->Axis_2->go_L();
    }

    if (axis3OnMax && (!zOnMax)) {
      // Serial.println("ENTROU Z");
      this->Axis_3->resetMax();
      this->Axis_3->stop();
      zOnMax = true;
      delay(10);
    }
    else if (zOnMax == false) {
      this->Axis_3->go_L();
    }

    if (xOnMax && zOnMax) {
      going = false;
      //Serial.println("going max no more");
  }
}
  delay(100);
}

void Controller::stop(bool axis1,bool axis2, bool axis3){
   if(axis1){
    this->Axis_1->stop();
    //Serial.println("Axis1 stop");
  }
  if(axis2){
    this->Axis_2->stop();
    //Serial.println("Axis2 stop");
  }
  if(axis3){
    this->Axis_3->stop();
  }
}

void Controller::reset_PID(){
   this->Axis_1->reset();
   this->Axis_2->reset();
   this->Axis_3->reset();
}

void Controller::reset_Y(int DELAY_CON){
  this->Bomba_Y->turn_off();
  this->Atuador_Y->Contract();
  delay(DELAY_CON);
  this->Atuador_Y->Stop();
}

float* Controller::positionPoint(){
  position[0]=this->Axis_1->position();
  position[1]=this->Axis_2->position();
  position[2]=this->Axis_3->position();
  return position;
}

void Controller::setEnvelope(float tolerance1,float tolerance2){
  this->Axis_1->setEnvelope(tolerance1);
  this->Axis_2->setEnvelope(tolerance2);
}

bool Controller::onGoal(){
  if(this->Axis_1->onGoal() && this->Axis_2->onGoal() && this->Axis_3->onGoal()){
    return true;
  }else{
    return false;
  }
}

void Controller::autoTunning() {
  const int numTests = 5;
  const double timeout = 1000.0; // milisegundos
  const double tolerancia = 4.0;

  for (int i = 1; i <= numTests; ++i) {
      this->setGoal(100*i, 100*i, 40*i);
      auto startTime = millis();

      double maxX1 = 0, maxX2 = 0, maxZ = 0;
      while ((this->Axis_1->onGoal() && this->Axis_2->onGoal()) || ((millis() - startTime) < timeout)) {
          this->move();
          if (maxX1 < this->Axis_1->position()) maxX1 = this->Axis_1->position();
          if (maxX2 < this->Axis_2->position()) maxX2 = this->Axis_2->position();
          // if (maxZ < Axis_3->position()) maxZ = Axis_3->position();   testar sem o z primeiro
      }

      double settleTime = millis() - startTime;  // talvez seja melhor calibrar o Z separadamente
      double goalX = 100*i;
      double goalZ = 40*i;

      double overshootX1 = ((maxX1 - goalX) / goalX) * 100.0;
      double overshootX2 = ((maxX2 - goalX) / goalX) * 100.0;
      double overshootZ  = ((maxZ  - goalZ) / goalZ) * 100.0;

      double erroX1 = fabs(goalX - Axis_1->position());
      double erroX2 = fabs(goalX - Axis_2->position());
      double erroZ  = fabs(goalZ - Axis_3->position());
    
      Serial.print("Test "); Serial.println(i);

      // --- Ajuste por eixo ---
      auto tuneAxis = [&](Axis* axis, double overshoot, double erro) {
          auto params = axis->getSystemParameters();
          double Kp = params[0], Ki = params[1], Kd = params[2];

          if (overshoot > 10.0) { Kp *= 0.9; Kd *= 1.1; }
          else if (overshoot < 2.0 && erro > tolerancia) { Ki *= 1.1; }
          else if (overshoot < 2.0 && erro <= tolerancia) { /* mantém */ }
          else if ((millis() - startTime) >= timeout) { Kp *= 1.3; }

          axis->setSystemParameters(Kp, Ki, Kd);
      };

      Serial.print("Test ");

      tuneAxis(this->Axis_1, overshootX1, erroX1);
      tuneAxis(this->Axis_2, overshootX2, erroX2);
      tuneAxis(this->Axis_3, overshootZ,  erroZ);
  }
}

// double* Controller::getControllerParameters
