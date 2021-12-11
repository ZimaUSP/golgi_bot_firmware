#include "Motion_Control.h"
#include "Arduino.h"

static Encoder *_instance0;
static Axis *_instance1;

// Funções da classe Encoder

Encoder::Encoder(int pin_IN1, int pin_IN2, int pulses, double pitch, byte mode){

  _pinIN1 = pin_IN1;
  _pinIN2 = pin_IN2;
  _pitchPerRev = pitch;
  _detectionMode = mode;

  pinMode(pin_IN1, INPUT_PULLUP);
  pinMode(pin_IN2, INPUT_PULLUP);

  _currentState = (digitalRead(pin_IN1) << 1) | digitalRead(pin_IN2);
  _currentEventTime = 1;
  _lastEventTime = 0;
  _direction = 1;

  if(mode == 0){

    _pulsesPerRev = pulses;

    attachInterrupt(digitalPinToInterrupt(pin_IN1), ISREvent1, RISING);

  }else if(mode == 1){

    _pulsesPerRev = pulses * 2;

    attachInterrupt(digitalPinToInterrupt(pin_IN1), ISREvent2, CHANGE);

  } else {

    _pulsesPerRev = pulses * 4;

    attachInterrupt(digitalPinToInterrupt(pin_IN1), ISREvent3, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin_IN2), ISREvent3, CHANGE);

  }

  _instance0 = this;

}

void Encoder::ISREvent1(){
  _instance0->runInterrupt1();
}

void Encoder::ISREvent2(){
  _instance0->runInterrupt2();
}

void Encoder::ISREvent3(){
  _instance0->runInterrupt3();
}

void Encoder::runInterrupt1(){

  _lastEventTime = _currentEventTime;
  _currentEventTime = micros();

  _currentState = digitalRead(_pinIN2);

  _currentPulses = _currentPulses + _fwdOrder1[int(_currentState)];
  _direction = _fwdOrder1[int(_currentState)];

}

void Encoder::runInterrupt2(){

  _lastEventTime = _currentEventTime;
  _currentEventTime = micros();

  _currentState = (digitalRead(_pinIN1) << 1) | digitalRead(_pinIN2);

  _currentPulses = _currentPulses + _fwdOrder2[_currentState];
  _direction = _fwdOrder2[_currentState];

}

void Encoder::runInterrupt3(){

  _lastEventTime = _currentEventTime;
  _currentEventTime = micros();

  _lastState = _currentState;
  _currentState = (digitalRead(_pinIN1) << 1) | digitalRead(_pinIN2);

  if(_lastState == _fwdOrder4[_currentState]){
    _currentPulses++;
    _direction = 1;
  } else {
    _currentPulses--;
    _direction = -1;
  }

}

void Encoder::setReference(double value){

  _currentPulses = value;

}

long Encoder::readPulses(){

  return _currentPulses;

}

double Encoder::readPosition(bool mode){

  if(mode == 0) return ((double)_currentPulses * _pitchPerRev) / (double)_pulsesPerRev;
  else return (double)_currentPulses / (double)_pulsesPerRev;

}

double Encoder::readSpeed(bool mode){

  long delta = _currentEventTime - _lastEventTime;

  if(delta < 1){
    delta = 1;
  }

  if(micros() - _currentEventTime > 10000){
    return 0;
  } else {
    if(mode == 0){
      return (_pitchPerRev * _direction * 1000000) / (_pulsesPerRev * delta);
    } else {
      return (60 * 1000000 * (double)_direction) / (_pulsesPerRev * delta);
    }
  }

}


// Funções da classe Axis
Axis::Axis(int pin_output, int pin_IN1, int pin_IN2){

  _pinOutput = pin_output;
  _pinIN1 = pin_IN1;
  _pinIN2 = pin_IN2;

  _debug = -1;
  _homeStatus = -1;
  _output = 255;
  _maxOutput = 255;
  _opMode = 0;
  _errorCode = -1;

  _instance1 = this;

  pinMode(pin_IN1, OUTPUT);
  pinMode(pin_IN2, OUTPUT);

  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(pin_output, control_channel);
    ledcSetup(control_channel 5000, 12);
  #else
    pinMode(pin_output, OUTPUT);
  #endif

}

