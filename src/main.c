/**
 * @file main.c
 * @brief Entrada principal para el proyecto de concurrencia.
 */

// Inclusión de bibliotecas estándar y archivos de cabecera personalizados
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../include/globals.h"
#include "../include/operations.h"

// Declaración de mutexes y semáforos para sincronización de hilos
pthread_mutex_t mutex_read;
pthread_mutex_t mutex_readers_count;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_admin;
pthread_mutex_t mutex_readers_waiting;
pthread_mutex_t mutex_priority_writers;
pthread_cond_t cond_admin_wait;
sem_t concurrency_semaphore;

// Variables globales y contadores para control de acceso concurrente
int concurrency_count = 0;
int max_recorded_concurrency = 0;
int readers_count = 0;
int admins_count = 0;
int is_writer_waiting = 0;

/**
 * @brief Función principal que inicia la simulación.
 * @return int Retorna 0 para indicar una ejecución exitosa.
 */
int main()
{
    // Inicialización de semillas para generación de números aleatorios
    srand(time(NULL));

    // Inicialización de mutexes y semáforos
    pthread_mutex_init(&mutex_read, NULL);
    pthread_mutex_init(&mutex_readers_count, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    pthread_mutex_init(&mutex_admin, NULL);
    pthread_mutex_init(&mutex_readers_waiting, NULL);
    pthread_mutex_init(&mutex_priority_writers, NULL);
    pthread_cond_init(&cond_admin_wait, NULL);
    sem_init(&concurrency_semaphore, 0, MAX_CONCURRENCY);

    // Menú interactivo para selección de casos de uso
    int choice;
    do
    {
        printf("=================================================\n");
        printf(" MENÚ PRINCIPAL - SIMULACIÓN DE BASE DE DATOS\n");
        printf("=================================================\n");
        printf("Seleccione una opción:\n");
        printf("1 - Simulación Básica (Lecturas y Escrituras Concurrentes)\n");
        printf("2 - Simulación Avanzada (Incluye Administración)\n");
        printf("3 - Análisis Detallado por Día (Operaciones por Hora y Horas Pico)\n");
        printf("5 - Salir\n");
        printf("Introduzca su elección: ");
        scanf("%d", &choice);
        printf("\n");

        // Ejecución del caso de uso seleccionado
        switch (choice)
        {
        // Caso 1
        case 1:

            pthread_t a_t1_read, a_t2_read, a_t3_read, a_t1_write;

            // max_recorded_concurrency = 0;

            pthread_create(&a_t1_read, NULL, read_operation, NULL);
            pthread_create(&a_t1_write, NULL, write_operation, NULL);
            pthread_create(&a_t2_read, NULL, read_operation, NULL);
            pthread_create(&a_t3_read, NULL, read_operation, NULL);

            pthread_join(a_t1_read, NULL);
            pthread_join(a_t1_write, NULL);
            pthread_join(a_t2_read, NULL);
            pthread_join(a_t3_read, NULL);

            printf("Operaciones de lectura: 3\n");
            printf("Operaciones de escritura: 1\n");
            printf("Operaciones de administración: 0\n");
            printf("Máxima concurrencia registrada: %d\n", max_recorded_concurrency);

            break;

        // Caso 2
        case 2:

            pthread_t b_t1_read, b_t2_read, b_t3_read, b_t1_write, b_t2_write, b_t1_admin, b_t2_admin;

            // max_recorded_concurrency = 0;

            pthread_create(&b_t1_read, NULL, read_operation, NULL);
            pthread_create(&b_t1_write, NULL, write_operation, NULL);
            pthread_create(&b_t2_write, NULL, write_operation, NULL);
            pthread_create(&b_t1_admin, NULL, admin_operation, NULL);
            pthread_create(&b_t2_admin, NULL, admin_operation, NULL);
            pthread_create(&b_t2_read, NULL, read_operation, NULL);
            pthread_create(&b_t3_read, NULL, read_operation, NULL);

            pthread_join(b_t1_read, NULL);
            pthread_join(b_t1_write, NULL);
            pthread_join(b_t2_write, NULL);
            pthread_join(b_t1_admin, NULL);
            pthread_join(b_t2_admin, NULL);
            pthread_join(b_t2_read, NULL);
            pthread_join(b_t3_read, NULL);

            printf("Operaciones de lectura: 3\n");
            printf("Operaciones de escritura: 2\n");
            printf("Operaciones de administración: 2\n");
            printf("Máxima concurrencia registrada: %d\n", max_recorded_concurrency);

            break;

        // Caso 3
        case 3:
            max_recorded_concurrency = 0;

            int read_operations = 0;
            int write_operations = 0;
            int admin_operations = 0;

            int read_operations_hour_array[HOURS_IN_A_DAY] = {0};
            int write_operations_hour_array[HOURS_IN_A_DAY] = {0};
            int admin_operations_hour_array[HOURS_IN_A_DAY] = {0};

            // Simulación de N días
            for (int i = 0; i < DAYS; i++)
            {
                // Generar un número aleatorio para determinar la operación dominante
                // 0 para lectura, 1 para escritura, 2 para administración
                int dominant_op = rand() % 3;

                int read_operations_per_hour = 0;
                int write_operations_per_hour = 0;
                int admin_operations_per_hour = 0;

                pthread_t read_threads[MAX_THREADS] = {0};
                pthread_t write_threads[MAX_THREADS] = {0};
                pthread_t admin_threads[MAX_THREADS] = {0};

                // Simulación de M horas
                for (int j = 0; j < HOURS_IN_A_DAY; j++)
                {

                    // Lanzar un dado 1d10 para determinar si se realiza una operación de lectura
                    int read_dice = rand() % 9 + 1;

                    // Si la operación es dominante, se suma un bono de 3 al dado
                    if (dominant_op == 0)
                        read_dice += 3;

                    // Si el dado es mayor o igual a 6, se realiza la operación
                    if (read_dice >= 6)
                    {
                        // Sumar al dado un segundo dado 1d5 para determinar cuántas operaciones de lectura se realizan
                        read_dice += rand() % 3 + 1;
                        for (int k = 0; k < read_dice; k++)
                        {
                            read_operations_per_hour++;
                            read_operations_hour_array[j] += 1;
                            pthread_create(&read_threads[read_operations_per_hour], NULL, read_operation, NULL);
                        }
                    }

                    // Lanzar un dado 1d10 para determinar si se realiza una operación de escritura
                    int write_dice = rand() % 9 + 1;

                    // Si la operación es dominante, se suma un bono de 3 al dado
                    if (dominant_op == 1)
                        write_dice += 3;

                    // Si el dado es mayor o igual a 6, se realiza la operación
                    if (write_dice >= 6)
                    {
                        // Sumar al dado un segundo dado 1d5 para determinar cuántas operaciones de escritura se realizan
                        write_dice += rand() % 3 + 1;
                        for (int k = 0; k < write_dice; k++)
                        {
                            write_operations_per_hour++;
                            write_operations_hour_array[j] += 1;
                            pthread_create(&write_threads[write_operations_per_hour], NULL, write_operation, NULL);
                        }
                    }

                    // Lanzar un dado 1d10 para determinar si se realiza una operación de administración
                    int admin_dice = rand() % 9 + 1;

                    // Si la operación es dominante, se suma un bono de 3 al dado
                    if (dominant_op == 2)
                        admin_dice += 3;

                    // Si el dado es mayor o igual a 6, se realiza la operación
                    if (admin_dice >= 6)
                    {
                        // Sumar al dado un segundo dado 1d5 para determinar cuántas operaciones de administración se realizan
                        admin_dice += rand() % 3 + 1;
                        for (int k = 0; k < admin_dice; k++)
                        {
                            admin_operations_per_hour++;
                            admin_operations_hour_array[j] += 1;
                            pthread_create(&admin_threads[admin_operations_per_hour], NULL, admin_operation, NULL);
                        }
                    }
                }

                read_operations += read_operations_per_hour;
                write_operations += write_operations_per_hour;
                admin_operations += admin_operations_per_hour;

                // Esperar a que terminen todos los hilos del día
                for (int k = 0; k < read_operations_per_hour; k++)
                {
                    pthread_join(read_threads[k], NULL);
                }

                for (int k = 0; k < write_operations_per_hour; k++)
                {
                    pthread_join(write_threads[k], NULL);
                }

                for (int k = 0; k < admin_operations_per_hour; k++)
                {
                    pthread_join(admin_threads[k], NULL);
                }
            }

            int max_read_operations_hour = 0;
            int max_write_operations_hour = 0;
            int max_admin_operations_hour = 0;

            int max_read_operations_hour_index = 0;
            int max_write_operations_hour_index = 0;
            int max_admin_operations_hour_index = 0;

            // Encontrar hora con más operaciones por tipo
            for (int i = 0; i < HOURS_IN_A_DAY; i++)
            {
                if (read_operations_hour_array[i] > max_read_operations_hour)
                {
                    max_read_operations_hour = read_operations_hour_array[i];
                    max_read_operations_hour_index = i;
                }

                if (write_operations_hour_array[i] > max_write_operations_hour)
                {
                    max_write_operations_hour = write_operations_hour_array[i];
                    max_write_operations_hour_index = i;
                }

                if (admin_operations_hour_array[i] > max_admin_operations_hour)
                {
                    max_admin_operations_hour = admin_operations_hour_array[i];
                    max_admin_operations_hour_index = i;
                }
            }

            printf("Operaciones de lectura: %d\n", read_operations);
            printf("Operaciones de escritura: %d\n", write_operations);
            printf("Operaciones de administración: %d\n", admin_operations);

            printf("Operaciones de lectura por hora: %.2f\n", roundf((float)read_operations / (DAYS * HOURS_IN_A_DAY) * 100) / 100);
            printf("Operaciones de escritura por hora: %.2f\n", roundf((float)write_operations / (DAYS * HOURS_IN_A_DAY) * 100) / 100);
            printf("Operaciones de administración por hora: %.2f\n", roundf((float)admin_operations / (DAYS * HOURS_IN_A_DAY) * 100) / 100);

            printf("Hora con más operaciones de lectura: %d\n", max_read_operations_hour_index);
            printf("Hora con más operaciones de escritura: %d\n", max_write_operations_hour_index);
            printf("Hora con más operaciones de administración: %d\n", max_admin_operations_hour_index);
            printf("Máxima concurrencia registrada: %d\n", max_recorded_concurrency);

            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opción no válida. Por favor, introduzca un número entre 1 y 5.\n");
        }
    } while (choice != 5);

    // Limpieza y finalización de hilos
    pthread_mutex_destroy(&mutex_read);
    pthread_mutex_destroy(&mutex_readers_count);
    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_admin);
    pthread_mutex_destroy(&mutex_readers_waiting);
    pthread_mutex_destroy(&mutex_priority_writers);
    pthread_cond_destroy(&cond_admin_wait);
    sem_destroy(&concurrency_semaphore);

    return 0;
}