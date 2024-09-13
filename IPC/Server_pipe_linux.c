#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define PIPE_NUMERIC "/tmp/numbers_pipe"
#define PIPE_STRING "/tmp/strings_pipe"
#define REQUESTS_NUM 5 // Número de requisições por cliente
#define CLIENT_THREADS 3 // Número de clientes simultâneos

void* send_numeric_requests(void* args) {
    char buffer[256];
    int pipe_fd = open(PIPE_NUMERIC, O_WRONLY);
    if (pipe_fd < 0) {
        perror("Erro ao abrir o pipe numérico");
        return NULL;
    }

    for (int i = 0; i < REQUESTS_NUM; i++) {
        sprintf(buffer, "%d %d", rand() % 100, rand() % 100);  // Enviar dois números aleatórios
        write(pipe_fd, buffer, strlen(buffer) + 1);
        printf("Cliente numérico enviou: %s\n", buffer);
        sleep(1);
    }

    close(pipe_fd);
    return NULL;
}

void* send_string_requests(void* args) {
    char buffer[256];
    int pipe_fd = open(PIPE_STRING, O_WRONLY);
    if (pipe_fd < 0) {
        perror("Erro ao abrir o pipe de strings");
        return NULL;
    }

    for (int i = 0; i < REQUESTS_NUM; i++) {
        sprintf(buffer, "Mensagem %d", rand() % 100);  // Enviar uma string aleatória
        write(pipe_fd, buffer, strlen(buffer) + 1);
        printf("Cliente string enviou: %s\n", buffer);
        sleep(1);
    }

    close(pipe_fd);
    return NULL;
}

int main() {
    pthread_t client_threads[CLIENT_THREADS * 2];  // *2 para ter threads numéricas e de strings

    // Criando clientes simultâneos (metade para números, metade para strings)
    for (int i = 0; i < CLIENT_THREADS; i++) {
        pthread_create(&client_threads[i], NULL, &send_numeric_requests, NULL);
        pthread_create(&client_threads[i + CLIENT_THREADS], NULL, &send_string_requests, NULL);
    }

    // Espera que todas as threads de cliente terminem
    for (int i = 0; i < CLIENT_THREADS * 2; i++) {
        pthread_join(client_threads[i], NULL);
    }

    return 0;
}
