#ifndef Motion_Control_h
#define Motion_Control_h
#define LIBRARY_VERSION	1.0.1

/* What's new?! - v1.0.1
 *
 * Consertado o algoritmo de implementação de um controlador PID discreto
 * Retirado os comentários nos construtures da classe Axis
 *
 */

#include "Arduino.h"


// Structs

struct Envelope {

  double Axis1_min, Axis1_max;
  double Axis2_min, Axis2_max;
  double Axis3_min, Axis3_max;

};

struct PID_setup {

  double Kp, Ki, Kd;
  double input, output, setpoint, error, tolerance;
  double lastInput, lastError, dError, dt, integral;
  long period, lastIteration;
  byte controller;

};

struct Position {

  double Axis1, Axis2, Axis3;

};

/* Classe Encoder
 *
 * Utilizada para realizar a leitura de encoders incrementais
 * Permite utilizar leituras de x1, x2 e x4
 * Necessária a utilização de interrupções
 *
*/

class Encoder {

public:

  Encoder(int pin_IN1, int pin_IN2, int pulsesPerRev, double pitchPerRev, byte mode);

  void setReference(double value);
  long readPulses();
  double readPosition(bool mode);
  double readSpeed(bool mode);

  void runInterrupt1();
  void runInterrupt2();
  void runInterrupt3();

private:

  static void ISREvent1();
  static void ISREvent2();
  static void ISREvent3();

  int _pinIN1, _pinIN2, _pulsesPerRev, _direction;
  double _pitchPerRev;
  long _lastEventTime, _currentEventTime, _currentPulses;
  byte _currentState, _lastState, _detectionMode;

  const int _fwdOrder1[2] = {-1, 1};
  const int _fwdOrder2[4] = {1, -1, -1, 1};
  const int _fwdOrder4[4] = {2, 0, 3, 1};

};



// Classe Axis

class Axis {

public:

  Axis(int pin_output, int pin_IN1, int pin_IN2);
  Axis(int pin_output, int pin_IN1, int pin_IN2, int pin_limitMin, int pin_limitMax);
  Axis(int pin_output, int control_channel, int pin_IN1, int pin_IN2);
  Axis(int pin_output, int control_channel, int pin_IN1, int pin_IN2, int pin_limitMin, int pin_limitMax);

  void runInterrupt();
  void setOpMode(int mode);
  int getOpMode();
  void setOutput(double value); // [0 to 100 %]
  void setMaxOutput(int value); // [0 to 4095]
  bool getHoming();
  void setDebug(bool value);
  void moveAxisFoward();
  void moveAxisBackward();
  void brakeAxis();
  void looseAxis();

private:

  static void ISREvent();

  void setError(int code);

  bool _debug;
  int _pinOutput, _controlChannel, _pinIN1, _pinIN2, _pinLimitMin, _pinLimitMax;
  int _opMode, _errorCode, _maxOutput, _output,  _homeStatus;

};


// Classe Controller

class Controller {

public:

  Controller(Axis *axis1, Encoder *encoder1); //ok
  Controller(Axis *axis1, Encoder *encoder1, Axis *axis2, Encoder *encoder2); //ok
  Controller(Axis *axis1, Encoder *encoder1, Axis *axis2, Encoder *encoder2, Axis *axis3, Encoder *encoder3); //ok

  void runController();

  void setPosition(Position position); //ok
  void setEnvelope(Envelope envelope); //ok
  void setGains(int axis, double Kp, double Ki, double Kd); //ok
  void setPIDPeriod(int axis, double period); //ok
  void setController(int controller); //ok
  void setController(int axis, int controller); //ok
  void setOpMode(bool value); //ok

  Position getPosition(); //ok
  Position getSetpoint(); //ok

  bool isInPosition(int axis); //ok
  void moveAxis(Axis *axis, double output); //ok

private:

  bool isInPosition(PID_setup *pid); //ok
  void runCalculation(PID_setup *pid); //ok
  void inicializePID(PID_setup *pid); //ok
  void inicializeEnvelope(Envelope *envelope);

  Axis *_axis1, *_axis2, *_axis3;
  Encoder *_encoder1, *_encoder2, *_encoder3;
  PID_setup _pid1, _pid2, _pid3;
  Envelope _envelope;

  int _nAxis;
  bool _opMode, _envelopeStatus;

};

#endif
