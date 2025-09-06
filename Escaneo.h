// Responsable: Angel David Morales Palomo
// Módulo: Este modulo sirve como logica para el escaneo de Puertos TCP en un S.O Windows
#ifndef ESCANEO_H
#define ESCANEO_H
#include <string>
struct Puerto {
    int numero;
    std::string estado;   
    bool sospechoso;      
};
bool inicializarRed(std::string& error);
void finalizarRed();
void escanearPuertos(const std::string& ip,
                     int inicio,
                     int fin,
                     Puerto* resultados,
                     int timeoutMs = 800);
#endif
