#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 80

int main(int argc, char *argv[]) {

    int fd1, fd2;
    char buf[BLOCK_SIZE];
    int num_bloques = 0;
    ssize_t bytes_leidos;
    char encabezado[64];
    off_t posicion;

    /* Abrir archivo de entrada */
    if (argc < 2) {
        fd1 = STDIN_FILENO;
    } else {
        if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
            perror("Error en open entrada");
            exit(EXIT_FAILURE);
        }
    }

    /* Crear archivo de salida */
    if ((fd2 = open("salida.txt",
                    O_CREAT | O_TRUNC | O_WRONLY,
                    S_IRUSR | S_IWUSR)) < 0) {
        perror("Error en open salida");
        exit(EXIT_FAILURE);
    }

    write(fd2, "                    \n", 21);  // espacio fijo


    /* Leer en bloques de 80 bytes */
    while ((bytes_leidos = read(fd1, buf, BLOCK_SIZE)) > 0) {
        num_bloques++;

        snprintf(encabezado, sizeof(encabezado),
                 "Bloque %d\n", num_bloques);
        write(fd2, encabezado, strlen(encabezado));

        write(fd2, "/*\n", 3);
        write(fd2, buf, bytes_leidos);
        write(fd2, "\n*/\n", 4);
    }

    if (lseek(fd2, 0, SEEK_SET) < 0) {
    perror("Error en lseek");
    }
    snprintf(encabezado, sizeof(encabezado), "Hay %d bloques\n", num_bloques);
    write(fd2, encabezado, strlen(encabezado));


    if (bytes_leidos < 0) {
        perror("Error en read");
    }

    if (fd1 != STDIN_FILENO)
        close(fd1);

    close(fd2);

    return 0;
}