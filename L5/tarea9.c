// tarea9.c

#include <stdio.h>
#include <signal.h>

int main()
{
    struct sigaction sa;   // Estructura donde configuramos el comportamiento de la señal

    // ---------------------------------------------------------------------
    // 1. sa_handler = SIG_IGN
    // ---------------------------------------------------------------------
    // Esto indica que la señal NO tendrá manejador.
    // En lugar de ejecutar un handler, la señal SE IGNORA completamente.
    //
    // En este caso: se ignora SIGINT (Ctrl + C).
    //
    // Eso significa que, aunque el usuario presione Ctrl+C, este programa
    // no se detendrá ni ejecutará ningún manejador.
    // ---------------------------------------------------------------------
    sa.sa_handler = SIG_IGN;  // Ignorar la señal

    // ---------------------------------------------------------------------
    // 2. sa_mask
    // ---------------------------------------------------------------------
    // Lista de señales que se bloquearán mientras se ejecute el handler.
    // Como no hay handler (la señal se ignora), esto solo debe inicializarse.
    //
    // sigemptyset() → deja el conjunto de señales vacío.
    // ---------------------------------------------------------------------
    sigemptyset(&sa.sa_mask);

    // ---------------------------------------------------------------------
    // 3. sa_flags = SA_RESTART
    // ---------------------------------------------------------------------
    // SA_RESTART significa:
    //   “Si una llamada al sistema fue interrumpida por una señal,
    //    la llamada se reanudará automáticamente.”
    //
    // Aunque estemos ignorando SIGINT, es buena práctica mantenerlo.
    // ---------------------------------------------------------------------
    sa.sa_flags = SA_RESTART;

    // ---------------------------------------------------------------------
    // 4. Registrar el nuevo comportamiento de SIGINT
    // ---------------------------------------------------------------------
    // sigaction() asocia esta configuración con la señal SIGINT.
    //
    // SIGINT = Ctrl + C
    //
    // Después de esta llamada, presionar Ctrl+C NO detiene el programa.
    // ---------------------------------------------------------------------
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        printf("error en el manejador");
    }

    // ---------------------------------------------------------------------
    // 5. Bucle infinito
    // ---------------------------------------------------------------------
    // El programa se queda ejecutando sin parar.
    // Como SIGINT se ignora, ni Ctrl+C lo detendrá.
    // Para finalizarlo, hay que usar:
    //   - kill (desde otra terminal)
    //   - Ctrl + Z (para suspender)
    //   - kill -9 PID
    // ---------------------------------------------------------------------
    while(1);

    return 0;
}

