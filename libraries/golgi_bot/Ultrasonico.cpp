/**
 * @file  Ultrasonico.cpp
 *
 * @brief Ultrasonico class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "Ultrasonico.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

Ultrasonico::Ultrasonico(int Trig_pin,int Echo_pin) {
    this->Trig_pin = Trig_pin;
    this->Echo_pin = Echo_pin;
}
void Ultrasonico::init(){
    pinMode(this->Trig_pin,OUTPUT);
    pinMode(this->Echo_pin,INPUT);

}

int Ultrasonico::get_distance(){
  digitalWrite(this->Trig_pin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(this->Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->Trig_pin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(this->Echo_pin, HIGH);

  // Calculating the distance
  distance = duration*0.034/2;

  return distance;
}


