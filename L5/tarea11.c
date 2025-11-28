// tarea11.c

#include <stdio.h>
#include <signal.h>

int main()

{
sigset_t new_mask;

/* inicializar la nueva mascara de señales */
sigemptyset(&new_mask);

sigaddset(&new_mask, SIGUSR1); // Añade SIGUSRI1  a new_mask

/*esperar a cualquier señal excepto SIGUSR1 */
sigsuspend(&new_mask); //Bloquea a SIGUSRI (La señal NO se entrega al proceso mientras está bloqueada.
                       // La señal queda pendiente (“en cola”) si llega.
                      // Cuando el proceso desbloquee esa señal, se entregará.)

}