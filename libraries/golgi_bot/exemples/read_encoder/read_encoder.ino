#include <Motion_control.h>

/* Funções da classe:
 *  
 *  enc.readPulses() - retorna quantos pulsos foram contabilizados
 *  enc.readPosition() - retorna a posição atual calculada em mm (mode = 0) ou rotações (mode = 1)
 *  enc.readSpeed() - retorna a velocidade do eixo em mm / s (mode = 0) ou RPM (mode = 1)
 *  enc.setReference() - zera o valor de steps contabilizados
 *  
 */

// valores utilizados pelo objeto da classe Encoder

#define PIN1 2      // channel 1
#define PIN2 7      // channel 2
#define PULSES 600   // resolution in pulses per revolution
#define PITCH 40    // [mm]

/* Modos de operação do encoder
 *  
 *  1 x resolução: mode = 0 (1 interrupt - PIN1)
 *  2 x resolução: mode = 1 (1 interrupt - PIN1)
 *  4 x resolução: mode = 2 (2 interrupts - PIN1 and PIN2)
 *  
*/

#define MODE 0

// cria um novo objeto da classe Encoder

Encoder enc(PIN1, PIN2, PULSES, PITCH, MODE);

long current = 0, last1 = 0;

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {
  
  current = micros();

  // Faz um print da posição atual a cada 100ms

  if (current - last1 >= 100000) {
    Serial.println(enc.readPosition(0), 3);
    last1 = current;
  }

}
