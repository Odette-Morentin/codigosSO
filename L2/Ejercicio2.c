#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    DIR *dir_ptr;
    struct dirent *entry;
    struct stat st;
    long valor;
    mode_t nuevo_permiso, viejo_permiso;
    char ruta[PATH_MAX];

    /* Validación de entrada */
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <directorio> <permisos_octal>\n", argv[0]);
        return 1;
    }

    /* Convertir permisos */
    valor = strtol(argv[2], NULL, 8);
    if (valor < 0 || valor > 0777) {
        fprintf(stderr, "Permisos octales inválidos\n");
        return 2;
    }
    nuevo_permiso = (mode_t)valor;

    /* Abrir directorio */
    dir_ptr = opendir(argv[1]);
    if (dir_ptr == NULL) {
        perror("Error al abrir el directorio");
        return 3;
    }

    printf("--- Archivo | Permisos antiguos | Permisos nuevos ---\n");

    /* Recorrer directorio */
    while ((entry = readdir(dir_ptr)) != NULL) {

        /* Ignorar . y .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(ruta, sizeof(ruta), "%s/%s", argv[1], entry->d_name);

        if (stat(ruta, &st) < 0) {
            fprintf(stderr, "%s | errno=%d | no se pudieron leer permisos\n",
                    entry->d_name, errno);
            continue;
        }

        if (S_ISREG(st.st_mode)) {
            viejo_permiso = st.st_mode & 0777;

            if (chmod(ruta, nuevo_permiso) < 0) {
                fprintf(stderr, "%s | errno=%d | permisos antiguos: 0%o\n",
                        entry->d_name, errno, viejo_permiso);
                continue;
            }

            printf("%s | 0%o | 0%o\n",
                   entry->d_name, viejo_permiso, nuevo_permiso);
        }
    }

    closedir(dir_ptr);
    return 0;
}
