#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

void main(void)
{
    printf("Identificador de usuario: %d\n", getuid()); //Quién inició el proceso
    printf("Identificador de usuario efectivo: %d\n", geteuid()); //Con qué permisos corre
    printf("Identificador de grupo: %d\n", getgid()); //Grupo del usuario que inició el proceso
    printf("Identificador de grupo efectivo: %d\n", getegid()); //Qué permisos grupales usa el programa
}