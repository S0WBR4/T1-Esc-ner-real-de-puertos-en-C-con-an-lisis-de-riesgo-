# Proyecto: Escáner de Puertos en C++

## Contenido
- [Descripción General](#descripción-general)
- [Integrantes y Responsabilidades](#integrantes-y-responsabilidades)
- [Módulos](#módulos)
  - [Escaneo (Angel David Morales Palomo)](#escaneo-angel-david-morales-palomo)
  - [Registro (Juan Angel Rodriguez Bulnes)](#registro-juan-angel-rodriguez-bulnes)
  - [Análisis (Sofia Flores Martinez Cisneros)](#análisis-sofia-flores-martinez-cisneros)
- [Cómo usar el módulo de Escaneo](#cómo-usar-el-módulo-de-escaneo)
- [Cómo usar el módulo de Registro](#cómo-usar-el-módulo-de-registro)
- [Cómo usar el módulo de Análisis](#cómo-usar-el-módulo-de-análisis)

## Descripción General
Este proyecto consiste en el desarrollo de un programa en C++ que realiza un escaneo real de puertos sobre una dirección IP específica.
El sistema identifica puertos abiertos, cerrados o filtrados, y clasifica como sospechosos aquellos que cumplan con criterios definidos por el equipo.

El objetivo principal es aplicar conocimientos de:
- Programación modular
- Uso de estructuras de datos y memoria dinámica
- Principios básicos de ciberseguridad

## Integrantes y Responsabilidades
- Angel David Morales Palomo — Módulo de Escaneo (`Escaneo.cpp` y `Escaneo.h`)
- Juan Angel Rodriguez Bulnes — Módulo de Registro (`Registro.cpp` y `Registro.h`)
- Sofia Flores Martinez Cisneros — Módulo de Análisis (`Analisis.cpp` y `Analisis.h`)

## Módulos

### Escaneo (Angel David Morales Palomo)
*(Aquí Angel David agregará su descripción del módulo)*

### Registro (Juan Angel Rodriguez Bulnes)
Mi responsabilidad en el proyecto fue implementar el módulo de **Registro**, encargado de generar el reporte final del escaneo en un archivo de texto.

Funciones principales del módulo:
- Guarda la fecha y hora del escaneo.
- Registra la IP objetivo y el nivel de sensibilidad utilizado.
- Genera una tabla con todos los puertos escaneados, indicando si son sospechosos.
- Calcula el total de puertos revisados y proporciona un resumen con justificaciones para los puertos sospechosos.

Archivos relacionados:
- `Registro.cpp` — Implementación del módulo
- `Registro.h` — Declaración de funciones públicas para generar el reporte

### Análisis (Sofia Flores Martinez Cisneros)
*(Aquí Sofia agregará su descripción del módulo)*

## Cómo usar el módulo de Escaneo
## Cómo usar el módulo de Registro
El módulo de **Registro** se utiliza dentro del programa principal después de realizar el escaneo de puertos y el análisis de sospechosos.

Pasos para usarlo:
1. Llamar a la función `guardarResultados()` pasando los siguientes parámetros:
   - Nombre del archivo de salida (ej. `"reporte.txt"`)
   - IP objetivo que fue escaneada
   - Lista de puertos con su estado y marca de sospecha
   - Cantidad total de puertos escaneados
   - Nivel de sensibilidad elegido (1 = baja, 2 = media, 3 = alta)
2. La función genera el archivo de reporte con:
   - Tabla de puertos y su estado
   - Resumen de puertos sospechosos
   - Justificación de cada puerto marcado como sospechoso

No requiere configuración adicional; solo asegurarse de que se llame después de tener los resultados del escaneo.

## Cómo usar el módulo de Análisis
