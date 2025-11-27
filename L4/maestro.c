// maestro.c
// Programa maestro: reparte el intervalo entre dos esclavos,
// recoge los números primos desde dos cauces y los muestra ordenados.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      // pipe(), fork(), dup2(), read(), close(), execlp()
#include <sys/types.h>
#include <sys/wait.h>    // wait()
#include <errno.h>
#include <string.h>      // sprintf()

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,
                "Uso: %s <limite_inferior> <limite_superior>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    int inicio = atoi(argv[1]);
    int fin    = atoi(argv[2]);

    if (inicio > fin) {
        fprintf(stderr, "Error: limite_inferior > limite_superior\n");
        exit(EXIT_FAILURE);
    }

    // División del intervalo en dos subintervalos
    int mid = (inicio + fin) / 2;

    int sub1_ini = inicio;
    int sub1_fin = mid;

    int sub2_ini = mid + 1;
    int sub2_fin = fin;

    int pipe1[2];  // pipe para el esclavo 1
    int pipe2[2];  // pipe para el esclavo 2

    if (pipe(pipe1) == -1) {
        perror("Error al crear pipe1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) == -1) {
        perror("Error al crear pipe2");
        exit(EXIT_FAILURE);
    }

    pid_t pid1, pid2;

    // ------------------ Crear ESCLAVO 1 ------------------
    pid1 = fork();
    if (pid1 < 0) {
        perror("Error en fork para esclavo 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Proceso hijo 1: esclavo 1
        // Redirigimos su salida estándar al pipe1 (escritura)

        // Cerramos el extremo de lectura: el hijo solo escribe
        close(pipe1[0]);

        // Duplicamos el extremo de escritura en STDOUT
        dup2(pipe1[1], STDOUT_FILENO);

        // Ya no necesitamos el descriptor original
        close(pipe1[1]);

        // Cerrar pipe2 completo en este hijo (no lo usa)
        close(pipe2[0]);
        close(pipe2[1]);

        // Preparamos los argumentos para el ejecutable esclavo
        char ini_str[32], fin_str[32];
        sprintf(ini_str, "%d", sub1_ini);
        sprintf(fin_str, "%d", sub1_fin);

        execlp("./esclavo", "esclavo", ini_str, fin_str, (char *)NULL);

        // Si execlp falla:
        perror("Error al ejecutar esclavo 1");
        exit(EXIT_FAILURE);
    }

    // ------------------ Crear ESCLAVO 2 ------------------
    pid2 = fork();
    if (pid2 < 0) {
        perror("Error en fork para esclavo 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Proceso hijo 2: esclavo 2
        // Redirigimos su salida estándar al pipe2 (escritura)

        // Cerramos el extremo de lectura de pipe2
        close(pipe2[0]);

        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        // Cerrar pipe1 completo en este hijo (no lo usa)
        close(pipe1[0]);
        close(pipe1[1]);

        // Argumentos para esclavo
        char ini_str[32], fin_str[32];
        sprintf(ini_str, "%d", sub2_ini);
        sprintf(fin_str, "%d", sub2_fin);

        execlp("./esclavo", "esclavo", ini_str, fin_str, (char *)NULL);

        perror("Error al ejecutar esclavo 2");
        exit(EXIT_FAILURE);
    }

    // ------------------ PROCESO MAESTRO ------------------

    // El maestro solo necesita leer de los pipes
    close(pipe1[1]); // cierra escritura
    close(pipe2[1]); // cierra escritura

    // Calculamos tamaños máximos posibles para arrays de primos
    // (en el peor de los casos, todos los números fueran primos)
    int max_primos1 = (sub1_fin >= sub1_ini) ? (sub1_fin - sub1_ini + 1) : 0;
    int max_primos2 = (sub2_fin >= sub2_ini) ? (sub2_fin - sub2_ini + 1) : 0;

    int *primos1 = NULL;
    int *primos2 = NULL;

    if (max_primos1 > 0) {
        primos1 = (int *)malloc(max_primos1 * sizeof(int));
        if (!primos1) {
            perror("Error al reservar memoria para primos1");
            exit(EXIT_FAILURE);
        }
    }
    if (max_primos2 > 0) {
        primos2 = (int *)malloc(max_primos2 * sizeof(int));
        if (!primos2) {
            perror("Error al reservar memoria para primos2");
            exit(EXIT_FAILURE);
        }
    }

    int count1 = 0, count2 = 0;
    int primo;
    ssize_t leidos;

    // Leer todos los primos enviados por el esclavo 1
    while ((leidos = read(pipe1[0], &primo, sizeof(int))) > 0) {
        if (leidos == sizeof(int)) {
            primos1[count1++] = primo;
        } else {
            // Lectura parcial (raro al tratarse de enteros pequeños)
            fprintf(stderr, "Lectura parcial en pipe1\n");
        }
    }

    // Leer todos los primos enviados por el esclavo 2
    while ((leidos = read(pipe2[0], &primo, sizeof(int))) > 0) {
        if (leidos == sizeof(int)) {
            primos2[count2++] = primo;
        } else {
            fprintf(stderr, "Lectura parcial en pipe2\n");
        }
    }

    // Ya no necesitamos leer más
    close(pipe1[0]);
    close(pipe2[0]);

    // Esperar a que terminen los esclavos
    wait(NULL);
    wait(NULL);

    // Ahora primos1[0..count1-1] y primos2[0..count2-1] están ordenados
    // de menor a mayor (cada esclavo recorre su intervalo en orden creciente).
    // Toca fusionarlos y mostrarlos en orden creciente.

    int i = 0, j = 0;

    printf("Primos en el intervalo [%d, %d]:\n", inicio, fin);

    while (i < count1 && j < count2) {
        if (primos1[i] <= primos2[j]) {
            printf("%d\n", primos1[i]);
            i++;
        } else {
            printf("%d\n", primos2[j]);
            j++;
        }
    }

    // Imprimir los que queden en el primer array
    while (i < count1) {
        printf("%d\n", primos1[i]);
        i++;
    }

    // Imprimir los que queden en el segundo array
    while (j < count2) {
        printf("%d\n", primos2[j]);
        j++;
    }

    free(primos1);
    free(primos2);

    return 0;
}
