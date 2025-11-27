/*
tarea7.c
Programa ilustrativo del uso de pipes y la redirección de entrada y
salida estándar: "ls | sort"
*/

#include <sys/types.h>   // Tipos como pid_t
#include <fcntl.h>       // Para manejo de archivos y flags (no se usan aquí directamente)
#include <unistd.h>      // Para pipe(), fork(), dup(), close(), exec()
#include <stdio.h>       // printf(), perror()
#include <stdlib.h>      // exit()
#include <errno.h>       // Gestión de errores

int main(int argc, char *argv[]) 
{
    int fd[2];       // fd[0] → lectura del pipe, fd[1] → escritura del pipe
    pid_t PID;

    pipe(fd);        // Llamada al sistema para crear un pipe

    if ( (PID = fork()) < 0 ) {
        perror("fork");
        exit(-1);
    }

    // -----------------------------------------------------------
    // PROCESO HIJO → ejecutará "ls"
    // -----------------------------------------------------------
    if (PID == 0) {  // ls

        // Cierra el extremo de lectura del pipe: el hijo solo escribirá
        close(fd[0]);

        // Redirigir la salida estándar para enviar datos al cauce
        //--------------------------------------------------------

        // Cerrar la salida estándar del proceso hijo (descriptor 1 = STDOUT_FILENO)
        close(STDOUT_FILENO);

        // Duplicar el descriptor de escritura del pipe sobre la salida estándar
        // dup(fd[1]) hace que el descriptor 1 (stdout) *sea* una copia de fd[1]
        //
        // Desde este punto, cualquier printf o salida del comando "ls"
        // NO va a la pantalla → va al pipe.
        dup(fd[1]);

        // Ejecutar el comando "ls"
        // Reemplaza completamente el código del hijo por el binario ls
        execlp("ls", "ls", NULL);

        // Si exec falla, el código continúa (lo cual es raro)
        perror("execlp ls");
        exit(1);
    }

    // -----------------------------------------------------------
    // PROCESO PADRE → ejecutará "sort"
    // -----------------------------------------------------------
    else {  // sort. Estoy en el proceso padre porque PID != 0

        // Cerrar el descriptor de escritura del pipe:
        // el padre solo necesita LEER los datos enviados por "ls"
        close(fd[1]);

        // Redirigir la entrada estándar para tomar los datos del cauce.
        // Cerrar la entrada estándar del proceso padre (descriptor 0 = STDIN_FILENO)
        close(STDIN_FILENO);

        // Duplica fd[0] (lectura del pipe) en el descriptor 0 (stdin)
        //
        // Ahora, cuando "sort" lea de stdin, realmente estará leyendo del pipe
        // que le envía los datos del comando "ls".
        dup(fd[0]);

        // Ejecutar el comando "sort"
        execlp("sort", "sort", NULL);

        perror("execlp sort");
        exit(1);
    }

    return(0);
}
