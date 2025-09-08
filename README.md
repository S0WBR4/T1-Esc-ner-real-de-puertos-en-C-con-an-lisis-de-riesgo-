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
Mi parte para este proyecto fue el implementar los mudulos de Escaneo, el cual es encargado de realizar la verificación en tiempo real de los puertos TCP de cualquier dirección IP de la red como objetivo mediante un entorno a base de Windows.

Funciones Principales:
Lo primero que hace este modulo es inicializar y finalizar el uso de la librería de Winsock que sirve para habilitar las conexiones a la red, después de eso valida la dirección IP que es ingresada por el usuario y pregunta ya sea por el rango de puertos de su elección o ciertos rangos específicos separados por una coma. Tambien establece conexión TCP en cada uno de los puertos para clasificar el estado de este, por ejemplo "Abierto", "Cerrado", "Filtrado" y por ultimo, Registra los resultados en un formato que incluye el numero del puerto analizado, el estado en el que se encuentra y la marca de si es sospechoso o no. 
Los puertos que son seleccionados como sospechosos son aquellos que se supone que no deben estar abiertos normalmente, que luego esto se analiza en otro modulo.  

Archivos relacionados con el modulo:
-`Escaneo.cpp` (Este archivo tiene la lógica de conexión y el escaneo de puertos)
-`Escaneo.h` (Aquí esta básicamente la estructura y las funciones publicas del puerto.)

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
El modulo de escaneo se utiliza dentro del programa principal para obtener el estado real de los puertos TCP, el como funciona es mediante la función "inicializarRed()" antes de comenzar el escaneo para poder preparar la librería Winsock.

Después llama a la función "escanearPuertos()" después de haber pasado por los parámetros:
*Dirección IP
*Puerto Inicial y Final ó selección de puertos
*Tiempo de espera (Que por predeterminado le puse que solo esperara 800ms por cada puerto que escane como máximo)

Después de esto, este modulo revisa los resultados en la estructura de "Puerto" donde la función "finalizarRed()" es llamada para liberar los recursos de la misma red y en automático valida la dirección IP ingresada, verifica cada puerto que fue seleccionado o cada puerto del rango especificado y checa la clasificación del estado de cada puerto.

Es necesario decir que realmente el modulo no requiere una configuración adicional, solo es de asegurare de inicializar la red antes de escanear y cerrarla al final del proceso.
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
