#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    DIR *dirp;
    struct dirent *dp;
    
    // Verificar que se proporcionó un directorio
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nombre_directorio>\n", argv[0]);
        return 1;
    }

    // Abrir el directorio
    dirp = opendir(argv[1]);

    if (dirp == NULL) {
        perror("No se pudo abrir el directorio");
        return 1;
    }

    printf("--- Contenido del directorio: %s ---\n", argv[1]);
    
    // Leer cada entrada del directorio usando readdir
    errno = 0; // Se limpia errno antes del bucle
    while ((dp = readdir(dirp)) != NULL) {
        // Imprimir el i-nodo y el nombre del archivo
        printf("I-Node: %ld \t Nombre: %s\n", dp->d_ino, dp->d_name);
    }

    // Verificar si el bucle terminó por un error (si errno es distinto de 0)
    if (errno != 0) {
        perror("Error de lectura de directorio (readdir)");
    }
    
    // Cerrar el directorio
    if (closedir(dirp) == -1) {
        perror("Error al cerrar el directorio");
        return 1;
    }

    printf("--- Lectura completa ---\n");
    return 0;
}