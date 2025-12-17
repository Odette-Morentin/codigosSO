/*
Desarrollar un programa que utilizando un cause sin nombre permita que un proceso padre y dos hijos se comuniquen.
Este programa tendrá como argumento el nombre de un fichero.
1. El primer hijo obtendrá el número de bloques asignados al fichero.
2. El segundo hijo obtendrá el tamaño de un archivoen bytes usando la función du.
3. El padre que debe esperar para que no queden hijos zombies y deberám mostrar por pantalla la información obtenida por sus hijos.
*/

/*
Entender ejercicio:
Padre -> Lee
Hijos -> Escribir 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipe1[2], pipe2[2];
    struct stat st;
    pid_t pid1, pid2;

    /* Validar entrada */
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtener información del archivo */
    if (lstat(argv[1], &st) < 0) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    /* Crear pipes */
    if (pipe(pipe1) < 0) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe2) < 0) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    /* ---------- Hijo 1: número de bloques ---------- */
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork hijo 1");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0) {
        long bloques = st.st_blocks;

        close(pipe1[0]);  // Cierra lectura
        write(pipe1[1], &bloques, sizeof(bloques));
        close(pipe1[1]);

        exit(EXIT_SUCCESS);
    }

    /* ---------- Hijo 2: tamaño en bytes ---------- */
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork hijo 2");
        exit(EXIT_FAILURE);
    }
    else if (pid2 == 0) {
        off_t tam = st.st_size;

        close(pipe2[0]);  // Cierra lectura
        write(pipe2[1], &tam, sizeof(tam));
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    }

    /* ---------- Padre ---------- */
    close(pipe1[1]);  // Cierra escritura
    close(pipe2[1]);

    long bloques;
    off_t tam;

    read(pipe1[0], &bloques, sizeof(bloques));
    read(pipe2[0], &tam, sizeof(tam));

    close(pipe1[0]);
    close(pipe2[0]);

    /* Esperar a los hijos (evitar zombis) */
    wait(NULL);
    wait(NULL);

    /* Mostrar resultados */
    printf("Bloques asignados al fichero: %ld\n", bloques);
    printf("Tamaño del fichero en bytes: %ld\n", (long)tam);

    return EXIT_SUCCESS;
}