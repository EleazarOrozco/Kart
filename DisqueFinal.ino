/*
   Robot Seguidor de Línea (+ lectura de sensor sonico)
   Este programa controla un robot que puede seguir líneas negras.
   Autor: [Dan Eleazar Orozco Tapia 5°I]
   Fecha: [06/10/2023]
*/

#include "Robot.h"  // Incluye el archivo de encabezado de la clase Robot

Robot robot;  // Crea un objeto de la clase Robot llamado "robot"

void setup() {
  robot.inicializar();  // Inicializa el robot llamando al método inicializar() de la clase Robot
}

void loop() {
  robot.ejecutar();  // Ejecuta las operaciones principales del robot llamando al método ejecutar() de la clase Robot
}
