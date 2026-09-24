#include "DigiKeyboard.h"

void setup() {
  // No necesitamos configurar pines para esto, solo el teclado.
  // Inicializamos el generador de números aleatorios usando una lectura "flotante"
  // del pin 2 para obtener una semilla más variada.
  randomSeed(analogRead(2));
  
  // Pequeña pausa para dar tiempo al sistema a reconocer el dispositivo USB
  DigiKeyboard.delay(5000); 
}

void loop() {
  // 1. Generar un número aleatorio entre 0 y 99
  int numeroRuleta = random(0, 100);

  // 2. Escribir el resultado en la pantalla
  DigiKeyboard.print("Resultado: ");
  DigiKeyboard.println(numeroRuleta);

  // 3. Esperar 5 segundos antes de generar otro número.
  // Esto evita que sature la pantalla y te da tiempo a ver el resultado.
  DigiKeyboard.delay(5000);
  
  // El bucle se repite infinitamente.
}