Axis::Axis(int pin_output, int pin_IN1, int pin_IN2, int pin_limitMin, int pin_limitMax){

  _pinOutput = pin_output;
  _pinIN1 = pin_IN1;
  _pinIN2 = pin_IN2;
  _pinLimitMin = pin_limitMin;
  _pinLimitMax = pin_limitMax;

  _debug = -1;
  _homeStatus = -1;
  _output = 255;
  _maxOutput = 255;
  _opMode = 0;
  _errorCode = -1;

  _instance1 = this;

  pinMode(pin_IN1, OUTPUT);
  pinMode(pin_IN2, OUTPUT);

  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(pin_output, control_channel);
    ledcSetup(control_channel 5000, 12);
  #else
    pinMode(pin_output, OUTPUT);
  #endif

  pinMode(pin_limitMin, INPUT_PULLUP);
  pinMode(pin_limitMax, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pin_limitMin), ISREvent, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin_limitMax), ISREvent, FALLING);

}

Axis::Axis(int pin_output, int control_channel, int pin_IN1, int pin_IN2){

  _pinOutput = pin_output;
  _controlChannel = control_channel;
  _pinIN1 = pin_IN1;
  _pinIN2 = pin_IN2;

  _debug = -1;
  _homeStatus = -1;
  _output = 4095;
  _maxOutput = 4095;
  _opMode = 0;
  _errorCode = -1;

  _instance1 = this;

  pinMode(pin_IN1, OUTPUT);
  pinMode(pin_IN2, OUTPUT);

  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(pin_output, control_channel);
    ledcSetup(control_channel 5000, 12);
  #else
    pinMode(pin_output, OUTPUT);
  #endif

}

Axis::Axis(int pin_output, int control_channel, int pin_IN1, int pin_IN2, int pin_limitMin, int pin_limitMax){

  _pinOutput = pin_output;
  _controlChannel = control_channel;
  _pinIN1 = pin_IN1;
  _pinIN2 = pin_IN2;
  _pinLimitMin = pin_limitMin;
  _pinLimitMax = pin_limitMax;

  _debug = 0;
  _homeStatus = 0;
  _output = 4095;
  _maxOutput = 4095;
  _opMode = 0;
  _errorCode = -1;

  _instance1 = this;

  pinMode(pin_IN1, OUTPUT);
  pinMode(pin_IN2, OUTPUT);

  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(pin_output, control_channel);
    ledcSetup(control_channel 5000, 12);
  #else
    pinMode(pin_output, OUTPUT);
  #endif

  pinMode(pin_limitMin, INPUT_PULLUP);
  pinMode(pin_limitMax, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pin_limitMin), ISREvent, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin_limitMax), ISREvent, FALLING);

}

void Axis::ISREvent(){
  _instance1->runInterrupt();
}

void Axis::runInterrupt(){

  if(_opMode != 3){

    if(digitalRead(_pinLimitMax) == HIGH){

      setError(4);

    } else {

      setError(5);

    }

  } else {

    setError(6);

  }

}

void Axis::setOpMode(int mode){

  if(mode >= 0 && mode < 3){

    _opMode = mode;

  } else if(mode == 3){

    if(_homeStatus = - 1){

      _opMode = 0;
      setError(4);

    } else {

      _opMode = mode;
      _homeStatus = 0;

    }

  } else {

    setError(1);

  }

}

int Axis::getOpMode(){

  return _opMode;

}

void Axis::setOutput(double value){

  if(value >= 0 && value <= 1){

    _output = int(value * _maxOutput);

  } else {

    setError(1);

  }


}

void Axis::setMaxOutput(int value){

  if(value >= 1 && value <= 4095) {
    _maxOutput = value;
  } else {
    setError(2);
  }

}

bool Axis::getHoming(){

  return _homeStatus;

}

void Axis::setDebug(bool value){

  _debug = value;

}

void Axis::moveAxisFoward(){

  if(_opMode != 0){

    int output = 0;

    switch (_opMode){

      case 1: // axis on normal operation

        output = _output;
        break;

      case 2: // axis on setting operation

        output = int(0.1 * _maxOutput);
        break;

      case 3: // axis on homing operation

        output = int(0.05 * _maxOutput);
        break;

    }

    digitalWrite(_pinIN1, HIGH);
    digitalWrite(_pinIN2, LOW);

    #if defined(ARDUINO_ARCH_ESP32)
      ledcWrite(_controlChannel, output);
    #else
      analogWrite(_pinOutput, output);
    #endif

  } else {

    setError(0);

  }

}

void Axis::moveAxisBackward(){

  if(_opMode != 0){

    int output = 0;

    switch (_opMode){

      case 1: // axis on normal operation

        output = _output;
        break;

      case 2: // axis on setting operation

        output = int(0.1 * _maxOutput);
        break;

      case 3: // axis on homing operation

        output = int(0.05 * _maxOutput);
        break;

    }

    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, HIGH);

    #if defined(ARDUINO_ARCH_ESP32)
      ledcWrite(_controlChannel, output);
    #else
      analogWrite(_pinOutput, output);
    #endif

  } else {

    setError(0);

  }

}

