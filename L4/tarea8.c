/*
tarea8.c
Programa ilustrativo del uso de pipes y la redirección de entrada y
salida estándar: "ls | sort", utilizando la llamada dup2.
*/

#include <sys/types.h>   // Tipos como pid_t
#include <fcntl.h>       // Flags de control de archivos
#include <unistd.h>      // pipe(), fork(), dup2(), execlp(), close()
#include <stdio.h>       // printf(), perror()
#include <stdlib.h>      // exit()
#include <errno.h>       // Manejo de errores con errno

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t PID;

    pipe(fd);  // Llamada al sistema para crear un pipe: fd[0]=lectura, fd[1]=escritura

    if ( (PID = fork()) < 0 ) {
        perror("\Error en fork");
        exit(-1);
    }

    // -------------------------------------------------
    // PROCESO HIJO → ejecutará "ls"
    // -------------------------------------------------
    if (PID == 0) { // ls

        // Cerrar el descriptor de lectura del pipe: el hijo no va a leer
        close(fd[0]);

        // Redirección de stdout usando dup2
        //
        // dup2(fd[1], STDOUT_FILENO) hace:
        //  1) Cerrar STDOUT_FILENO si está abierto
        //  2) Copiar fd[1] en el descriptor 1 (stdout)
        //
        // Ahora, todo lo que imprima "ls" irá al pipe en vez de a la pantalla.
        dup2(fd[1], STDOUT_FILENO);

        // Ejecutar el comando "ls". Sustituye completamente al proceso hijo.
        execlp("ls", "ls", NULL);

        // Solo llega aquí si exec falla
        perror("execlp ls");
        exit(1);
    }

    // -------------------------------------------------
    // PROCESO PADRE → ejecutará "sort"
    // -------------------------------------------------
    else {  // sort

        // Cerrar el descriptor de escritura del pipe: el padre solo va a leer
        close(fd[1]);

        // Redirección de stdin usando dup2
        //
        // dup2(fd[0], STDIN_FILENO) hace:
        //  1) Cerrar STDIN si está abierto
        //  2) Copiar fd[0] en STDIN (descriptor 0)
        //
        // Ahora, "sort" leerá directamente del pipe.
        dup2(fd[0], STDIN_FILENO);

        // Ejecutar el comando "sort"
        execlp("sort", "sort", NULL);

        perror("execlp sort");
        exit(1);
    }

    return(0);
}




