#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define LOCKFILE "/tmp/mi_programa.lock"

int main(void) {
    int fd;
    struct flock lock;

    /* Abrir o crear el archivo de bloqueo */
    fd = open(LOCKFILE, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Eliminar cualquier contenido previo */
    if (ftruncate(fd, 0) < 0) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    /* Preparar el bloqueo */
    lock.l_type   = F_WRLCK;     // Bloqueo exclusivo
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;           // Todo el archivo

    /* Intentar adquirir el bloqueo (NO bloqueante) */
    if (fcntl(fd, F_SETLK, &lock) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            fprintf(stderr, "Ya hay una instancia en ejecución\n");
            exit(EXIT_FAILURE);
        } else {
            perror("fcntl");
            exit(EXIT_FAILURE);
        }
    }

    printf("Instancia única en ejecución (PID %d)\n", getpid());

    /* Bucle infinito para comprobar que no se puede lanzar otra instancia */
    while (1) {
        sleep(5);
    }

    return 0;
}
