#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Definimos el tamaño máximo para la ruta
#define MAX_PATH 4096

// Función auxiliar para imprimir los permisos octales (los 9 bits menos significativos)
// st_mode & 0777 asegura que solo se muestren los bits de permisos
void imprimir_permisos(mode_t modo) {
    printf("0%o", modo & 0777);
}

int main(int argc, char *argv[]) {
    DIR *dirp;
    struct dirent *dp;
    struct stat st;
    char ruta_completa[MAX_PATH];
    long modo_long;
    mode_t nuevos_permisos;

    // --- 1. Verificación de Argumentos ---
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <pathname_directorio> <modo_octal_4_digitos>\n", argv[0]);
        return 1;
    }

    // --- 2. Conversión del Argumento Octal ---
    // Usamos strtol para convertir la cadena octal a un valor numérico (long)
    // El '8' en el tercer argumento indica que la base es octal.
    errno = 0; // Limpiamos errno antes de la llamada
    modo_long = strtol(argv[2], NULL, 8); 

    if (errno != 0) {
        perror("Error al convertir el modo octal con strtol");
        return 1;
    }
    
    // Convertir el long al tipo mode_t y asegurarnos de que es un permiso válido (máximo 07777)
    if (modo_long < 0 || modo_long > 07777) {
        fprintf(stderr, "Error: El modo octal debe estar entre 0000 y 7777.\n");
        return 1;
    }

    nuevos_permisos = (mode_t)modo_long;

    // --- 3. Abrir el Directorio ---
    dirp = opendir(argv[1]);
    if (dirp == NULL) {
        perror("Error al abrir el directorio base");
        return 1;
    }

    printf("--- Cambiando permisos a 0%o en el directorio %s ---\n", nuevos_permisos, argv[1]);

    // --- 4. Recorrer, Comprobar y Aplicar chmod ---
    while ((dp = readdir(dirp)) != NULL) {
        
        // Excluir las entradas especiales '.' y '..' para evitar problemas
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        // Construir la ruta completa del elemento actual
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", argv[1], dp->d_name);

        // Obtener la información y permisos actuales del archivo
        if (lstat(ruta_completa, &st) == -1) {
            fprintf(stderr, "Error en lstat para %s: %s\n", dp->d_name, strerror(errno));
            continue; // Saltar a la siguiente entrada
        }

        // --- Aplicar solo a archivos regulares, como requiere el enunciado ---
        // Nota: Los directorios son archivos, pero generalmente los permisos de los directorios
        // se manejan por separado o con otro set de comandos. Usamos S_ISREG para archivos.
        // Si se desean cambiar también los directorios, usar S_ISDIR en vez de S_ISREG.
        if (S_ISREG(st.st_mode)) {
            
            mode_t permisos_antiguos = st.st_mode; // Guardamos los permisos actuales

            // Intentar aplicar los nuevos permisos usando la llamada al sistema chmod
            if (chmod(ruta_completa, nuevos_permisos) == 0) {
                // Éxito: Imprimir el formato requerido
                printf("%s : ", dp->d_name);
                imprimir_permisos(permisos_antiguos);
                printf(" ");
                imprimir_permisos(nuevos_permisos);
                printf("\n");
            } else {
                // Fallo: Imprimir el formato de error requerido
                // <nombre_de_archivo> : <errno> <permisos_antiguos>
                printf("%s : %d ", dp->d_name, errno);
                imprimir_permisos(permisos_antiguos);
                printf("\n");
            }
        }
    }

    // --- 5. Cerrar el Directorio ---
    closedir(dirp);
    return 0;
}