#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sched.h>        // este es el correcto
#include <malloc.h>

#define _GNU_SOURCE         /* Necesario para habilitar clone() */

// Variable global compartida entre el padre y el hilo creado
int variable = 3;

// -------------------------------------------------------------
// FUNCION QUE EJECUTA EL HILO CREADO POR clone()
// -------------------------------------------------------------
int thread(void *p) {

    int tid;

    printf("\nSoy el hilo (hijo)\n");

    sleep(5);               // Simula trabajo

    variable++;             // Incrementa la variable global
                            // Ambos hilos ven la MISMA variable 
                            // porque usan CLONE_VM

    // Obtener el TID del hilo mediante syscall
    tid = syscall(SYS_gettid);

    // getpid() devuelve el PID del proceso (igual para ambos hilos)
    printf("\nPID y TID del hijo: %d %d\n", getpid(), tid);

    printf("\nEn el hijo la variable vale: %d\n", variable);

    return 0;
}

// -------------------------------------------------------------
// PROGRAMA PRINCIPAL: CREA UN HILO CON clone()
// -------------------------------------------------------------
int main() {

    void **stack;   // Puntero a la memoria que usará como stack del hilo
    int i, tid;

    // Reservamos memoria para la pila del hilo (15000 bytes)
    // clone() exige que tú proporciones el stack
    stack = (void **)malloc(15000);
    if (!stack)
        return -1;

    // ---------------------------------------------------------
    // LLAMADA A clone()
    // ---------------------------------------------------------
    // thread → función que ejecutará el hilo
    // (char*)stack + 15000 → dirección AL FINAL del stack
    //                      (el stack crece hacia abajo)
    //
    // Flags:
    //  CLONE_VM       → Comparte memoria con el padre (como un hilo)
    //  CLONE_FILES    → Comparte descriptores de archivo
    //  CLONE_FS       → Comparte info del sistema de archivos
    //  CLONE_THREAD   → Lo convierte en un thread del MISMO proceso
    //  CLONE_SIGHAND  → Comparte manejadores de señales
    //
    // NULL → No pasamos argumentos a thread()
    // ---------------------------------------------------------
    i = clone(thread,
              (char*) stack + 15000,
              CLONE_VM | CLONE_FILES | CLONE_FS |
              CLONE_THREAD | CLONE_SIGHAND,
              NULL);

    sleep(5);   // Esperamos para asegurar que el hilo ejecute

    if (i == -1)
        perror("clone");

    // Obtener el TID del padre (su identificador de hilo)
    tid = syscall(SYS_gettid);

    // getpid() devuelve el PID del proceso que ambos hilos comparten
    printf("\nPID y TID del padre: %d %d\n", getpid(), tid);

    // Aquí vemos si la variable global cambió
    printf("\nEn el padre la variable vale: %d\n", variable);

    return 0;
}
