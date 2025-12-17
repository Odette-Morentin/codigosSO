#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

long bytes_totales = 0;
int archivo_regular_con_permisosx = 0;

void recorrerDirectorio(const char *directorio) {

    struct dirent *entry;
    struct stat st;
    DIR *dir_ptr;
    char ruta[PATH_MAX];

    dir_ptr = opendir(directorio);
    if (dir_ptr == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir_ptr)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(ruta, sizeof(ruta), "%s/%s",
                 directorio, entry->d_name);

        if (lstat(ruta, &st) < 0) {
            perror("lstat");
            continue;
        }

        if (S_ISREG(st.st_mode)) {
            if ((st.st_mode & S_IXGRP) &&
                (st.st_mode & S_IXOTH)) {

                archivo_regular_con_permisosx++;
                bytes_totales += st.st_size;

                printf("Archivo: %s | Inodo: %lu\n",
                       ruta, st.st_ino);
            }
        }

        if (S_ISDIR(st.st_mode)) {
            recorrerDirectorio(ruta);
        }
    }

    closedir(dir_ptr);
}

int main(int argc, char *argv[]) {
    const char *directorio_inicial;

    if (argc < 2)
        directorio_inicial = ".";
    else
        directorio_inicial = argv[1];

    recorrerDirectorio(directorio_inicial);

    printf("\nHay %d archivos regulares con x para grupo y otros\n",
           archivo_regular_con_permisosx);
    printf("Tamaño total de bytes: %ld\n", bytes_totales);

    return 0;
}
