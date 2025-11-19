#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int n_hijos = 5;
    pid_t pid;

    // Crea 5 procesos hijo
    for (int i = 0; i < n_hijos; i++) {
        pid = fork();

        if (pid < 0) {   // Error con el fork
            perror("Error en fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // Proceso hijo
            printf("Soy el hijo con PID: %d\n", getpid());
            // Simular trabajo con sleep
            sleep(1 + i);  // No terminen todos a la vez
            exit(EXIT_SUCCESS);  // Que el hijo no siga al padre
        }

    }

    // Sólo llega aquí el padre (los hijos ya hicieron exit)
    int hijos_vivos = n_hijos;
    int status;
    pid_t pid_terminado;

    while ((pid_terminado = wait(&status)) > 0) {
        hijos_vivos--;
        printf("Acaba de finalizar mi hijo con PID: %d. Sólo me quedan %d hijos vivos.\n",
               pid_terminado, hijos_vivos);
    }

    return 0;
}
