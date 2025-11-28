// tarea10.c 

#include <stdio.h>
#include <signal.h>

static int s_recibida = 0; 
// Variable global y estática que contará cuántas veces se recibió la señal.
// El handler la incrementará cada vez que llegue SIGINT (Ctrl+C).

// -----------------------------------------------------------------------
// Función que actuará como manejador (handler) de la señal SIGINT.
// Cada vez que llegue SIGINT, esta función se ejecutará automáticamente.
// -----------------------------------------------------------------------
static void handler(int signum) {
    printf("\n Nueva acción del manejador \n");
    s_recibida++;      // Contamos cuántas veces se recibió SIGINT
}

int main()
{
    struct sigaction sa;

    // -------------------------------------------------------------------
    // Asignamos nuestro handler personalizado para SIGINT
    // (Este comentario en el código original estaba mal: NO ignora la señal,
    //   la maneja usando nuestra función handler).
    // -------------------------------------------------------------------
    sa.sa_handler = handler;

    // -------------------------------------------------------------------
    // Inicializamos la máscara del handler.
    // Esto indica qué señales se bloquearán mientras corre el handler.
    // Aquí, ninguna señal será bloqueada.
    // -------------------------------------------------------------------
    sigemptyset(&sa.sa_mask);

    // -------------------------------------------------------------------
    // SA_RESTART:
    // - Si una llamada del sistema es interrumpida por una señal,
    //   se reiniciará automáticamente.
    //
    // Es una buena práctica en handlers.
    // -------------------------------------------------------------------
    sa.sa_flags = SA_RESTART;

    // -------------------------------------------------------------------
    // Instalamos el manejador para SIGINT (Ctrl + C)
    // Si el usuario presiona CTRL+C, se ejecutará la función handler().
    // -------------------------------------------------------------------
    if (sigaction(SIGINT, &sa, NULL) == -1) { 
        printf("error en el manejador");
    }

    // -------------------------------------------------------------------
    // Bucle principal:
    // El programa se quedará aquí hasta que hayamos recibido SIGINT 3 veces.
    //
    // Cada vez que el usuario presione Ctrl+C:
    //    → se ejecuta handler() 
    //    → s_recibida aumenta
    //
    // Cuando s_recibida llegue a 3, el bucle termina y finaliza el programa.
    // -------------------------------------------------------------------
    while (s_recibida < 3);

    return 0;
}