void Axis::brakeAxis(){

  if(_opMode != 0){

    digitalWrite(_pinIN1, HIGH);
    digitalWrite(_pinIN2, HIGH);

    #if defined(ARDUINO_ARCH_ESP32)
      ledcWrite(_controlChannel, 0);
    #else
      analogWrite(_pinOutput, 0);
    #endif

  } else {

    setError(0);

  }

}

void Axis::looseAxis(){

  digitalWrite(_pinIN1, LOW);
  digitalWrite(_pinIN2, LOW);

  #if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(_controlChannel, 0);
  #else
    analogWrite(_pinOutput, 0);
  #endif

}

void Axis::setError(int code){

  _errorCode = code;

  switch (code) {

    case 0:

      if(_debug == 1) Serial.println("Axis is deactivated");

      break;

    case 1:

      looseAxis();
      _opMode = 0;

      if(_debug == 1) Serial.println("The output value should be between 0 and 1");

      break;

    case 2:

      if(_debug == 1) Serial.println("The max output value shoud be between 1 and 4095");

      break;

    case 3:

        _opMode = 0;

        if(_debug == 1) Serial.println("It is not possible to set this axis to homing operation");

        break;

    case 4:

      looseAxis();
      _opMode = 0;

      if(_debug == 1) Serial.println("Axis max switch pressed");

      break;

    case 5:

      looseAxis();
      _opMode = 0;

      if(_debug == 1) Serial.println("Axis min switch pressed");

      break;

    case 6:

      looseAxis();
      _opMode = 0;
      _homeStatus = 1;

      if(_debug == 1) Serial.println("Homing routine done");

      break;


  }

}


// Funções da classe Controller

Controller::Controller(Axis *axis1, Encoder *encoder1){

  _axis1 = axis1;
  _encoder1 = encoder1;

  inicializePID(&_pid1);
  inicializeEnvelope(&_envelope);

  _opMode = 0;
  _nAxis = 1;
  _envelopeStatus = 0;

}

Controller::Controller(Axis *axis1, Encoder *encoder1, Axis *axis2, Encoder *encoder2){

  _axis1 = axis1;
  _axis2 = axis2;

  _encoder1 = encoder1;
  _encoder2 = encoder2;

  inicializePID(&_pid1);
  inicializePID(&_pid2);
  inicializeEnvelope(&_envelope);

  _opMode = 0;
  _nAxis = 2;
  _envelopeStatus = 0;

}

Controller::Controller(Axis *axis1, Encoder *encoder1, Axis *axis2, Encoder *encoder2, Axis *axis3, Encoder *encoder3){

  _axis1 = axis1;
  _axis2 = axis2;
  _axis3 = axis3;

  _encoder1 = encoder1;
  _encoder2 = encoder2;
  _encoder3 = encoder3;

  inicializePID(&_pid1);
  inicializePID(&_pid2);
  inicializePID(&_pid3);
  inicializeEnvelope(&_envelope);

  _opMode = 0;
  _nAxis = 3;
  _envelopeStatus = 0;

}

void Controller::runController(){

  if(_opMode == 1){

    _pid1.input = _encoder1->readPosition(0);

    if(isInPosition(&_pid1) == 0){
      runCalculation(&_pid1);
      moveAxis(_axis1, _pid1.output);
    }

    if(_nAxis >= 2) {

      _pid2.input = _encoder2->readPosition(0);

      if(isInPosition(&_pid2) == 0){
        runCalculation(&_pid2);
        moveAxis(_axis2, _pid2.output);
      }

    }

    if(_nAxis == 3) {

      _pid3.input = _encoder3->readPosition(0);

      if(isInPosition(&_pid3) == 0){
        runCalculation(&_pid3);
        moveAxis(_axis3, _pid3.output);
      }

    }

  }

}

void Controller::setPosition(Position position){

  if(_envelopeStatus == 1){

    if(position.Axis1 >= _envelope.Axis1_min && position.Axis1 <= _envelope.Axis1_max) _pid1.setpoint = position.Axis1;
    if(position.Axis2 >= _envelope.Axis2_min && position.Axis2 <= _envelope.Axis2_max) _pid2.setpoint = position.Axis2;
    if(position.Axis3 >= _envelope.Axis3_min && position.Axis3 <= _envelope.Axis3_max) _pid3.setpoint = position.Axis3;

  } else {

    _pid1.setpoint = position.Axis1;
    _pid2.setpoint = position.Axis2;
    _pid3.setpoint = position.Axis3;

  }

}

void Controller::setEnvelope(Envelope envelope){

  _envelope = envelope;
  _envelopeStatus = 1;

}

