// tarea12.c

#include <signal.h>   // Para señales POSIX: sigaction, sigprocmask, sigset_t...
#include <stdio.h>    // printf(), perror()
#include <string.h>   // memset()
#include <unistd.h>   // sleep(), etc.

// Variable global para indicar si se recibió SIGTERM
static int signal_recibida = 0;

// Manejador de señal: se ejecuta cuando llega SIGTERM
static void manejador (int sig)
{
    signal_recibida = 1;   // Marcamos que SIGTERM ha sido recibida
}

int main (int argc, char *argv[])
{
    sigset_t conjunto_mascaras;         // Máscara temporal donde bloqueamos SIGTERM
    sigset_t conj_mascaras_original;    // Máscara original del proceso (la restauraremos)
    struct sigaction act;               // Estructura para definir el manejador

    // Poner a cero toda la estructura act
    memset(&act, 0, sizeof(act));

    // Indicar que el manejador de SIGTERM será la función 'manejador'
    act.sa_handler = manejador;

    // Instalar el manejador para SIGTERM
    if (sigaction(SIGTERM, &act, 0)) {
        perror("sigaction");
        return -1;
    }

    // Crear un conjunto de señales vacío
    sigemptyset(&conjunto_mascaras);

    // Añadir SIGTERM al conjunto
    sigaddset(&conjunto_mascaras, SIGTERM);

    // Bloquear SIGTERM y guardar la antigua máscara en conj_mascaras_original
    if (sigprocmask(SIG_BLOCK, &conjunto_mascaras, &conj_mascaras_original) < 0) {
        perror("primer sigprocmask");
        return -1;
    }

    // Durante estos 10 segundos SIGTERM está bloqueada
    // Si llega, no se entrega, se queda PENDIENTE
    sleep(10);

    // Restaurar (desbloquear) la máscara original
    // Aquí se "entregan" todas las señales pendientes
    if (sigprocmask(SIG_SETMASK, &conj_mascaras_original, NULL) < 0) {
        perror("segundo sigprocmask");
        return -1;
    }

    // Dar tiempo a que, si hay señales pendientes, el manejador se ejecute
    sleep(1);

    // Si el manejador modificó signal_recibida, es que llegó SIGTERM
    if (signal_recibida)
        printf("\nSeñal recibida\n");

    return 0;
}
/* Ejemplo de uso:
./tarea12
kill -15 <PID> -15 es SIGTERM
*/