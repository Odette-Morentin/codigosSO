#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);

/* Si pid es positivo, entonces se envía la señal sig al proceso con identificador de proceso igual a pid. 
    pid > 0

    kill(1234, SIGTERM);   // Envía SIGTERM al proceso con PID 1234
    kill(5000, SIGKILL);   // Mata solo al proceso 5000
    kill(getpid(), SIGUSR1); // Me envío SIGUSR1 a mí mismo
*/


/*Si pid es 0, entonces sig se envía a cada proceso en el grupo de procesos del proceso actual.
    pid = 0
    kill(0, SIGSTOP);  // Detiene todos los procesos del mismo grupo
    kill(0, SIGUSR1);  // Envía SIGUSR1 a todos en mi grupo
*/

/* Si pid es igual a -1, entonces se envía la señal sig a cada proceso, excepto al primero, desde los números más altos en la tabla de procesos hasta los más bajos.
    pid = -1
    kill(-1, SIGTERM);  // Envía SIGTERM a todos los procesos que pueda

*/

/* Si pid es menor que -1, entonces se envía sig a cada proceso en el grupo de procesos -pid.
    pid < - 1
    kill(-20, SIGKILL); // Mata todos los procesos del grupo 20
    kill(-40, SIGUSR1); // Enviar SIGUSR1 al grupo 40
*/

/* Si sig es 0, entonces no se envía ninguna señal, pero sí se realiza la comprobación de errores.
    sig = 0
    if (kill(1234, 0) == 0) {
        printf("El proceso 1234 existe y tengo permiso.\n");
    } else {
        printf("No existe o no tengo permiso.\n");
    }
*/