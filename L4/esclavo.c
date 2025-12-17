#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // write()
#include <math.h>     // sqrt()
#include <errno.h>

// Devuelve 1 si n es primo, 0 si no
int es_primo(int n) {
    if (n < 2)
        return 0;

    int limite = (int)sqrt((double)n);

    for (int k = 2; k <= limite; k++) {
        if (n % k == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int inicio, fin, primo;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <inicio> <fin>\n", argv[0]);
        return 1;
    }

    inicio = atoi(argv[1]);
    fin    = atoi(argv[2]);

    for (int i = inicio; i <= fin; i++) {
        if (es_primo(i)) {
            primo = i;
            if (write(STDOUT_FILENO, &primo, sizeof(int)) != sizeof(int)) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
