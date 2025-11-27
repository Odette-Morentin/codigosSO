// esclavo.c
// Programa esclavo: calcula números primos en un intervalo y los envía
// por la salida estándar (que el maestro habrá redirigido a un cauce).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // write()
#include <math.h>     // sqrt()
#include <errno.h>

// Función que devuelve 1 si n es primo, 0 si no lo es
int es_primo(int n) {
    if (n < 2)        // 0,1 y negativos no son primos
        return 0;

    int k;
    int limite = (int)sqrt((double)n); // límite superior de búsqueda

    // El enunciado dice: 2 < k <= sqrt(n)
    // Lo habitual (y correcto) es 2 <= k <= sqrt(n)
    for (k = 2; k <= limite; k++) {
        if (n % k == 0)
            return 0; // Si tiene un divisor, no es primo
    }
    return 1; // No se encontró divisor, es primo
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,
                "Uso: %s <limite_inferior> <limite_superior>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    int inicio = atoi(argv[1]);  // Extremo inferior del intervalo
    int fin    = atoi(argv[2]);  // Extremo superior del intervalo

    // Recorremos el intervalo y comprobamos cada número
    for (int n = inicio; n <= fin; n++) {
        if (es_primo(n)) {
            // Enviar el primo al maestro como entero de 4 bytes.
            // La salida estándar (STDOUT_FILENO) estará redirigida
            // a un cauce sin nombre por el proceso maestro.
            int primo = n;
            ssize_t escritos = write(STDOUT_FILENO, &primo, sizeof(int));
            if (escritos != sizeof(int)) {
                perror("Error al escribir primo en el cauce");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Cuando termina el intervalo, el esclavo simplemente acaba,
    // el maestro detectará EOF (read() == 0) en el pipe.
    return 0;
}
