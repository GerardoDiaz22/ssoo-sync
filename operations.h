/**
 * @file operations.h
 * @brief Cabecera para las operaciones concurrentes de lectura, escritura
 * y administración.
 */

#ifndef OPERATIONS_H
#define OPERATIONS_H

/**
 * @brief Simula una operación de lectura concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *read_operation(void *args);

/**
 * @brief Simula una operación de escritura concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *write_operation(void *args);

/**
 * @brief Simula una operación de administración concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *admin_operation(void *args);

#endif