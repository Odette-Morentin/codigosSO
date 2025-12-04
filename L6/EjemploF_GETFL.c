#include <unistd.h>
#include <fcntl.h>

// Ejemplo para ver las banderas del archivo
int banderas;

banderas=fcntl(fd, F_GETFL);
if (banderas == -1)
    perror("fcntl error");


// Comprobar si el archivo fue abierto para escrituras sincronizadas:
if (banderas & O_SYNC)
    printf("Las escrituras son sincronizadas \n");


// Saber con qué permisos fue abierto:
int ModoAcceso;
ModoAcceso=banderas & O_ACCMODE; // sirve para extraer exclusivamente el modo de acceso (lectura/escritura) de las banderas de apertura.
if (ModoAcceso == O_WRONLY || ModoAcceso == O_RDWR)
    printf("El archivo permite la escritura \n");


// Por ejemplo, para habilitar la bandera O_APPEND podemos escribir el siguiente código:
int bandera;
bandera = fcntl(fd, F_GETFL); //Obtener las banderas
if (bandera == -1)
   perror("fcntl");
bandera |= O_APPEND; // Agregar O_APPEND: mueve el puntero al final
if (fcntl(fd, F_SETFL, bandera) == -1)
   perror("fcntl");


