//consumidorFIFO.c
//Consumidor que usa mecanismo de comunicacion FIFO.

#include <sys/types.h>    // Tipos de datos como mode_t, pid_t, etc.
#include <sys/stat.h>     // Para mknod(), mkfifo() y constantes de permisos
#include <fcntl.h>        // Para open() y banderas como O_RDWR
#include <unistd.h>       // Para read(), close() y funciones POSIX
#include <stdio.h>        // Para printf(), perror()
#include <stdlib.h>       // Para exit()
#include <errno.h>        // Para manejo de errores
#include <string.h>       // Para strcmp(), manejo de cadenas

#define ARCHIVO_FIFO "ComunicacionFIFO" //Archivo que ambos procesos usarán para comunicarse

int main(void)
{
    int fd;                     // Descriptor de archivo para interactuar con el FIFO
    char buffer[80];            // Almacenamiento del mensaje recibido del productor
    int leidos;                 // Cantidad de bytes leídos (aunque aquí no se usa de forma significativa)

    //Crear el cauce con nombre (FIFO) si no existe
    umask(0);                   // Ajusta la máscara de permisos para que se apliquen exactamente 0666
                                // (si no se hace, el sistema podría restringir los permisos automáticamente)

    mknod(ARCHIVO_FIFO, S_IFIFO | 0666, 0); 
    // mknod crea un archivo especial.
    // ARCHIVO_FIFO: nombre del archivo FIFO
    // S_IFIFO | 0666: indica que el archivo será un FIFO con permisos lectura/escritura para todos
    // El último parámetro (0) se usa solo para dispositivos especiales, aquí no aplica.

    /* 
    S_IFCHR: Orientado a caracteres
    S_IFBLK: Orientado a bloques
    S_IFSOCK: Archivo para un socket
    S_IFFIFO: Archivo FIFO (tubería con nombre)
    */

    //también vale: mkfifo(ARCHIVO_FIFO,0666); Código de archivo , permisos
    // mkfifo es la llamada más común y clara para crear un FIFO


    //Abrir el cauce para lectura-escritura
    // O_RDWR evita que el proceso quede bloqueado esperando a que otro lo abra
    // Si se usara solo O_RDONLY, el open bloquearía hasta que un escritor abra el FIFO
    if ((fd = open(ARCHIVO_FIFO, O_RDWR)) < 0) {
        perror("open");         // Imprime mensaje de error si no se pudo abrir
        exit(-1);               // Termina el programa con error
    }


    //Aceptar datos a consumir hasta que se envíe la cadena "fin"
    while (1) {

        // Lee hasta 80 caracteres del FIFO y los guarda en buffer
        // read() se queda bloqueado hasta que otro proceso escriba algo en el FIFO
        leidos = read(fd, buffer, 80);

        // IMPORTANTE: No se garantiza que buffer termine en '\0'
        // pero en este caso, se asume que el escritor envía cadenas terminadas en '\0'

        // Si el mensaje recibido es "fin", se cierra el FIFO y termina el programa
        if (strcmp(buffer, "fin") == 0) {
            close(fd);          // Cierra el descriptor de archivo del FIFO
            return 0;           // Sale del programa correctamente
        }

        // Imprime el mensaje recibido desde el productor
        printf("\nMensaje recibido: %s\n", buffer);
    }

    return 0;
}
