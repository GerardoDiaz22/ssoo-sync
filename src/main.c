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
#include "../include/globals.h"
#include "../include/operations.h"

// Constantes de configuración
#define DAYS 30
#define HOURS_IN_A_DAY 24

// Declaración de mutexes y semáforos para sincronización de hilos
pthread_mutex_t mutex_read;
pthread_mutex_t mutex_readers_count;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_admin;
pthread_mutex_t mutex_admins_count;
pthread_mutex_t mutex_readers_waiting;
pthread_mutex_t mutex_priority_writers;
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
    pthread_mutex_init(&mutex_admins_count, NULL);
    pthread_mutex_init(&mutex_readers_waiting, NULL);
    pthread_mutex_init(&mutex_priority_writers, NULL);
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
        printf("3 - Análisis Detallado del Día (Operaciones por Hora y Horas Pico)\n");
        printf("4 - Pruebas de Borde y Casos Especiales\n");
        printf("5 - Salir\n");
        printf("Introduzca su elección: ");
        scanf("%d", &choice);
        printf("\n");

        // Ejecución del caso de uso seleccionado
        switch (choice)
        {
        case 1:
            pthread_t a_t1_read, a_t2_read, a_t3_read, a_t1_write, a_t2_write, a_t1_admin, a_t2_admin;

            // Caso 1
            pthread_create(&a_t1_read, NULL, read_operation, NULL);
            pthread_create(&a_t1_write, NULL, write_operation, NULL);
            sleep(1);
            pthread_create(&a_t2_read, NULL, read_operation, NULL);
            pthread_create(&a_t3_read, NULL, read_operation, NULL);

            pthread_join(a_t1_read, NULL);
            pthread_join(a_t1_write, NULL);
            pthread_join(a_t2_read, NULL);
            pthread_join(a_t3_read, NULL);
            break;

        case 2:
            pthread_t b_t1_read, b_t2_read, b_t3_read, b_t1_write, b_t2_write, b_t1_admin, b_t2_admin;

            // Caso 2
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
            break;

        case 3:
            int read_operations = 0;
            int write_operations = 0;
            int admin_operations = 0;

            int read_operations_hour_array[HOURS_IN_A_DAY] = {0};
            int write_operations_hour_array[HOURS_IN_A_DAY] = {0};
            int admin_operations_hour_array[HOURS_IN_A_DAY] = {0};

            for (int i = 0; i < DAYS; i++)
            {
                // Generate a random number to decide the dominant operation for the day
                int dominant_op = rand() % 3; // 0 for read, 1 for write, 2 for admin

                int read_operations_per_hour = 0;
                int write_operations_per_hour = 0;
                int admin_operations_per_hour = 0;

                for (int j = 0; j < HOURS_IN_A_DAY; j++)
                {
                    pthread_t op_thread;
                    int read_flag = 0;
                    int write_flag = 0;
                    int admin_flag = 0;

                    // Dice roll for read operation
                    int read_dice = rand() % 9 + 1;
                    if (dominant_op == 0)
                        read_dice += 3; // Bonus for dominant operation

                    if (read_dice >= 6) // Threshold for operation to occur
                    {
                        read_operations_per_hour++;
                        read_operations_hour_array[j] += 1;
                        pthread_create(&op_thread, NULL, read_operation, NULL);
                        read_flag = 1;
                    }

                    // Dice roll for write operation
                    int write_dice = rand() % 9 + 1;
                    if (dominant_op == 1)
                        write_dice += 3; // Bonus for dominant operation

                    if (write_dice >= 6) // Threshold for operation to occur
                    {
                        write_operations_per_hour++;
                        write_operations_hour_array[j] += 1;
                        pthread_create(&op_thread, NULL, write_operation, NULL);
                        write_flag = 1;
                    }

                    // Dice roll for admin operation
                    int admin_dice = rand() % 9 + 1;
                    if (dominant_op == 2)
                        admin_dice += 3; // Bonus for dominant operation

                    if (admin_dice >= 6) // Threshold for operation to occur
                    {
                        admin_operations_per_hour++;
                        admin_operations_hour_array[j] += 1;
                        pthread_create(&op_thread, NULL, admin_operation, NULL);
                        admin_flag = 1;
                    }

                    if (read_flag == 1)
                    {
                        pthread_join(op_thread, NULL);
                    }

                    if (write_flag == 1)
                    {
                        pthread_join(op_thread, NULL);
                    }
                    if (admin_flag == 1)
                    {
                        pthread_join(op_thread, NULL);
                    }
                }
                read_operations += read_operations_per_hour;
                write_operations += write_operations_per_hour;
                admin_operations += admin_operations_per_hour;

                printf("Operación dominante: %d\n", dominant_op);
            }

            printf("Operaciones de lectura: %d\n", read_operations);
            printf("Operaciones de escritura: %d\n", write_operations);
            printf("Operaciones de administración: %d\n", admin_operations);

            printf("Operaciones de lectura por hora: %f\n", (float)read_operations / (DAYS * HOURS_IN_A_DAY));
            printf("Operaciones de escritura por hora: %f\n", (float)write_operations / (DAYS * HOURS_IN_A_DAY));
            printf("Operaciones de administración por hora: %f\n", (float)admin_operations / (DAYS * HOURS_IN_A_DAY));

            // Encontrar hora con más operaciones
            int max_read_operations_hour = 0;
            int max_write_operations_hour = 0;
            int max_admin_operations_hour = 0;

            int max_read_operations_hour_index = 0;
            int max_write_operations_hour_index = 0;
            int max_admin_operations_hour_index = 0;

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

            printf("Hora con más operaciones de lectura: %d\n", max_read_operations_hour_index);
            printf("Hora con más operaciones de escritura: %d\n", max_write_operations_hour_index);
            printf("Hora con más operaciones de administración: %d\n", max_admin_operations_hour_index);
            printf("Máxima concurrencia registrada: %d\n", max_recorded_concurrency);

            break;
        case 4:
            // Pruebas de Borde: ejecuta escenarios de prueba para casos especiales y de borde
            /* ... lógica para las Pruebas de Borde ... */
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
    pthread_mutex_destroy(&mutex_admins_count);
    pthread_mutex_destroy(&mutex_readers_waiting);
    pthread_mutex_destroy(&mutex_priority_writers);
    sem_destroy(&concurrency_semaphore);

    return 0;
}