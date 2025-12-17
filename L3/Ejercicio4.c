#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t childpid;
    int hijos_vivos = 5;

    /* Crear los 5 hijos */
    for (int i = 0; i < 5; i++) {
        childpid = fork();

        if (childpid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (childpid == 0) {
            printf("Soy el hijo con el pid %d\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

    /* El padre espera a que terminen */
    while (hijos_vivos > 0) {
        childpid = wait(NULL);

        if (childpid > 0) {
            hijos_vivos--;
            printf("Acaba de finalizar mi hijo con %d. "
                   "Sólo me quedan %d hijos vivos\n",
                   childpid, hijos_vivos);
        }
    }

    return 0;
}
