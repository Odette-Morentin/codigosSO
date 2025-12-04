// Ejemplo de uso:

#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int orden, /* argumento_orden */);
/* Retorna: si Ok, depende de orden; -1 si error. */

/*
Significado de los argumentos:

fd → descriptor del archivo

orden = F_GETFL, F_SETFL, F_SETLK, F_SETLKW, F_GETLK

argumento_orden = (no aplica) → se omite (no es necesario)
*/