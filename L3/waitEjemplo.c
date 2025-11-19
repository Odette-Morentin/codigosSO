#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, sleep, getpid, getppid
#include <sys/types.h>
#include <sys/wait.h>   // wait, waitpid

int main(void) {
    pid_t hijo1, hijo2;
    int status;

    // 1. Creamos el primer hijo
    hijo1 = fork();

    if (hijo1 < 0) {
        perror("Error al crear hijo1");
        exit(EXIT_FAILURE);
    }

    if (hijo1 == 0) {
        // === CÓDIGO DEL HIJO 1 ===
        printf("[HIJO 1] Soy el hijo 1. PID=%d, PPID=%d\n", getpid(), getppid());
        sleep(3);   // Simulamos trabajo
        printf("[HIJO 1] Termino con exit(10)\n");
        exit(10);   // Código de salida 10
    }

    // 2. Creamos el segundo hijo
    hijo2 = fork();

    if (hijo2 < 0) {
        perror("Error al crear hijo2");
        exit(EXIT_FAILURE);
    }

    if (hijo2 == 0) {
        // === CÓDIGO DEL HIJO 2 ===
        printf("[HIJO 2] Soy el hijo 2. PID=%d, PPID=%d\n", getpid(), getppid());
        sleep(1);   // Termina antes que el hijo 1
        printf("[HIJO 2] Termino con exit(20)\n");
        exit(20);   // Código de salida 20
    }

    // === CÓDIGO DEL PADRE ===
    printf("[PADRE] Soy el padre. PID=%d. He creado a hijo1=%d y hijo2=%d\n",
           getpid(), hijo1, hijo2);

    // 3. El padre espera PRIMERO, específicamente, al hijo1 con waitpid
    pid_t terminado = waitpid(hijo1, &status, 0);  // 0 = modo bloqueante

    if (terminado == -1) {
        perror("Error en waitpid");
    } else {
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("[PADRE] Ha terminado hijo1 (PID=%d) con exit(%d)\n",
                   terminado, code);
        }
    }

    // 4. El padre espera ahora a CUALQUIER otro hijo que quede con wait
    terminado = wait(&status);   // ahora solo queda hijo2

    if (terminado == -1) {
        perror("Error en wait");
    } else {
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("[PADRE] Ha terminado otro hijo (PID=%d) con exit(%d)\n",
                   terminado, code);
        }
    }

    printf("[PADRE] Ya no me quedan hijos. Termino.\n");
    return 0;
}
