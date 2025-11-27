//productorFIFO.c
//Productor que usa mecanismo de comunicacion FIFO

#include <sys/types.h>   // Tipos POSIX básicos
#include <sys/stat.h>    // Para mkfifo(), permisos y constantes de estado
#include <fcntl.h>       // Para open() y banderas como O_WRONLY
#include <unistd.h>      // Para write(), close(), funciones del sistema
#include <stdio.h>       // Para printf(), perror()
#include <stdlib.h>      // Para exit(), funciones de utilidad
#include <string.h>      // Para strlen(), manejo de cadenas
#include <errno.h>       // Para manejo de errores con errno

#define ARCHIVO_FIFO "ComunicacionFIFO" // Nombre del FIFO compartido con el consumidor

int main(int argc, char *argv[])
{
    int fd;   // Descriptor del archivo FIFO utilizado para escribir

    //Comprobar el uso correcto del programa
    // Se espera exactamente 1 argumento: el mensaje a enviar
    if(argc != 2) {
        printf("\nproductorFIFO: faltan argumentos (mensaje)");
        printf("\nPruebe: productorFIFO <mensaje>, donde <mensaje> es una cadena de caracteres.\n");
        exit(-1); // Finaliza si no se cumple la sintaxis correcta
    }

    //Intentar abrir para escritura el cauce FIFO
    // O_WRONLY indica que este proceso solo escribirá en el FIFO.
    //
    // IMPORTANTE:
    // open() bloqueará la ejecución hasta que el FIFO también se abra para lectura
    // por otro proceso (el consumidor).
    // Si no hay un consumidor ejecutándose, el productor se quedará esperando aquí.
    if( (fd=open(ARCHIVO_FIFO, O_WRONLY)) <0 ) {
        perror("\nError en open");  // Mensaje de error si el FIFO no existe o no se puede abrir
        exit(-1);
    }

    //Escribir en el cauce FIFO el mensaje introducido como argumento
    //
    // argv[1] es la cadena enviada por el usuario.
    // strlen(argv[1]) + 1 → se suma 1 para incluir el carácter nulo '\0',
    // lo que permite al consumidor detectar correctamente el final de la cadena.
    //
    // write devuelve el número de bytes escritos; si no coincide, hubo un error.
    if( (write(fd, argv[1], strlen(argv[1]) + 1)) != strlen(argv[1]) + 1 ) {
        perror("\nError al escribir en el FIFO");  // Indica fallo al escribir
        exit(-1);
    }

    close(fd);  // Cierra el FIFO después de escribir. Esto indica EOF al consumidor si no hay más escritores.
    return 0;   // Fin del programa exitoso
}

