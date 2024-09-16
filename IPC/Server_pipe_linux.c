#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define PIPE_NUMERIC "/tmp/numbers_pipe"
#define PIPE_STRING "/tmp/strings_pipe"
#define THREAD_NUM 4

void* workerThread(void* args) {
    char buffer[256];
    int pipe_num_fd = open(PIPE_NUMERIC, O_RDONLY);
    int pipe_str_fd = open(PIPE_STRING, O_RDONLY);

    if (pipe_num_fd < 0) {
        perror("Erro ao abrir o pipe numérico");
        exit(1);
    }
    if (pipe_str_fd < 0) {
        perror("Erro ao abrir o pipe de strings");
        exit(1);
    }

    while (1) {
        // Ler do pipe numérico
        if (read(pipe_num_fd, buffer, sizeof(buffer)) > 0) {
            printf("Servidor recebeu do cliente numérico: %s\n", buffer);
        }

        // Ler do pipe de strings
        if (read(pipe_str_fd, buffer, sizeof(buffer)) > 0) {
            printf("Servidor recebeu do cliente de strings: %s\n", buffer);
        }
    }

    close(pipe_num_fd);
    close(pipe_str_fd);
    return NULL;
}

int main() {
    // Criar pipes nomeados
    if (mkfifo(PIPE_NUMERIC, 0666) == -1) {
        perror("Erro ao criar o pipe numérico");
        exit(1);
    }
    if (mkfifo(PIPE_STRING, 0666) == -1) {
        perror("Erro ao criar o pipe de strings");
        exit(1);
    }

    pthread_t th;
    if (pthread_create(&th, NULL, &workerThread, NULL) != 0) {
        perror("Falha ao criar thread");
        exit(1);
    }

    // Espera que a thread do servidor rode indefinidamente
    pthread_join(th, NULL);

    // Remover os pipes ao finalizar
    unlink(PIPE_NUMERIC);
    unlink(PIPE_STRING);

    return 0;
}
