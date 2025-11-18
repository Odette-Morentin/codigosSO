#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definimos el tamaño máximo para la ruta (un valor seguro en sistemas modernos)
#define MAX_PATH 4096

// --- Función Recursiva ---
void recorrer_directorio(const char *ruta_base) {
    DIR *dirp;
    struct dirent *dp;
    struct stat st;
    char ruta_completa[MAX_PATH];

    // 1. Abrir el directorio actual
    dirp = opendir(ruta_base);
    if (dirp == NULL) {
        perror("Error al abrir directorio");
        // Si no se puede abrir (por permisos, etc.), simplemente terminamos este camino recursivo
        return;
    }

    // 2. Leer las entradas
    while ((dp = readdir(dirp)) != NULL) {
        
        // Excluir las entradas especiales '.' (directorio actual) y '..' (directorio padre)
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        // 3. Construir la ruta completa del elemento actual (clave para la recursión)
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta_base, dp->d_name);

        // 4. Obtener la información del archivo/directorio (necesario para saber si es un directorio)
        if (stat(ruta_completa, &st) == -1) {
            perror("Error en stat");
            continue;
        }

        // 5. Verificar el tipo de archivo
        if (S_ISDIR(st.st_mode)) {
            // --- Caso 5A: Es un SUBDIRECTORIO ---
            printf("DIRECTORIO ENCONTRADO: %s\n", ruta_completa);
            // Llamada recursiva: ¡la función se llama a sí misma!
            recorrer_directorio(ruta_completa);
        } else {
            // --- Caso 5B: Es un ARCHIVO ---
            printf("Archivo: %s\n", ruta_completa);
        }
    }

    // 6. Cerrar el directorio
    closedir(dirp);
}

// --- Función Principal ---
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <directorio_inicial>\n", argv[0]);
        return 1;
    }
    
    printf("--- Iniciando recorrido recursivo desde: %s ---\n", argv[1]);
    recorrer_directorio(argv[1]);
    printf("--- Recorrido finalizado ---\n");
    
    return 0;
}