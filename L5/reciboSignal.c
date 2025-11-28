/*
 * reciboSignal.c
 * Ejemplo de manejo de señales POSIX mediante la llamada sigaction().
 * El proceso instala un manejador para SIGUSR1 y SIGUSR2,
 * y muestra un mensaje cada vez que las recibe.
 */

#include <sys/types.h>   // Tipos POSIX (pid_t, etc.)
#include <unistd.h>      // Funciones POSIX (pause, etc.)
#include <stdio.h>       // printf(), perror()
#include <signal.h>      // Definición de señales y sigaction
#include <errno.h>       // Manejo de errores

/*
 * Manejador de señal para SIGUSR1 y SIGUSR2.
 * Esta función será llamada automáticamente cuando
 * el proceso reciba una de esas señales.
 */
static void sig_USR_hdlr(int sigNum)
{
    // Comprobar cuál señal se recibió y mostrar mensaje
    if (sigNum == SIGUSR1)
        printf("\nRecibida la señal SIGUSR1\n\n");
    else if (sigNum == SIGUSR2)
        printf("\nRecibida la señal SIGUSR2\n\n");
}

int main(int argc, char *argv[])
{
    // Estructura que definirá el nuevo comportamiento de las señales
    struct sigaction sig_USR_nact;

    /*
     * Desactivar el buffer de salida estándar.
     * Esto hace que printf muestre el texto inmediatamente
     * (útil para ver mensajes en tiempo real al recibir señales).
     */
    if (setvbuf(stdout, NULL, _IONBF, 0))
    {
        perror("\nError en setvbuf");
    }

    /*
     * Asignar la función manejadora a sa_handler.
     * Con esto indicamos qué función se ejecutará al recibir la señal.
     */
    sig_USR_nact.sa_handler = sig_USR_hdlr;

    /*
     * Inicializar la máscara de señales a un conjunto vacío.
     * Esto significa que ninguna señal adicional se bloqueará
     * mientras se esté ejecutando el manejador.
     */
    sigemptyset(&sig_USR_nact.sa_mask);

    /*
     * No usar ninguna opción avanzada: flags = 0.
     * Podríamos usar SA_RESTART, SA_SIGINFO, etc., pero no es necesario.
     */
    sig_USR_nact.sa_flags = 0;

    /*
     * Instalar el manejador para SIGUSR1.
     * Si sigaction devuelve < 0, hubo error y el programa termina.
     */
    if (sigaction(SIGUSR1, &sig_USR_nact, NULL) < 0)
    {
        perror("\nError al intentar establecer el manejador de señal para SIGUSR1");
        exit(-1);
    }

    /*
     * Instalar el manejador para SIGUSR2.
     */
    if (sigaction(SIGUSR2, &sig_USR_nact, NULL) < 0)
    {
        perror("\nError al intentar establecer el manejador de señal para SIGUSR2");
        exit(-1);
    }

    /*
     * Bucle infinito para mantener el proceso en ejecución.
     * En este caso no se utiliza pause(), pero el proceso sigue vivo
     * y puede recibir y manejar señales.
     */
    for (;;)
    {
        // Se podría usar pause(); pero aquí se deja vacío.
    }

    return 0;
}
