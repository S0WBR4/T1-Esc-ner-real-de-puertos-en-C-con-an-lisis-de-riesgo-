// Responsable: Sofía Martínez Cisneros
// Módulo: Este modulo sirve para marcar puertos sospechosos y mapear algun servicio conocido
#ifndef ANALISIS_H
#define ANALISIS_H
#include <string>
#include "Escaneo.h"
std::string servicioDePuerto(int puerto);
void marcarSospechosos(Puerto* resultados, int cantidad, int sensibilidad = 2);
#endif
