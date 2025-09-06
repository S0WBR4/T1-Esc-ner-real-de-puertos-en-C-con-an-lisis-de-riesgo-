// Responsable: Sofía Martínez Cisneros
// Criterios para marcar puertos sospechosos y mapear servicios
#include "Analisis.h"
#include <unordered_map>
#include <unordered_set>
static const std::unordered_map<int, std::string> kServicios = {
    {21,  "FTP"},
    {22,  "SSH"},
    {23,  "TELNET"},
    {25,  "SMTP"},
    {53,  "DNS"},    
    {80,  "HTTP"},
    {110, "POP3"},
    {143, "IMAP"},
    {443, "HTTPS"},
    {3389,"RDP"}  
};
std::string servicioDePuerto(int puerto) {
    auto it = kServicios.find(puerto);
    return (it != kServicios.end()) ? it->second : "desconocido";
}
static const std::unordered_set<int> kAltoRiesgo = {
    21,22,23,25,3389
};
static const std::unordered_set<int> kListaBlancaMin = {
    80,443,53 
};
void marcarSospechosos(Puerto* resultados, int cantidad, int sensibilidad) {
    if (!resultados || cantidad <= 0) return;
    if (sensibilidad < 1) sensibilidad = 1;
    if (sensibilidad > 3) sensibilidad = 3;
    for (int i = 0; i < cantidad; ++i) {
        auto& p = resultados[i];
        p.sospechoso = false;
        if (p.estado != "Abierto") continue;
        const bool esAltoRiesgo = (kAltoRiesgo.count(p.numero) > 0);
        const bool estaEnListaBlanca = (kListaBlancaMin.count(p.numero) > 0);
        const bool esNoEstandar = (p.numero > 1024 && !estaEnListaBlanca);
        switch (sensibilidad) {
            case 1:
                p.sospechoso = esAltoRiesgo;
                break;
            case 2:
                p.sospechoso = esAltoRiesgo || esNoEstandar;
                break;
            case 3:
                p.sospechoso = !estaEnListaBlanca;
                break;
        }
    }
}
