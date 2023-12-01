/**
 * @file operations.c
 * @brief Implementación de las operaciones concurrentes de lectura, escritura
 * y administración.
 */

#include "operations.h"
#include "globals.h"
#include <stdio.h>

/**
 * @brief Simula una operación de lectura concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *read_operation(void *args)
{
    printf("R - Esperando espacio de concurrencia...\n");
    sem_wait(&concurrency_semaphore);
    printf("R - Espera terminada de espacio de concurrencia...\n");
    concurrency_count++;
    if (concurrency_count > max_recorded_concurrency)
        max_recorded_concurrency = concurrency_count;

    printf("R - Esperando prioridad de escritores...\n");
    // Busy wait
    while (is_writer_waiting == 1)
        ;
    printf("R - Espera terminada de prioridad de escritores...\n");

    printf("R - Esperando a administradores...\n");
    while (admins_count > 0)
        ;
    printf("R - Espera terminada de administradores...\n");

    printf("R - Anuncia entrada...\n");

    // Bloquear SC para lectores
    pthread_mutex_lock(&mutex_readers_count);
    // Si es el primer lector, bloquear escritura
    if (readers_count == 0)
    {
        // Bloquear escritura
        printf("R - Esperando a escritores...\n");
        pthread_mutex_lock(&mutex_write);
        printf("R - Espera terminada de escritores...\n");
    }
    // Aumentar cantidad de lectores
    readers_count++;
    // Desbloquear SC para lectores
    pthread_mutex_unlock(&mutex_readers_count);

    printf("R - Iniciando SC...\n");

    // SC
    printf("\033[1;32mLeyendo...\033[0m\n");
    for (int time = 0; time < UNIT_OF_TIME; time++)
    {
    }

    printf("R - Finalizando SC...\n");

    // Bloquear SC para lectores
    pthread_mutex_lock(&mutex_readers_count);
    // Disminuir cantidad de lectores
    readers_count--;
    // Si es el ultimo lector, desbloquear escritura
    if (readers_count == 0)
    {
        // Desbloquear escritura
        pthread_mutex_unlock(&mutex_write);
    }
    // Desbloquear SC para lectores
    pthread_mutex_unlock(&mutex_readers_count);

    sem_post(&concurrency_semaphore);
    concurrency_count--;

    printf("R - Anuncia salida...\n");

    return NULL;
}

/**
 * @brief Simula una operación de escritura concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *write_operation(void *args)
{
    printf("W - Esperando espacio de concurrencia...\n");
    sem_wait(&concurrency_semaphore);
    printf("W - Espera terminada de espacio de concurrencia...\n");
    concurrency_count++;
    if (concurrency_count > max_recorded_concurrency)
        max_recorded_concurrency = concurrency_count;

    printf("W - Anuncia entrada...\n");
    is_writer_waiting = 1;

    // Bloquear escritura
    printf("W - Esperando a lectores y escritores...\n");
    pthread_mutex_lock(&mutex_write);
    printf("W - Espera terminada de lectores y escritores...\n");

    printf("W - Esperando a administradores...\n");
    // Busy wait
    while (admins_count > 0)
        ;
    printf("W - Espera terminada de administradores...\n");

    printf("W- Iniciando SC...\n");

    // SC
    printf("\033[1;32mEscribiendo...\033[0m\n");
    for (int time = 0; time < (UNIT_OF_TIME * 3); time++)
    {
    }

    printf("W - Finalizando SC...\n");

    // Desbloquear escritura
    pthread_mutex_unlock(&mutex_write);

    sem_post(&concurrency_semaphore);
    concurrency_count--;

    printf("W - Anuncia salida...\n");
    is_writer_waiting = 0;

    return NULL;
}

/**
 * @brief Simula una operación de administración concurrente.
 * @param args Argumentos para la función (no utilizados).
 * @return void* Siempre retorna NULL.
 */
void *admin_operation(void *args)
{
    printf("A - Esperando espacio de concurrencia...\n");
    sem_wait(&concurrency_semaphore);
    printf("A - Espera terminada de espacio de concurrencia...\n");
    concurrency_count++;
    if (concurrency_count > max_recorded_concurrency)
        max_recorded_concurrency = concurrency_count;

    printf("A - Anuncia entrada...\n");

    // Aumentar cantidad de administradores
    admins_count++;

    printf("A - Iniciando SC...\n");

    // SC
    printf("\033[1;32mAdministrando...\033[0m\n");
    for (int time = 0; time < (UNIT_OF_TIME * 2); time++)
    {
    }

    printf("A - Finalizando SC...\n");

    // Disminuir cantidad de administradores
    admins_count--;

    sem_post(&concurrency_semaphore);
    concurrency_count--;

    printf("A - Anuncia salida...\n");

    return NULL;
}