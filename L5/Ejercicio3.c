/*
 * espera_SIGUSR1.c
 * Suspende la ejecución hasta recibir SIGUSR1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Manejador para SIGUSR1 */
void manejador(int signo) {
    if (signo == SIGUSR1) {
        printf("¡Señal SIGUSR1 recibida! Saliendo.\n");
        /* Restaurar la acción por defecto (opcional) */
        signal(SIGUSR1, SIG_DFL);
    }
}

int main(void) {
    sigset_t mask_old, mask_new;

    /* Instalar manejador de SIGUSR1 */
    if (signal(SIGUSR1, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    /* Inicializar mask_new como conjunto vacío */
    if (sigemptyset(&mask_new) < 0) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }

    /* Agregar SIGUSR1 al conjunto de señales mask_new */
    if (sigaddset(&mask_new, SIGUSR1) < 0) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    /* Bloquear SIGUSR1, guardando la máscara anterior en mask_old */
    if (sigprocmask(SIG_BLOCK, &mask_new, &mask_old) < 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("Esperando la señal SIGUSR1...\n");

    /* Suspende la ejecución hasta que llegue una señal no bloqueada */
    /* Mientras tanto, SIGUSR1 sigue bloqueada, así que se despierta solo cuando se reciba SIGUSR1. */
    while (1) {
        sigsuspend(&mask_old);
        /* Cuando aquí retomamos, significa que llegó una señal manejable */
        /* Comprobamos: si fue SIGUSR1, salimos */
        /* En el manejador imprimimos y restauramos acción por defecto */
        break;
    }

    printf("Fin del programa.\n");
    return 0;
}

/* Cómo ejecutar:
gcc espera_SIGUSR1.c -o espera_SIGUSR1
./espera_SIGUSR1 &
ps -ax | grep espera_SIGUSR1
kill -SIGUSR1 <PID>
*/