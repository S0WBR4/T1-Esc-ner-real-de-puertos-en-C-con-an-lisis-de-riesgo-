// Responsable: Juan Angel Rodriguez Bulnes
// Módulo: Implementa la generación del reporte en disco.
#include "Registro.h"
#include "Analisis.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <unordered_set>
static std::string nowStr() {
    std::time_t t = std::time(nullptr);
    std::tm* tmv = std::localtime(&t);
    char buf[64]{};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmv);
    return std::string(buf);
}
static bool esAltoRiesgo(int p) {
    static const std::unordered_set<int> R = {21,22,23,25,3389}; 
    return R.count(p) > 0;
}
static bool enWhitelist(int p) {
    static const std::unordered_set<int> W = {80,443,53}; 
    return W.count(p) > 0;
}
static std::string justificacion(int puerto, int sensibilidad) {
    const std::string svc = servicioDePuerto(puerto);
    if (esAltoRiesgo(puerto)) {
        return "Puerto de alto riesgo conocido (" + svc + ")";
    }
    if (sensibilidad == 2 && puerto > 1024 && !enWhitelist(puerto)) {
        return "Abierto no estandar (>1024) y fuera de la lista blanca";
    }
    if (sensibilidad == 3 && !enWhitelist(puerto)) {
        return "Sensibilidad alta: abierto y fuera de la lista blanca";
    }
    return "Marcado segun criterios del equipo";
}
void guardarResultados(const std::string& archivo,
                       const std::string& ip,
                       const Puerto* resultados, int cantidad,
                       int sensibilidad) {
    std::ofstream out(archivo);
    if (!out) {
        std::cerr << "No se pudo crear/abrir el archivo: " << archivo << "\n";
        return;
    }
    out << "Reporte de Escaneo de Puertos (TCP)\n";
    out << "Fecha/Hora: " << nowStr() << "\n";
    out << "Objetivo: " << ip << "\n";
    out << "Sensibilidad: " << sensibilidad << " (1=baja,2=media,3=alta)\n\n";
    out << std::left << std::setw(10) << "PUERTO"
        << std::setw(14) << "SERVICIO"
        << std::setw(12) << "ESTADO"
        << "SOSPECHOSO\n";
    out << "-----------------------------------------------\n";
    int sospechosos = 0;
    for (int i = 0; i < cantidad; ++i) {
        const auto& r = resultados[i];
        if (r.sospechoso) ++sospechosos;
        out << std::left << std::setw(10) << r.numero
            << std::setw(14) << servicioDePuerto(r.numero)
            << std::setw(12) << r.estado
            << (r.sospechoso ? "SI" : "NO") << "\n";
    }
    out << "\nTotal puertos: " << cantidad
        << " | Marcados sospechosos: " << sospechosos << "\n";

    out << "\n--- Puertos sospechosos (detalles) ---\n";
    bool alguno = false;
    for (int i = 0; i < cantidad; ++i) {
        const auto& r = resultados[i];
        if (!r.sospechoso) continue;
        alguno = true;
        out << "- " << r.numero << " (" << servicioDePuerto(r.numero) << "): "
            << justificacion(r.numero, sensibilidad) << "\n";
    }
    if (!alguno) {
        out << "Ninguno\n";
    }
}
