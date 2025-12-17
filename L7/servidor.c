// servidor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define CHUNK 1024
#define LOCKFILE "bloqueo"

static void reap(int sig) {
    (void)sig;
    int saved = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
    errno = saved;
}

static void safe_mkfifo(const char *name) {
    if (mkfifo(name, S_IRWXU) < 0) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_fifos_conocidos>\n", argv[0]);
        return 1;
    }

    char fifoe[256], fifos[256];
    snprintf(fifoe, sizeof(fifoe), "%se", argv[1]);
    snprintf(fifos, sizeof(fifos), "%ss", argv[1]);

    // FIFO conocidos
    safe_mkfifo(fifoe);
    safe_mkfifo(fifos);

    // Crear archivo de bloqueo
    int lockfd = open(LOCKFILE, O_CREAT | O_RDWR, S_IRWXU);
    if (lockfd < 0) { perror("open bloqueo"); return 1; }
    close(lockfd);

    // Evitar zombis
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = reap;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        perror("sigaction SIGCHLD");
        return 1;
    }

    // Abrir FIFO de entrada en O_RDWR para evitar EOF por falta de escritores
    int fde = open(fifoe, O_RDWR);
    if (fde < 0) { perror("open fifoe"); return 1; }

    // FIFO de salida: normalmente O_WRONLY; si no hay clientes aún puede bloquear.
    // Solución típica: abrir también en O_RDWR.
    int fds = open(fifos, O_RDWR);
    if (fds < 0) { perror("open fifos"); return 1; }

    printf("Servidor listo. FIFO entrada=%s, salida=%s\n", fifoe, fifos);

    char buf[CHUNK];
    while (1) {
        // (2) esperar petición
        ssize_t n = read(fde, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read fifoe");
            continue;
        }
        if (n == 0) {
            // Con O_RDWR no debería pasar, pero por si acaso:
            continue;
        }

        // (2) crear proxy
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // HIJO = "proxy launcher"
            pid_t mypid = getpid();
            char fifopriv[64];
            snprintf(fifopriv, sizeof(fifopriv), "%d", (int)mypid);

            // Crear FIFO privado
            safe_mkfifo(fifopriv);

            // Abrir para lectura (se bloqueará hasta que el cliente abra en escritura)
            int fdpriv = open(fifopriv, O_RDONLY);
            if (fdpriv < 0) { perror("open fifo privado"); _exit(1); }

            // Redirigir stdin al FIFO privado
            if (dup2(fdpriv, STDIN_FILENO) < 0) { perror("dup2"); _exit(1); }
            close(fdpriv);

            // Ejecutar proxy (leerá de stdin y escribirá a stdout)
            execlp("./proxy", "proxy", (char*)NULL);

            perror("execlp proxy");
            _exit(1);
        } else {
            // PADRE: responder al cliente con PID del proxy
            char msg[64];
            int len = snprintf(msg, sizeof(msg), "%d\n", (int)pid);
            if (write(fds, msg, len) < 0) {
                perror("write fifos");
            }
        }
    }

    // nunca llega aquí
    close(fde);
    close(fds);
    return 0;
}
