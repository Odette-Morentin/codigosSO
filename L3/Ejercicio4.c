#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(void){

    int n_procesos = 5;
    pid_t childpid;


    for (int i=0; i<n_procesos;1++)
    {
        if ((childpid=fork()) < 0) //Error al crear el hijo
        {
            fprintf(stderr,"No se pudo crear el hijo %d: %s", i,strerror(errno));
        }
        if (!childpid){
            break;
        }
        printf("Soy el hijo número %d con PID: %d y PPID; %d",i,getpid(),getppid());
        sleep(1);
    }
    return 0;
}