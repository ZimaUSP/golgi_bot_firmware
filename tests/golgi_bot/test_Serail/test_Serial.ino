
#include "serial_communication.hpp"
#include "config.hpp"

SerialCommunication* comu;

void setup() {
    Serial.begin(SERIAL_VEL);
    comu = new SerialCommunication("teste communication");
};

void loop() {
    std::cout<< comu->get_name() <<std::endl;

    comu->read_data();
    std::cout<<comu->get_received_data()<<std::endl;

    comu->send_data("FAAFF\n");
};