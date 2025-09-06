// Responsable: Juan Angel Rodriguez Bulnes
// Módulo: Genera el reporte del escaneo en un archivo de texto.
// En este archivo solo se declara la función que se usará en el main.

#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include "Escaneo.h"

// Esta función guarda todos los resultados del escaneo en un archivo.
// Recibe la IP, la lista de puertos escaneados y el nivel de sensibilidad elegido.
void guardarResultados(const std::string& archivo,
                       const std::string& ip,
                       const Puerto* resultados, int cantidad,
                       int sensibilidad);
#endif
