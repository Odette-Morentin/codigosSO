#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PATH 4096

// Estructura para almacenar y pasar los resultados acumulados
typedef struct {
    long long total_bytes;  // Suma total de bytes ocupados
    int contador_archivos;  // Cuenta de archivos encontrados
    // No necesitamos un campo para los i-nodos, los imprimiremos directamente
} Resultados;

/**
 * Función recursiva para recorrer el directorio y contar archivos.
 * @param ruta_base Directorio actual a explorar.
 * @param resultados Puntero a la estructura donde se acumulan los datos.
 */
void recorrer_directorio(const char *ruta_base, Resultados *resultados) {
    DIR *dirp;
    struct dirent *dp;
    struct stat st;
    char ruta_completa[MAX_PATH];
    
    // 1. Abrir el directorio
    dirp = opendir(ruta_base);
    if (dirp == NULL) {
        // Imprimir error si el directorio no se puede abrir (e.g., permisos denegados)
        fprintf(stderr, "Error al abrir %s: %s\n", ruta_base, strerror(errno));
        return;
    }

    // 2. Leer las entradas
    while ((dp = readdir(dirp)) != NULL) {
        
        // Excluir las entradas especiales '.' y '..'
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        // 3. Construir la ruta completa
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta_base, dp->d_name);

        // 4. Obtener información con lstat
        if (lstat(ruta_completa, &st) == -1) {
            perror("Error en lstat");
            continue;
        }

        // 5. Verificar el tipo y permisos
        if (S_ISDIR(st.st_mode)) {
            // --- Caso A: Es un SUBDIRECTORIO ---
            recorrer_directorio(ruta_completa, resultados);
        } else if (S_ISREG(st.st_mode)) {
            // --- Caso B: Es un ARCHIVO REGULAR ---
            
            // Verificar si tiene permiso de ejecución (x) para grupo (G) Y otros (O)
            // S_IXGRP (0010) y S_IXOTH (0001)
            if ((st.st_mode & S_IXGRP) && (st.st_mode & S_IXOTH)) {
                
                // 5.1. Imprimir la información requerida (nombre e i-nodo)
                printf("%s %ld\n", ruta_completa, (long)st.st_ino);
                
                // 5.2. Actualizar el contador de archivos
                resultados->contador_archivos++;
                
                // 5.3. Sumar el tamaño (bytes)
                resultados->total_bytes += st.st_size;
            }
        }
    }

    // 6. Cerrar el directorio
    closedir(dirp);
}

// --- Función Principal ---
int main(int argc, char *argv[]) {
    Resultados resultados = {0, 0};
    const char *directorio_inicial;

    // 1. Determinar el directorio inicial
    if (argc == 1) {
        directorio_inicial = "."; // Si no hay argumento, usa el directorio actual
    } else {
        directorio_inicial = argv[1]; // Usa el argumento proporcionado
    }
    
    printf("Los i-nodos son:\n");
    
    // 2. Llamar a la función recursiva
    recorrer_directorio(directorio_inicial, &resultados);
    
    // 3. Imprimir el resumen final
    printf("\nExisten %d archivos regulares con permiso x para grupo y otros\n", resultados.contador_archivos);
    printf("El tamaño total ocupado por dichos archivos es %lld bytes\n", resultados.total_bytes);
    
    return 0;
}