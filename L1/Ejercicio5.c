#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>

int main(int argc, char *argv[]) {
    int fd;
    struct stat st;
    struct passwd *pw;

    /* Comprobar argumentos */
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    /* Abrir archivo */
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        if (errno == ENOENT)
            fprintf(stderr, "El archivo no existe\n");
        else if (errno == EACCES)
            fprintf(stderr, "No hay permisos para abrir el archivo\n");
        else
            perror("Error al abrir");
        return 2;
    }

    /* Obtener información */
    if (fstat(fd, &st) < 0) {
        fprintf(stderr, "No se pudo obtener información del archivo\n");
        close(fd);
        return 3;
    }

    /* Obtener propietario */
    if ((pw = getpwuid(st.st_uid)) == NULL) {
        fprintf(stderr, "Error al obtener el propietario\n");
        close(fd);
        return 4;
    }

    /* Mostrar información */
    printf("Nombre del archivo: %s\n", argv[1]);
    printf("Tamaño en bytes: %ld\n", st.st_size);
    printf("Permisos: %04o\n", st.st_mode & 0777);
    printf("Nombre del propietario: %s\n", pw->pw_name);

    /* Cerrar archivo */
    if (close(fd) < 0) {
        perror("Error al cerrar el archivo");
        return 5;
    }

    return 0;
}
