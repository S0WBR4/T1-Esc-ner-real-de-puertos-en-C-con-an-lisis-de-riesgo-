// Responsable: Juan Angel Rodriguez Bulnes
// Módulo: Implementa la generación del reporte en disco.
// Aquí es donde realmente se arma el archivo de salida con los datos del escaneo.

#include "Registro.h"
#include "Analisis.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <unordered_set>

// Esta función auxiliar sirve para obtener la fecha y hora actual
// en un formato legible (ejemplo: 2025-09-06 16:00:00).
static std::string nowStr() {
    std::time_t t = std::time(nullptr);
    std::tm* tmv = std::localtime(&t);
    char buf[64]{};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmv);
    return std::string(buf);
}

// Aquí se define una pequeña lista de puertos considerados "de alto riesgo",
// por ejemplo FTP (21), SSH (22), Telnet (23), SMTP (25) y RDP (3389).
static bool esAltoRiesgo(int p) {
    static const std::unordered_set<int> R = {21,22,23,25,3389}; 
    return R.count(p) > 0;
}

// En esta lista se colocan los puertos "seguros" o comunes (whitelist),
// como HTTP (80), HTTPS (443) y DNS (53).
static bool enWhitelist(int p) {
    static const std::unordered_set<int> W = {80,443,53}; 
    return W.count(p) > 0;
}

// Esta función arma la justificación de por qué un puerto se marca como sospechoso,
// dependiendo del número de puerto y el nivel de sensibilidad elegido.
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

// Esta es la función principal del módulo.
// Se encarga de abrir un archivo y escribir en él:
// - Encabezado con la fecha, IP y nivel de sensibilidad.
// - Una tabla con todos los puertos escaneados y su estado.
// - Un resumen con cuántos puertos fueron sospechosos.
// - Una sección final con detalles y justificaciones de esos puertos.
void guardarResultados(const std::string& archivo,
                       const std::string& ip,
                       const Puerto* resultados, int cantidad,
                       int sensibilidad) {
    std::ofstream out(archivo);
    if (!out) {
        std::cerr << "No se pudo crear/abrir el archivo: " << archivo << "\n";
        return;
    }

    //Escribimos encabezado del reporte
    out << "Reporte de Escaneo de Puertos (TCP)\n";
    out << "Fecha/Hora: " << nowStr() << "\n";
    out << "Objetivo: " << ip << "\n";
    out << "Sensibilidad: " << sensibilidad << " (1=baja,2=media,3=alta)\n\n";
    out << std::left << std::setw(10) << "PUERTO"
        << std::setw(14) << "SERVICIO"
        << std::setw(12) << "ESTADO"
        << "SOSPECHOSO\n";
    out << "-----------------------------------------------\n";

    //Se recorre cada puerto escaneado y se imprime su info en la tabla
    int sospechosos = 0;
    for (int i = 0; i < cantidad; ++i) {
        const auto& r = resultados[i];
        if (r.sospechoso) ++sospechosos;
        out << std::left << std::setw(10) << r.numero
            << std::setw(14) << servicioDePuerto(r.numero)
            << std::setw(12) << r.estado
            << (r.sospechoso ? "SI" : "NO") << "\n";
    }

    //Resumen de la cantidad de sospechosos encontrados
    out << "\nTotal puertos: " << cantidad
        << " | Marcados sospechosos: " << sospechosos << "\n";
    
    //Sección con detalles de los sospechosos
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
