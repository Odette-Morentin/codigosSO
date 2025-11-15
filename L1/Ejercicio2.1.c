#include <sys/types.h>   // off_t (aunque aquí no lo usas)
#include <sys/stat.h>    // permisos
#include <fcntl.h>       // open
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>       // errno
#include <unistd.h>      // read, write, lseek, close
#include <string.h>      // strlen, snprintf

#define BLOCK_SIZE 80    // Definir tamaño de bytes (sin ;)


// Función para asegurarnos que se escriba todo el buffer
ssize_t write_all(int fd, const void *buf, size_t count) {
    size_t total_written = 0;
    const char *p = buf;

    while (total_written < count) {
        ssize_t n = write(fd, p + total_written, count - total_written);
        if (n < 0) {
            return -1; // error
        }
        total_written += n;
    }
    return total_written;
}

int main (int argc, char *argv[]) {
    // variables
    int fd_in, fd_out, bloque = 1, total_bloques = 0;
    char buffer[BLOCK_SIZE];
    ssize_t nread;

    // buffer para el texto "Bloque n\n/* "
    char header[64];
    int len_header;

    // 1. Determinar archivo de entrada
    if (argc == 1) {
        // Entrada estándar (0 = teclado o lo que se redirija)
        fd_in = STDIN_FILENO;
    } else {
        // entrada como argumento
        fd_in = open(argv[1], O_RDONLY);
        if (fd_in < 0) {
            perror("Fallo abrir archivo de entrada");
            exit(EXIT_FAILURE);
        }
    }

    // Archivo de salida
    fd_out = open("salida.txt",
                  O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Permisos 0644
    if (fd_out < 0) {
        perror("open salida.txt");
        if (argc > 1) {
            close(fd_in);
        }
        exit(EXIT_FAILURE);
    }

    //Reservar una linea al principio
    char linea_dummy[64];
    int len_dummy = snprintf(linea_dummy, sizeof(linea_dummy),
                         "El número de bloques es ??\n");

    if (write_all(fd_out, linea_dummy, len_dummy) < 0) {
        perror("write linea dummy");
        if (argc > 1) close(fd_in);
        close(fd_out);
        exit(EXIT_FAILURE);
}


    // Leer en bloques de 80 bytes y escribir en la salida
    while ((nread = read(fd_in, buffer, BLOCK_SIZE)) > 0) {

        len_header = snprintf(header, sizeof(header), "Bloque %d\n/* ", bloque);
        if (len_header < 0 || len_header >= (int)sizeof(header)) {
            fprintf(stderr, "Error en snprintf\n");
            close(fd_out);
            if (argc > 1) close(fd_in);
            exit(EXIT_FAILURE);
        }

        if (write_all(fd_out, header, len_header) < 0) {
            perror("Write header");
            close(fd_out);
            if (argc > 1) close(fd_in);
            exit(EXIT_FAILURE);
        }

        // Escribir los bytes del bloque actual
        if (write_all(fd_out, buffer, nread) < 0) {
            perror("Write datos bloque");
            close(fd_out);
            if (argc > 1) close(fd_in);
            exit(EXIT_FAILURE);
        }

        // Cerrar el comentario y salto de línea
        const char *tail = " */\n";
        if (write_all(fd_out, tail, strlen(tail)) < 0) {
            perror("Write tail");
            close(fd_out);
            if (argc > 1) close(fd_in);
            exit(EXIT_FAILURE);
        }

        bloque++;
        total_bloques++;
    }

    if (nread < 0) {
        perror("read");
        close(fd_out);
        if (argc > 1) close(fd_in);
        exit(EXIT_FAILURE);
    }

    // Hemos terminado: total_bloques tiene el número de bloques escritos

    // Volver al principio del archivo
    if (lseek(fd_out, 0, SEEK_SET) == (off_t)-1) {
        perror("lseek");
        if (argc > 1) close(fd_in);
        close(fd_out);
        exit(EXIT_FAILURE);
    }

    // Escribir ahora la línea real
    char linea_real[64];
    int len_real = snprintf(linea_real, sizeof(linea_real),
                            "El número de bloques es %d\n", total_bloques);

    if (write_all(fd_out, linea_real, len_real) < 0) {
        perror("write linea real");
        if (argc > 1) close(fd_in);
        close(fd_out);
        exit(EXIT_FAILURE);
    }


    // Cerrar descriptores
    if (argc > 1) {
        if (close(fd_in) < 0) {
            perror("Close entrada");
        }
    }

    if (close(fd_out) < 0) {
        perror("Close salida");
    }

    return 0;
}