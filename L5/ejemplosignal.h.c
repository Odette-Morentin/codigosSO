// Ejemplos básicos de:

#include <signal.h>

int kill(pid_t pid, int sig); // 1
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact); // 2
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); // 3
int sigpending(sigset_t *set); // 4
int sigsuspend(const sigset_t *mask); // 5

/* kill(pid,sig)
    kill(1234, SIGKILL);   // Mata el proceso con PID 1234
    kill(getpid(), SIGINT); // Me envío SIGINT a mí mismo (como CTRL + C)
*/

/* sigaction(signum,ac,oldact) -> Ejemplo: instalar handler para SIGINT. Sirve para instalar un manejador (handler) de señales, es decir, una función que se ejecuta cuando llega una señal.
    void handler(int s) {
        printf("Recibi SIGINT!\n");
    }

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
*/

/* sigprocmask(how, set, oldset) -> Ejemplo: bloquear SIGINT (Ctrl+C). Sirve para bloquear o desbloquear señales (poner una señal en “modo silencioso”).
*/

/*sigpending(set) -> Ejemplo: bloqueaste SIGINT y presionas Ctrl + C, queda "Pendiente". Sirve para ver qué señales están pendientes de ser entregadas porque estaban bloqueadas.
    sigset_t pend;
    sigpending(&pend);

    if (sigismember(&pend, SIGINT))
        printf("SIGINT está pendiente!\n");
*/

/* sigsuspend(mask) Sirve para: Desbloquear temporalmente señales, Suspender el proceso hasta que llegue una señal, Restaurar la máscara anterior después.
                    Útil para evitar condiciones de carrera.
    sigset_t mask;
    sigemptyset(&mask); // no bloquear nada temporalmente
    sigsuspend(&mask);  // suspende hasta que llegue una señal                   
*/




