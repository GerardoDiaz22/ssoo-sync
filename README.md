# Proyecto de Simulación de Concurrencia - Problema 1

## Descripción

Este proyecto simula un sistema de operaciones de lectura, escritura y administración, utilizando hilos en C para gestionar la concurrencia y el acceso a recursos compartidos como contadores. La simulación esta basada en el problema dado que es una variación del problema de lectores y escritores, donde se agregan operaciones de administración. Estas operaciones no son bloqueadas por las operaciones de lectura o escritura, pero tienen la capacidad de bloquear tanto a los lectores como a los escritores.

## Características

- Implementa hilos con la biblioteca pthreads de C.
- Usa semáforos y mutexes para controlar el acceso concurrente.
- Simula diferentes escenarios de operaciones concurrentes.

## Requisitos

- Compilador GCC
- Soporte para pthreads

## Compilación y Ejecución

Utilice el archivo `makefile` incluido para compilar:

```
$ make
```

Para ejecutar el programa:

```
$ ./run.out
```

## Estructura del Proyecto

- `main.c`: Punto de entrada del programa, contiene la lógica principal.
- `globals.h`: Define globales y constantes usadas en el proyecto.
- `operations.h`: Cabecera para las funciones de operación.
- `operations.c`: Implementación de las operaciones de lectura, escritura y administración.
