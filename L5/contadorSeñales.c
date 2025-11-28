/*
 * contador.c
 * Muestra cuántas veces se recibe cada señal manejable.
 * Informa de las señales que NO pueden manejarse.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Número máximo de señales definidas en Linux */
#define MAX_SIG 32

/* Array que llevará la cuenta de cuántas veces se recibe cada señal */
int contador[MAX_SIG] = {0};

/* Manejador de señales genérico */
void manejador(int sig)
{
    if (sig >= 0 && sig < MAX_SIG)
    {
        contador[sig]++;
        printf("La señal %d se ha recibido %d veces\n", sig, contador[sig]);
    }
}

int main()
{
    struct sigaction act;
    int i;

    /* Preparar la estructura sigaction */
    act.sa_handler = manejador;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    /* Mensaje inicial */
    printf("No puedo manejar la señal 9\n");   /* SIGKILL */
    printf("No puedo manejar la señal 19\n");  /* SIGSTOP */
    printf("Esperando el envío de señales...\n");

    /* Instalar manejadores para todas las señales posibles */
    for (i = 1; i < MAX_SIG; i++)
    {
        if (i == SIGKILL || i == SIGSTOP)
            continue;  // No se pueden capturar

        sigaction(i, &act, NULL);
    }

    /* Bucle infinito para mantener el proceso vivo */
    while (1)
    {
        pause();  // Espera una señal sin consumir CPU
    }

    return 0;
}
