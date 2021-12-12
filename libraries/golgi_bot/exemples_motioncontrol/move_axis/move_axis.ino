#include <Motion_control.h>

/* Funções da classe Axis:
 *  
 *  moveAxisFoward(): eixo se move para frente
 *  moveAxisBackward(): eixo se move para trás
 *  brakeAxis(): eixo resistente ao movimento forçado
 *  looseAxis(): eixo solto
 *  setOpMode(int mode): 0 = desativado, 1 = ativado, 2 = setting, 3 = homing
 *  int getOpMode(): lê qual é o modo de operação atual
 *  setOutput(double value): define o duty cycle do PWM (de 0 a 1)
 *  setMaxOutput(int value): define qual é o valor máximo para o PWM (de 0 a 4095);
 *  bool getHoming(): lê se o eixo está referenciado
 *  setDebug(int bool): 0 = desativado, 1 = imprime erros na serial
 *  resetError(): 
 *  
 */

#define OUTPUT_PIN 21 // IO vque envia o sinal PWM
#define IN1_PIN 19 // IO 1 de controle do sentido
#define IN2_PIN 18 // IO 2 de controle do sentido

Axis axis(OUTPUT_PIN, IN1_PIN, IN2_PIN);

void setup() {
  
  axis.setOpMode(1); // op mode = 1: eixo em operação normal

}

void loop() {

  // Eixo se move para frente por 2000 ms
 
  axis.moveAxisFoward();
  delay(2000);

  // Eixo é deixado solto para perder velocidade
  
  axis.looseAxis();
  delay(500);

  // Motor do eixo é curto-circuitado para gerar resistência ao movimento

  axis.brakeAxis();
  delay(2000);

  // Eixo se move para trás por 2000 ms

  axis.moveAxisFoward();
  delay(2000);

  // Eixo é deixado solto para perder velocidade
  
  axis.looseAxis();
  delay(500);

  // Motor do eixo é curto-circuitado para gerar resistência ao movimento

  axis.brakeAxis();
  delay(2000);

}
