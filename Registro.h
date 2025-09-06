// Responsable: Juan Angel Rodriguez Bulnes
// Módulo: Genera el reporte del escaneo.
#ifndef REGISTRO_H
#define REGISTRO_H
#include <string>
#include "Escaneo.h"
void guardarResultados(const std::string& archivo,
                       const std::string& ip,
                       const Puerto* resultados, int cantidad,
                       int sensibilidad);
#endif
