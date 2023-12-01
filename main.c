#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
// pthread_cancel

pthread_mutex_t mutex_read;
pthread_mutex_t mutex_readers_count;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_admin;
pthread_mutex_t mutex_admins_count;

sem_t concurrency_semaphore;
int concurrency_count = 0;
int max_recorded_concurrency = 0;

int readers_count = 0;
int admins_count = 0;
int is_writer_waiting = 0;

#define DAYS 30
#define HOURS_IN_A_DAY 24
#define MAX_CONCURRENCY 30
#define UNIT_OF_TIME 15

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

int main()
{
    srand(time(NULL));

    pthread_mutex_init(&mutex_read, NULL);
    pthread_mutex_init(&mutex_readers_count, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    pthread_mutex_init(&mutex_admin, NULL);
    pthread_mutex_init(&mutex_admins_count, NULL);

    sem_init(&concurrency_semaphore, 0, MAX_CONCURRENCY);

    // Entrada de usuario para seleccionar caso de uso
    int choice;
    printf("Seleccione un caso de uso:\n");
    printf("1 - Caso 1\n");
    printf("2 - Caso 2\n");
    printf("3 - Caso 3\n");
    printf("Introduzca su elección: ");
    scanf("%d", &choice);

    // Ejecuta el caso de uso seleccionado
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

        break;
    default:
        printf("Opción no válida.\n");
    }

    printf("Máxima concurrencia registrada: %d\n", max_recorded_concurrency);

    pthread_mutex_destroy(&mutex_read);
    pthread_mutex_destroy(&mutex_readers_count);
    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_admin);
    pthread_mutex_destroy(&mutex_admins_count);

    sem_destroy(&concurrency_semaphore);

    return 0;
}