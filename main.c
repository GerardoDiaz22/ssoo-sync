#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
// pthread_cancel

pthread_mutex_t mutex_read;
pthread_mutex_t mutex_readers_count;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_admin;
pthread_mutex_t mutex_admins_count;

int readers_count = 0;
int admins_count = 0;
int is_writer_waiting = 0;

void *read_operation(void *args)
{
    printf("R - Esperando prioridad de escritores...\n");
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
    printf("Leyendo...\n");
    sleep(1);

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

    printf("R - Anuncia salida...\n");

    return NULL;
}

void *write_operation(void *args)
{
    printf("W - Anuncia entrada...\n");
    is_writer_waiting = 1;

    // Bloquear escritura
    printf("W - Esperando a lectores y escritores...\n");
    pthread_mutex_lock(&mutex_write);
    printf("W - Espera terminada de lectores y escritores...\n");

    printf("W - Esperando a administradores...\n");
    while (admins_count > 0)
        ;
    printf("W - Espera terminada de administradores...\n");

    printf("W- Iniciando SC...\n");

    // SC
    printf("Escribiendo...\n");
    sleep(3);

    printf("W - Finalizando SC...\n");

    // Desbloquear escritura
    pthread_mutex_unlock(&mutex_write);

    printf("W - Anuncia salida...\n");
    is_writer_waiting = 0;

    return NULL;
}

void *admin_operation(void *args)
{
    printf("A - Anuncia entrada...\n");

    // Aumentar cantidad de administradores
    admins_count++;

    printf("A - Iniciando SC...\n");

    // SC
    printf("Administrando...\n");
    sleep(1);

    printf("A - Finalizando SC...\n");

    // Disminuir cantidad de administradores
    admins_count--;

    printf("A - Anuncia salida...\n");

    return NULL;
}

int main()
{
    pthread_t t1_read, t2_read, t3_read, t1_write, t2_write, t1_admin, t2_admin;

    // Inicializa el mutex en 1
    pthread_mutex_init(&mutex_read, NULL);
    pthread_mutex_init(&mutex_readers_count, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    pthread_mutex_init(&mutex_admin, NULL);
    pthread_mutex_init(&mutex_admins_count, NULL);

    pthread_create(&t1_read, NULL, read_operation, NULL);
    pthread_create(&t2_read, NULL, read_operation, NULL);
    pthread_create(&t3_read, NULL, read_operation, NULL);
    pthread_create(&t1_write, NULL, write_operation, NULL);
    pthread_create(&t2_write, NULL, write_operation, NULL);
    pthread_create(&t1_admin, NULL, admin_operation, NULL);
    pthread_create(&t2_admin, NULL, admin_operation, NULL);

    // Caso 1
    /*pthread_join(t1_read, NULL);
    pthread_join(t1_write, NULL);
    sleep(1);
    pthread_join(t2_read, NULL);
    pthread_join(t3_read, NULL);*/

    // Caso 2
    /*
    pthread_join(t1_read, NULL);
    pthread_join(t1_write, NULL);
    pthread_join(t2_write, NULL);
    pthread_join(t1_admin, NULL);
    pthread_join(t2_admin, NULL);
    pthread_join(t2_read, NULL);
    pthread_join(t3_read, NULL);*/

    pthread_mutex_destroy(&mutex_read);
    pthread_mutex_destroy(&mutex_readers_count);
    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_admin);
    pthread_mutex_destroy(&mutex_admins_count);

    return 0;
}
