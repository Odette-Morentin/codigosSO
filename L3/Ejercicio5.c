#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t hijos[5];

    /* Crear hijos */
    for (int i = 0; i < 5; i++) {
        hijos[i] = fork();

        if (hijos[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (hijos[i] == 0) {
            printf("Soy el hijo %d\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

    /* Esperar primero a los hijos con PID impar */
    for (int i = 0; i < 5; i++) {
        if (hijos[i] % 2 != 0) {
            waitpid(hijos[i], NULL, 0);
            printf("Finalizó hijo IMPAR %d\n", hijos[i]);
        }
    }

    /* Esperar después a los hijos con PID par */
    for (int i = 0; i < N; i++) {
        if (hijos[i] % 2 == 0) {
            waitpid(hijos[i], NULL, 0);
            printf("Finalizó hijo PAR %d\n", hijos[i]);
        }
    }

    return 0;
}
