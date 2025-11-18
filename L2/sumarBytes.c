#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PATH 4096

// --- Función Recursiva ---
// Ahora devuelve el número total de bytes encontrados en ese subárbol
long long recorrer_directorio(const char *ruta_base) {
    DIR *dirp;
    struct dirent *dp;
    struct stat st;
    char ruta_completa[MAX_PATH];
    long long total_bytes = 0; // Acumulador local para este nivel y sus subniveles

    dirp = opendir(ruta_base);
    if (dirp == NULL) {
        // Imprimir error si el directorio no se puede abrir (e.g., permisos)
        fprintf(stderr, "Error al abrir %s: %s\n", ruta_base, strerror(errno));
        return 0; // Devuelve 0 bytes si no se pudo abrir
    }

    while ((dp = readdir(dirp)) != NULL) {
        
        // Excluir las entradas especiales '.' y '..'
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        // Construir la ruta completa
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta_base, dp->d_name);

        // Usamos lstat para manejar enlaces simbólicos correctamente
        if (lstat(ruta_completa, &st) == -1) {
            perror("Error en lstat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // --- Caso A: Es un SUBDIRECTORIO ---
            // Llamada recursiva, y SUMAR el resultado al acumulador total
            total_bytes += recorrer_directorio(ruta_completa);
        } else if (S_ISREG(st.st_mode)) {
            // --- Caso B: Es un ARCHIVO REGULAR ---
            // Sumar el tamaño del archivo al acumulador local
            total_bytes += st.st_size;
            printf("Archivo: %s (Tamaño: %lld bytes)\n", ruta_completa, (long long)st.st_size);
        }
        // Nota: Los enlaces simbólicos (S_ISLNK) y otros tipos se ignoran para la suma de tamaño.
    }

    closedir(dirp);
    return total_bytes; // Devolver la suma total
}

// --- Función Principal ---
int main(int argc, char *argv[]) {
    long long resultado_final;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <directorio_inicial>\n", argv[0]);
        return 1;
    }
    
    printf("--- Iniciando cálculo de bytes desde: %s ---\n", argv[1]);
    
    // Llamar a la función recursiva y guardar el resultado
    resultado_final = recorrer_directorio(argv[1]);
    
    printf("\n--- Recorrido finalizado ---\n");
    printf("SUMA TOTAL DE BYTES DE ARCHIVOS REGULARES: %lld bytes\n", resultado_final);
    
    return 0;
}