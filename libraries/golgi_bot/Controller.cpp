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


/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Controller::Controller(Axis *Axis_1, Axis *Axis_2, Bomba *Bomba_Y, Atuador *Atuador_Y){
    this->Axis_1 = Axis_1;
    this->Axis_2= Axis_2;
    this->Bomba_Y= Bomba_Y;
    this->Atuador_Y= Atuador_Y;
}

void Controller::setGoal(double goal_axis_1,double goal_axis_2){
  this->Axis_1->setGoal(goal_axis_1);
  this->Axis_2->setGoal(goal_axis_2);
}

void Controller::move(){
   this->Axis_1->move( );
   this->Axis_2->move( );
}

void Controller::get_medicine(int DELAY_EX, int DELAY_CON){
  this->Bomba_Y->turn_on();
  this->Atuador_Y->Extend();
  delay(DELAY_EX);
  this->Atuador_Y->Contract();
  delay(DELAY_CON);
  this->Atuador_Y->Stop();
}

void Controller::drop_medicine(){
  this->go_origin(true,false);
  this->go_max(false,true);
  this->Bomba_Y->turn_off();
  this->reset_PID();
}

void Controller::go_origin(bool axis1,bool axis2){
  if(axis1){
    this->Axis_1->go_origin();
  }
  if(axis2){
    this->Axis_2->go_origin();
  }
}

void Controller::go_max(bool axis1,bool axis2){
  if(axis1){
    this->Axis_1->go_max();
  }
  if(axis2){
    this->Axis_2->go_max();
  }
}

void Controller::stop(bool axis1,bool axis2){
   if(axis1){
    this->Axis_1->stop();
  }
  if(axis2){
    this->Axis_2->stop();
  }
}

void Controller::reset_PID(){
   this->Axis_1->reset();
   this->Axis_2->reset();
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
  return  position;
}

void Controller::setEnvelope(float tolerance1,float tolerance2){
  this->Axis_1->setEnvelope(tolerance1);
  this->Axis_2->setEnvelope(tolerance2);
}

bool Controller::onGoal(){
  if(this->Axis_1->onGoal() && this->Axis_2->onGoal()){
    return true;
  }else{
    return false;
  }
  
}