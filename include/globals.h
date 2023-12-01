/**
 * @file globals.h
 * @brief Define variables globales y constantes para el proyecto de
 * concurrencia.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <semaphore.h>

// Declaración de mutexes y semáforos para sincronización de hilos
extern pthread_mutex_t mutex_read;
extern pthread_mutex_t mutex_readers_count;
extern pthread_mutex_t mutex_write;
extern pthread_mutex_t mutex_admin;
extern pthread_mutex_t mutex_readers_waiting;
extern pthread_mutex_t mutex_priority_writers;
extern pthread_cond_t cond_admin_wait;
extern sem_t concurrency_semaphore;

// Variables globales y contadores para control de acceso concurrente
extern int concurrency_count;
extern int max_recorded_concurrency;
extern int readers_count;
extern int admins_count;
extern int is_writer_waiting;

// Constantes de configuración
#define MAX_CONCURRENCY 100
#define MAX_THREADS 5000
#define UNIT_OF_TIME 10
#define DAYS 30
#define HOURS_IN_A_DAY 24

#endif