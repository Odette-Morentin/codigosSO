// ejecutor.c
// Ejecuta un programa con sus argumentos y opcionalmente en background si se especifica "bg"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2) { //Error si no recibe suficientes argumentos
        fprintf(stderr, "Uso: %s <programa> [args...] [bg]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int background = 0;

    // Comprobar si el último argumento es "bg"
    if (strcmp(argv[argc - 1], "bg") == 0) {
        background = 1;
        argv[argc - 1] = NULL;   // Quitamos "bg" de los argumentos
        argc--;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Proceso hijo
        execvp(argv[1], &argv[1]);  
        // Si execvp falla:
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Proceso padre
    if (!background) {
        // Ejecutar en foreground → esperamos al hijo
        int status;
        waitpid(pid, &status, 0);
        printf("Proceso %d finalizó con estado %d\n", pid, status);
    } else {
        // Ejecutar en background → NO esperamos
        printf("Proceso ejecutándose en background con PID %d\n", pid);
    }

    return 0;
}
