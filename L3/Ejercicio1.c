#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){

    int numero;
    pid_t pid;

    // Validación de entrada
    if(argc !=2 ){
        fprintf(stderr, "Error al pasar los argumentos");
        return 1;
    }

    // Convertir número
    numero = strtol(argv[1], NULL, 10);

    // Crear hijo
    if ((pid = fork()) < 0){
        perror("Error");
        return 2;
    } else if (pid==0){
        // Proceso hijo ejecutando
        if (numero % 2 == 0){
            printf("El hijo con pid %d dice que es par\n", getpid());
        } else {
            printf("El hijo con pid %d dice que no es par\n",getpid());
        }
        // Termina el hijo
        exit(0);

    } else {
        // Padre ejecutando
        if (numero % 4 == 0){
            printf("El padre con pid %d dice que es divisible entre 4\n", getpid());
        } else {
            printf("El padre con pid %d dice que no es divisible entre 4\n",getpid());
        }
        // Esperar al hijo
        wait(NULL);
    }

    return 0;
}