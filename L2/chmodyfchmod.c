#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Necesario para chmod y fchmod
#include <fcntl.h>    // Necesario para open
#include <unistd.h>   // Necesario para close

int main() {
    // --- 1. USO DE fchmod (Requiere el Descriptor de Archivo) ---
    char *archivo_fchmod = "log_abierto.txt";
    int fd; // Descriptor de archivo
    mode_t modo_fchmod = 0700; // rwx------ (Solo dueño)

    // Crear y abrir el archivo (permisos iniciales 644)
    fd = open(archivo_fchmod, O_CREAT | O_WRONLY, 0644); 
    if (fd == -1) {
        perror("Error al abrir log_abierto.txt");
        return 1;
    }
    printf("1. Archivo '%s' abierto (fd: %d). Permisos iniciales 644.\n", archivo_fchmod, fd);

    // Cambiar permisos usando fchmod (con el descriptor 'fd')
    if (fchmod(fd, modo_fchmod) == 0) {
        printf("   -> Éxito: Permisos cambiados con fchmod a %04o (rwx------).\n", modo_fchmod);
    } else {
        perror("Error al usar fchmod");
    }
    close(fd);

    printf("----------------------------------------------------\n");


    // --- 2. USO DE chmod (Requiere la Ruta del Archivo) ---
    char *archivo_chmod = "config_por_ruta.txt";
    mode_t modo_chmod = 0444; // r--r--r-- (Solo lectura para todos)

    // Crear el archivo (el programa NO lo mantiene abierto después)
    int fd_temp = open(archivo_chmod, O_CREAT | O_WRONLY, 0644); 
    if (fd_temp == -1) {
        perror("Error al crear config_por_ruta.txt");
        return 1;
    }
    close(fd_temp); // ¡Cerramos el archivo! Ya no tenemos el descriptor.
    printf("2. Archivo '%s' creado y cerrado. Permisos iniciales 644.\n", archivo_chmod);

    // Cambiar permisos usando chmod (con la RUTA del archivo)
    // El archivo NO está abierto por el programa en este momento.
    if (chmod(archivo_chmod, modo_chmod) == 0) {
        printf("   -> Éxito: Permisos cambiados con chmod a %04o (r--r--r--).\n", modo_chmod);
    } else {
        perror("Error al usar chmod");
    }

    // Limpieza (opcional: eliminar archivos creados)
    // unlink(archivo_fchmod);
    // unlink(archivo_chmod);

    return 0;
}