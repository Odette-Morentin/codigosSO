/*
tarea6.c
Trabajo con llamadas al sistema del Subsistema de Procesos y Cauces conforme a POSIX 2.10
*/

#include <sys/types.h>   // Tipos como pid_t, necesarios para fork()
#include <fcntl.h>       // Control de archivos (no se usa aquí directamente)
#include <unistd.h>      // Para pipe(), fork(), read(), write(), close()
#include <stdio.h>       // printf(), perror()
#include <stdlib.h>      // exit()
#include <errno.h>       // Manejo de errores

int main(int argc, char *argv[])
{
    int fd[2], numBytes;    // fd[0] = lectura del pipe, fd[1] = escritura del pipe
    pid_t PID;              // Almacena el PID devuelto por fork()
    char mensaje[]= "\nEl primer mensaje transmitido por un cauce!!\n"; 
                            // Mensaje que el hijo enviará al padre
    char buffer[80];        // Buffer para recibir la cadena en el padre

    // pipe(fd): crea un *cauce sin nombre* (un pipe anónimo)
    // fd[0] → extremo de lectura
    // fd[1] → extremo de escritura
    pipe(fd); // Llamada al sistema para crear un cauce sin nombre

    // Crear un nuevo proceso
    // fork() devuelve:
    //   < 0 → error
    //   = 0 → estamos en el proceso hijo
    //   > 0 → estamos en el proceso padre (PID del hijo)
    if ( (PID = fork()) < 0 ) {
        perror("fork");
        exit(1);
    }

    // -------------------------
    // PROCESO HIJO
    // -------------------------
    if (PID == 0) {

        // Cierre del descriptor de lectura en el proceso hijo
        // El hijo solo escribirá en el pipe, así que no necesita leer
        close(fd[0]);

        // Enviar el mensaje a través del cauce usando el descriptor de escritura
        //
        // write(fd[1], mensaje, strlen(mensaje)+1)
        // strlen(mensaje)+1 incluye el caracter '\0'
        write(fd[1], mensaje, strlen(mensaje) + 1);

        // Termina el proceso hijo
        exit(0);
    }

    // -------------------------
    // PROCESO PADRE
    // -------------------------
    else { // Estoy en el proceso padre porque PID != 0

        // Cerrar el descriptor de escritura en el proceso padre
        // El padre solo necesita leer el pipe
        close(fd[1]);

        // Leer datos desde el cauce.
        // El read() se bloquea hasta que el hijo escriba algo.
        numBytes = read(fd[0], buffer, sizeof(buffer));

        // Mostrar cuántos bytes llegaron
        printf("\nEl número de bytes recibidos es: %d", numBytes);

        // Mostrar el mensaje recibido
        printf("\nLa cadena enviada a través del cauce es: %s", buffer);
    }

    return(0);
}


