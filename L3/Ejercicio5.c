#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int n_hijos = 5;
    pid_t pid, hijos[5];
    int i;

    // Crear los 5 procesos hijo
    for (i = 0; i < n_hijos; i++) {
        pid = fork();

        if (pid < 0) {   // Error
            perror("Error en fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // Proceso hijo
            printf("Soy el hijo con PID: %d\n", getpid());
            // Simulamos trabajo para que no todos terminen a la vez
            sleep(1 + i);
            exit(EXIT_SUCCESS);  // importante: que el hijo no siga ejecutando al padre
        }

        // Solo el padre llega aquí: guardamos el PID del hijo creado
        hijos[i] = pid;
    }

    // Solo el padre llega aquí (los hijos ya hicieron exit)
    int hijos_vivos = n_hijos;
    int status;
    pid_t pid_terminado;

    // Primero esperar a los hijos creados en orden impar: 1º, 3º, 5º
    // Eso corresponde a los índices 0, 2, 4 del array 'hijos'
    for (i = 0; i < n_hijos; i += 2) {
        pid_terminado = waitpid(hijos[i], &status, 0);
        if (pid_terminado > 0) {
            hijos_vivos--;
            printf("Acaba de finalizar mi hijo con PID: %d. Sólo me quedan %d hijos vivos.\n",
                   pid_terminado, hijos_vivos);
        }
    }

    // Después esperar a los hijos creados en orden par: 2º y 4º
    // Eso corresponde a los índices 1 y 3 del array 'hijos'
    for (i = 1; i < n_hijos; i += 2) {
        pid_terminado = waitpid(hijos[i], &status, 0);
        if (pid_terminado > 0) {
            hijos_vivos--;
            printf("Acaba de finalizar mi hijo con PID: %d. Sólo me quedan %d hijos vivos.\n",
                   pid_terminado, hijos_vivos);
        }
    }

    return 0;
}
