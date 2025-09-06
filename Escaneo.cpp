// Responsable: Angel David Morales Palomo
// Escaneo real de puertos TCP 
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WIN32_WINNT 0x0601
#include "Escaneo.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
static bool g_wsa_ok = false;
bool inicializarRed(std::string& error) {
    WSADATA wsaData;
    int r = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (r != 0) {
        error = "WSAStartup fallo: " + std::to_string(r);
        g_wsa_ok = false;
        return false;
    }
    g_wsa_ok = true;
    return true;
}
void finalizarRed() {
    if (g_wsa_ok) {
        WSACleanup();
        g_wsa_ok = false;
    }
}
static bool ipValidaIPv4(const std::string& ip) {
    unsigned long ipbin = inet_addr(ip.c_str());
    return ipbin != INADDR_NONE;
}
static std::string probarPuertoTCP(const std::string& ip, int puerto, int timeoutMs) {
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        return "Filtrado";
    }
    u_long nonblock = 1;
    ioctlsocket(s, FIONBIO, &nonblock);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(static_cast<u_short>(puerto));
    unsigned long ipbin = inet_addr(ip.c_str());
    if (ipbin == INADDR_NONE) { closesocket(s); return "Filtrado"; }
    addr.sin_addr.s_addr = ipbin;
    int r = connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (r == 0) {
        closesocket(s);
        return "Abierto";
    } else {
        int wsaErr = WSAGetLastError();
        if (wsaErr != WSAEWOULDBLOCK &&
            wsaErr != WSAEINPROGRESS &&
            wsaErr != WSAEINVAL) {
            closesocket(s);
            return "Cerrado";
        }
    }
    fd_set writeSet, errSet;
    FD_ZERO(&writeSet);
    FD_ZERO(&errSet);
    FD_SET(s, &writeSet);
    FD_SET(s, &errSet);
    timeval tv{};
    tv.tv_sec  = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    int sel = select(0, nullptr, &writeSet, &errSet, &tv);
    if (sel == 0) {
        closesocket(s);
        return "Filtrado";
    }
    if (sel == SOCKET_ERROR) {
        closesocket(s);
        return "Filtrado";
    }
    if (FD_ISSET(s, &errSet)) {
        closesocket(s);
        return "Cerrado";
    }
    if (FD_ISSET(s, &writeSet)) {
        closesocket(s);
        return "Abierto";
    }
    closesocket(s);
    return "Filtrado";
}
//API
void escanearPuertos(const std::string& ip, int inicio, int fin, Puerto* resultados, int timeoutMs) {
    if (!g_wsa_ok) {
        throw std::runtime_error("Winsock no inicializado. Llama inicializarRed() antes de escanear.");
    }
    if (!ipValidaIPv4(ip)) {
        throw std::invalid_argument("IP invalida (se espera IPv4).");
    }
    if (inicio < 1 || fin > 65535 || inicio > fin) {
        throw std::invalid_argument("Rango de puertos invalido.");
    }
    if (!resultados) {
        throw std::invalid_argument("Puntero de resultados nulo.");
    }
    for (int p = inicio; p <= fin; ++p) {
        Puerto& slot = resultados[p - inicio];
        slot.numero = p;
        slot.sospechoso = false;
        slot.estado = probarPuertoTCP(ip, p, timeoutMs);
    }
}
