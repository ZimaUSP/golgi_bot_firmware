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
  this->go_origin(true,true);
  delay(1000);
  this->Bomba_Y->turn_off();
  this->reset_PID();
}

void Controller::go_origin(bool axis1,bool axis2){
  bool going = true;
  bool axis1OnOrigin = false; //caso axis1 seja false axis1OnMax fica true como padrão 
  bool axis2OnOrigin = false;
  bool axis3OnOrigin = false;

  bool xOnOrigin = false;
  bool zOnOrigin = false;
  //Serial.println("going origin");

  while (going) {

    axis1OnOrigin = this->Axis_1->onOrigin();
    axis2OnOrigin = this->Axis_2->onOrigin();
    axis3OnOrigin = this->Axis_3->onOrigin();

    this->Axis_3->go_R();

    //Serial.println(this->Axis_2->onGoal())

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
      this->Axis_2->setPoint((this->Axis_1->position()));
      delay(2);
      this->Axis_2->move();
    } else {
      delay(1);                           // só para pegar possíveis excessões
    }

    if (axis3OnOrigin && (!zOnOrigin)) {
      //Serial.println("ENTROU Z");
      this->Axis_3->resetOrigin();
      this->Axis_3->stop();
      zOnOrigin = true;
    }

    if (xOnOrigin && zOnOrigin) {
      going = false;
      this->Axis_1->stop();
      this->Axis_2->stop();
      this->Axis_3->stop();
    }
  }
  //Serial.println("going origin no more");
}

void Controller::go_max(bool axis1,bool axis2, bool axis3){
  bool going = true;
  bool axis1OnMax = false; //caso axis1 seja false axis1OnMax fica true como padrão 
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
    this->Axis_2->setPoint(this->Axis_1->position());

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
      this->Axis_2->move();
      delay(2);
    }

    if (axis3OnMax && (!zOnMax)) {
      //Serial.println("ENTROU Z");
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