void Controller::setGains(int axis, double Kp, double Ki, double Kd){


  switch (axis) {

    case 1:

      _pid1.Kp = Kp;
      _pid1.Ki = Ki * (_pid1.period / 1000000);
      _pid1.Kd = Kd / (_pid1.period / 1000000);
      break;

    case 2:

      _pid2.Kp = Kp;
      _pid2.Ki = Ki * (_pid2.period / 1000000);
      _pid2.Kd = Kd / (_pid2.period / 1000000);
      break;

    case 3:

      _pid3.Kp = Kp;
      _pid3.Ki = Ki * (_pid3.period / 1000000);
      _pid3.Kd = Kd / (_pid3.period / 1000000);
      break;

  }

}

void Controller::setPIDPeriod(int axis, double period){

  switch (axis) {

    case 1:

      _pid1.period = period;
      break;

    case 2:

      _pid2.period = period;
      break;

    case 3:

      _pid3.period = period;
      break;

  }

}

void Controller::setController(int controller){

  if(controller >= 0 && controller <= 4){

    _pid1.controller = controller;
    _pid2.controller = controller;
    _pid3.controller = controller;

  }

}

void Controller::setController(int axis, int controller){

  if(controller >= 0 && controller <= 4){

    switch (axis) {

      case 1:

        _pid1.controller = controller;
        break;

      case 2:

        _pid2.controller = controller;
        break;

      case 3:

        _pid3.controller = controller;
        break;

    }

  }

}

void Controller::setOpMode(bool value){

  _opMode = value;

}

Position Controller::getPosition(){

  Position temp;

  temp.Axis1 = _pid1.input;
  temp.Axis2 = _pid2.input;
  temp.Axis3 = _pid3.input;

  return temp;

}

Position Controller::getSetpoint(){

  Position temp;

  temp.Axis1 = _pid1.setpoint;
  temp.Axis2 = _pid2.setpoint;
  temp.Axis3 = _pid3.setpoint;

  return temp;

}

bool Controller::isInPosition(int axis){

  bool temp;

  switch (axis) {

    case 1:

      temp = isInPosition(&_pid1);
      break;

    case 2:

      temp = isInPosition(&_pid2);
      break;

    case 3:

      temp = isInPosition(&_pid3);
      break;

  }

  return temp;

}

bool Controller::isInPosition(PID_setup *pid){

  double temp1 = pid->input;
  double temp2 = pid->setpoint - temp1;
  double temp3 = temp1 * pid->tolerance;

  if(abs(temp2) <= temp3) return 1;
  else return 0;

}

void Controller::moveAxis(Axis *axis, double output){

  if(axis->getOpMode() == 1){

    axis->setOutput(output);

    if(output > 0){

      axis->moveAxisFoward();

    } else if(output < 0) {

      axis->moveAxisBackward();

    } else {

      axis->looseAxis();

    }

  }

}

void Controller::runCalculation(PID_setup *pid){

  long currentTime = micros();

  if(currentTime - pid->lastIteration > pid->period){

    pid->dt = (currentTime - (double)pid->lastIteration)/ 1000000;

    pid->error = pid->setpoint - pid->input;
    pid->dError = pid->error - pid->lastError;

    if(pid->output > -1 && pid->output < 1) pid->integral = pid->integral + pid->error * pid->dt;

    switch(pid->controller){

      case 0: // Open loop

        break;

        case 1: // P

          pid->output = pid->Kp * pid->error;
          break;

        case 2: // PI

          pid->output = pid->Kp * pid->error + pid->Ki * pid->integral;
          break;

        case 3: // PD

          pid->output = pid->Kp * pid->error + pid->Kd * pid->dError;
          break;

        case 4: // PID

          pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->dError;
          break;

      }

      if(pid->output > 1) pid->output = 1;
      else if(pid->output < - 1) pid->output = -1;

      pid->lastError = pid->error;
      pid->lastIteration = currentTime;

    }

}

void Controller::inicializePID(PID_setup *pid){

  pid->Kp = 1;
  pid->Ki = 1;
  pid->Kd = 0.1;

  pid->input = 0;
  pid->output = 0;
  pid->setpoint = 0;
  pid->period = 100000;
  pid->tolerance = 0.05;

  pid->lastError = 0;
  pid->lastInput = 0;
  pid->integral = 0;
  pid->lastIteration = 0;

  pid->controller = 0;

}

void Controller::inicializeEnvelope(Envelope *envelope){

  envelope->Axis1_min = 0;
  envelope->Axis1_max = 0;
  envelope->Axis2_min = 0;
  envelope->Axis2_max = 0;
  envelope->Axis3_min = 0;
  envelope->Axis3_max = 0;

}
