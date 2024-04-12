// Evita la inclusión múltiple del archivo de encabezado
#ifndef ROBOT_H
#define ROBOT_H

// Incluye las bibliotecas necesarias
#include <Arduino.h>    // Biblioteca principal de Arduino
#include <Bounce2.h>    // Biblioteca para gestionar botones con rebote

// Declaración de la clase Robot
class Robot { //Define una clase llamada Robot para encapsular las funcionalidades relacionadas con el robot
public: //Indica los niveles de acceso de los miembros de la clase. 
        //Los métodos públicos son accesibles desde fuera de la clase, mientras que los miembros privados solo son accesibles internamente.
  Robot();  // Constructor

  // Métodos públicos
  void inicializar();   // Inicializa el robot
  void ejecutar();      // Ejecuta las operaciones del robot

private:
  // Enumerador para definir modos de funcionamiento del robot
  enum Modo { ELECCION, SONICO, SEGUIDOR }; //Utiliza un enumerador para representar los modos de funcionamiento del robot.

  Modo modo;                // Almacena el modo actual del robot
  Modo modoAnterior;        // Almacena el modo anterior del robot
  Bounce button;            // Objeto para gestionar el botón de entrada
  unsigned long lastButtonPress; // Almacena el tiempo de la última pulsación del botón
  int buttonPressCount;     // Contador de pulsaciones del botón

  // Métodos privados para realizar operaciones específicas
  void leerSensorSonico();        // Leer datos del sensor ultrasónico
  void seguirLinea();             // Seguir una línea
  void activarMotoresAdelante();  // Activar motores para avanzar
  void activarMotoresIzquierda(); // Activar motores para girar a la izquierda
  void activarMotoresDerecha();   // Activar motores para girar a la derecha
  void detenerMotores();          // Detener los motores
};

#endif // ROBOT_H
