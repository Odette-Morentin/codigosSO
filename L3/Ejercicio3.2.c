#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(void) {

    int nprocs = 20;
    int childpid;
    int i;

    printf("PID=%d, PPID=%d: Inicio del programa\n", getpid(), getppid());

    for (i=1; i < nprocs; i++) {

        if ((childpid = fork()) == -1) {
            fprintf(stderr, "Could not create child %d: %s\n",
                    i, strerror(errno));
            exit(-1);
        }

        /*************  TIPO 1  *************/
        /*
        if (childpid)
            break;
        */
        
        /*************  TIPO 2  *************/
        
        if (!childpid)
            break;
        

    }

    printf("PID=%d, PPID=%d\n", getpid(), getppid());
    sleep(1); // Para que no se mezclen tanto las salidas
    
    return 0;
}
