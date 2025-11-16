

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // close
#include <sys/types.h>
#include <sys/stat.h>   // fstat, struct stat
#include <fcntl.h>      // open
#include <pwd.h>        // getpwuid, struct passwd
#include <errno.h>      // errno
#include <string.h>     // strerror

int main(int argc, char *argv[]) {
    int fd;
    struct stat st;
    struct passwd *pw;
    const char *pathname;

    // 1. Comprobar argumentos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;   // fallo por uso incorrecto
    }

    pathname = argv[1];

    // 2. Abrir el archivo (solo lectura)
    fd = open(pathname, O_RDONLY);
    if (fd == -1) {
        // Error al abrir: miramos errno
        if (errno == ENOENT) {
            fprintf(stderr, "Error: el archivo '%s' no existe.\n", pathname); //stdi (teclado), stdout(Pantalla),stderr(e)
        } else if (errno == EACCES) {
            fprintf(stderr, "Error: no tiene permisos para abrir '%s'.\n", pathname);
        } else {
            fprintf(stderr, "Error al abrir '%s': %s\n", pathname, strerror(errno));
        }
        return 2;   // código de error al abrir
    }

    // 3. Obtener información del archivo con fstat
    if (fstat(fd, &st) == -1) {
        fprintf(stderr, "Error al obtener información de '%s': %s\n",
                pathname, strerror(errno));
        close(fd);  // intentamos cerrar igual
        return 3;   // código de error al obtener info
    }

    // 4. Obtener nombre del propietario a partir del UID
    pw = getpwuid(st.st_uid);
    const char *owner_name = pw ? pw->pw_name : "(desconocido)";

    // 5. Mostrar la información por la salida estándar
    printf("Nombre del archivo: %s\n", pathname);
    printf("Tamaño en bytes: %lld\n", (long long)st.st_size);
    printf("Permisos (octal): %04o\n", st.st_mode & 0777);
    printf("Propietario: %s\n", owner_name);

    // 6. Cerrar el archivo
    if (close(fd) == -1) {
        fprintf(stderr, "Error al cerrar '%s': %s\n",
                pathname, strerror(errno));
        return 4;   // error al cerrar
    }

    return 0;   // éxito
}
