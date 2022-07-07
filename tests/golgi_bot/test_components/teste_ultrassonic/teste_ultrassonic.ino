

// Ultrassonico
#include "Ultrasonico.hpp"
const int trigPin = 6;
const int echoPin = 7;
Ultrasonico *Ultrasonico_Y;


void setup() {
Serial.begin(9600); // Starts the serial communication
Ultrasonico_Y= new Ultrasonico(trigPin,echoPin);
Ultrasonico_Y->init();
}


void loop() {
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(Ultrasonico_Y->get_distance());
}
