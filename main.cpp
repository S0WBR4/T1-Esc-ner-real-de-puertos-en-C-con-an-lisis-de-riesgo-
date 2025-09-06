#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Escaneo.h"
#include "Analisis.h"
#include "Registro.h"
static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    return s.substr(a, b - a + 1);
}
static std::vector<int> parsePortsCSV(const std::string& csv) {
    std::vector<int> ports;
    std::stringstream ss(csv);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        tok = trim(tok);
        if (tok.empty()) continue;
        try {
            int p = std::stoi(tok);
            if (p >= 1 && p <= 65535) ports.push_back(p);
        } catch (...) {
            // ignora tokens inválidos
        }
    }
    return ports;
}
int main() {
    std::string err;
    if (!inicializarRed(err)) {
        std::cerr << "Error Winsock: " << err << "\n";
        return 1;
    }
    std::string ip;
    std::cout << "IP objetivo: ";
    std::getline(std::cin, ip);
    ip = trim(ip);
    if (ip.empty()) { std::cerr << "IP vacia.\n"; finalizarRed(); return 1; }
    std::cout << "Modo de puertos:\n"
                 "  1) Rango (inicio/fin)\n"
                 "  2) Lista (ej. 22,53,80,443)\n"
                 "Elige 1 o 2: ";
    std::string modoStr;
    std::getline(std::cin, modoStr);
    int modo = (trim(modoStr) == "2") ? 2 : 1;
    std::vector<int> listaPuertos;
    int inicio = 0, fin = -1;
    if (modo == 1) {
        std::string tmp;
        std::cout << "Puerto inicial: ";
        std::getline(std::cin, tmp);
        try { inicio = std::stoi(trim(tmp)); } catch (...) { inicio = 0; }
        std::cout << "Puerto final: ";
        std::getline(std::cin, tmp);
        try { fin = std::stoi(trim(tmp)); } catch (...) { fin = -1; }
        if (inicio < 1 || fin > 65535 || inicio > fin) {
            std::cerr << "Rango invalido.\n"; finalizarRed(); return 1;
        }
    } else {
        std::string csv;
        std::cout << "Lista de puertos (separados por coma): ";
        std::getline(std::cin, csv);
        listaPuertos = parsePortsCSV(csv);
        if (listaPuertos.empty()) {
            std::cerr << "No se proporcionaron puertos validos.\n";
            finalizarRed(); return 1;
        }
    }
    std::string archivoSalida;
    std::cout << "Nombre del archivo de salida (ej. resultados.txt): ";
    std::getline(std::cin, archivoSalida);
    archivoSalida = trim(archivoSalida);
    if (archivoSalida.empty()) archivoSalida = "resultados.txt";
    std::cout << "Nivel de sensibilidad (1=baja, 2=media, 3=alta) [2]: ";
    std::string sensStr;
    std::getline(std::cin, sensStr);
    int sensibilidad = 2;
    if (!trim(sensStr).empty()) {
        try { sensibilidad = std::stoi(trim(sensStr)); } catch (...) {}
    }
    if (sensibilidad < 1) sensibilidad = 1;
    if (sensibilidad > 3) sensibilidad = 3;
    std::vector<Puerto> resultados;
    try {
        if (modo == 1) {
            int n = fin - inicio + 1;
            resultados.resize(n);
            escanearPuertos(ip, inicio, fin, resultados.data(), 800);
        } else {
            resultados.resize(static_cast<size_t>(listaPuertos.size()));
            for (size_t i = 0; i < listaPuertos.size(); ++i) {
                escanearPuertos(ip, listaPuertos[i], listaPuertos[i], &resultados[i], 800);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Excepcion durante escaneo: " << e.what() << "\n";
        finalizarRed();
        return 1;
    }
    marcarSospechosos(resultados.data(), static_cast<int>(resultados.size()), sensibilidad);
    std::cout << "\n=== RESULTADOS ===\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        const auto& r = resultados[i];
        std::cout << "Puerto " << r.numero
                  << " (" << servicioDePuerto(r.numero) << ") -> "
                  << r.estado
                  << (r.sospechoso ? " [Sospechoso]" : "")
                  << "\n";
    }
    guardarResultados(archivoSalida, ip, resultados.data(),
                      static_cast<int>(resultados.size()), sensibilidad);
    std::cout << "\nGuardado en: " << archivoSalida << "\n";
    finalizarRed();
    return 0;
}
