#include "Robot.h"  // Incluye el archivo de encabezado de la clase Robot

// Definición de pines utilizados en el robot
#define sensorIzquierdo A1
#define sensorCentroDerecho A3
#define sensorCentroIzquierda A2
#define sensorDerecho A4
#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define UMBRAL 600
#define motorAIN1 5
#define motorAIN2 6
#define motorBIN1 9
#define motorBIN2 10
#define BUTTON_PIN 1

// Constructor de la clase Robot
Robot::Robot()
  : modo(ELECCION), lastButtonPress(0), buttonPressCount(0) {}
  // El constructor inicializa los valores de los miembros de datos modo, lastButtonPress y buttonPressCount a ELECCION, 0 y 0, respectivamente

// Inicializa el robot
void Robot::inicializar() {
  Serial.begin(9600);  // Inicializa la comunicación serial

  // Configuración de pines de entrada/salida
  pinMode(sensorIzquierdo, INPUT);
  pinMode(sensorCentroDerecho, INPUT);
  pinMode(sensorCentroIzquierda, INPUT);
  pinMode(sensorDerecho, INPUT);

  pinMode(motorAIN1, OUTPUT);
  pinMode(motorAIN2, OUTPUT);
  pinMode(motorBIN1, OUTPUT);
  pinMode(motorBIN2, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configuración del botón con debounce
  button.attach(BUTTON_PIN);
  button.interval(25);

  Serial.println("Modo: Elección");  // Imprime el estado inicial
  modoAnterior = modo;
}

// Ejecuta las operaciones del robot
void Robot::ejecutar() {
  button.update();  // Actualiza el estado del botón

  // Control de pulsaciones del botón
  if (button.fell()) { //Se utiliza para detectar el momento en que un botón se presiona
    unsigned long now = millis();
    if (now - lastButtonPress < 500) {
      buttonPressCount++;
    } else {
      buttonPressCount = 1;
    }
    lastButtonPress = now;
    }

    // Cambio de modo según las pulsaciones del botón
    switch (buttonPressCount) {
    case 1:
      modo = SEGUIDOR;
      break;
    case 2:
      modo = SONICO;
      break;
    case 3:
      modo = ELECCION;
      break;
    }

    // Imprime el modo si cambia
    if (modo != modoAnterior) {
    switch (modo) {
      case ELECCION:
        Serial.println("Modo: Elección");
        break;
      case SONICO:
        Serial.println("Modo: Sónico");
        break;
      case SEGUIDOR:
        Serial.println("Modo: Seguidor");
        break;
    }
    modoAnterior = modo;
    }

   // Ejecución de acciones según el modo
   switch (modo) {
    case ELECCION:
      detenerMotores();
      break;
    case SONICO:
      leerSensorSonico();
      break;
    case SEGUIDOR:
      seguirLinea();
      break;
  }
}

// Lee datos del sensor ultrasónico
void Robot::leerSensorSonico() {
  // Envía un pulso ultrasónico
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Mide la duración del eco
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Manejo de situaciones excepcionales
  if (duration == 0) {
    Serial.println("El sensor está reseteando");
    digitalWrite(ECHO_PIN, LOW);
    delay(50);
    return;
  }

  // Calcula la distancia en centímetros
  long distance = duration * 0.0343 / 2;

  // Imprime la distancia
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Toma decisiones basadas en la distancia
  if (distance <= 20) {
    while (distance < 40) {
      activarMotoresDerecha();
      Serial.println("Girando a la derecha");

      // Repite la medición de distancia
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);

      duration = pulseIn(ECHO_PIN, HIGH);

      // Manejo de situaciones excepcionales
      if (duration == 0) {
        Serial.println("El sensor está reseteando");
        digitalWrite(ECHO_PIN, LOW);
        delay(50);
        return;
      }

      // Calcula la distancia en centímetros
      distance = duration * 0.0343 / 2;

      // Imprime la distancia
      Serial.print("Distancia: ");
      Serial.print(distance);
      Serial.println(" cm");
    }

    // Detiene los motores
    detenerMotores();
    Serial.println("Detenido");
  } else {
    // Activa los motores para avanzar
    activarMotoresAdelante();
    Serial.println("Avanzando recto");
  }
}

// Realiza el seguimiento de una línea
void Robot::seguirLinea() {
  // Lee los valores de los sensores de línea
  int izquierda = analogRead(sensorIzquierdo);
  int centroDerecha = analogRead(sensorCentroDerecho);
  int centroIzquierda = analogRead(sensorCentroIzquierda);
  int derecha = analogRead(sensorDerecho);

  // Toma decisiones basadas en los valores de los sensores
  if (centroIzquierda < UMBRAL && centroDerecha < UMBRAL) {
    activarMotoresAdelante();
    Serial.println("Avanzando recto");
    delay(1000);
    } else if (derecha < UMBRAL) {
    activarMotoresIzquierda();
    Serial.println("Girando a la izquierda");
    } else if (izquierda < UMBRAL) {
    activarMotoresDerecha();
    Serial.println("Girando a la derecha");
    } else {
    detenerMotores();
    Serial.println("Detenido");
  }
}

// Activa los motores para avanzar
void Robot::activarMotoresAdelante() {
  digitalWrite(motorAIN1, HIGH);
  digitalWrite(motorAIN2, LOW);
  digitalWrite(motorBIN1, HIGH);
  digitalWrite(motorBIN2, LOW);
}

// Activa los motores para girar a la izquierda
void Robot::activarMotoresIzquierda() {
  digitalWrite(motorAIN1, LOW);
  digitalWrite(motorAIN2, HIGH);
  digitalWrite(motorBIN1, HIGH);
  digitalWrite(motorBIN2, LOW);
}

// Activa los motores para girar a la derecha
void Robot::activarMotoresDerecha() {
  digitalWrite(motorAIN1, HIGH);
  digitalWrite(motorAIN2, LOW);
  digitalWrite(motorBIN1, LOW);
  digitalWrite(motorBIN2, HIGH);
}

// Detiene los motores
void Robot::detenerMotores() {
  digitalWrite(motorAIN1, LOW);
  digitalWrite(motorAIN2, LOW);
  digitalWrite(motorBIN1, LOW);
  digitalWrite(motorBIN2, LOW);
}
