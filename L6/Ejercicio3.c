// deadlock_locktest.c
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void lock_write_blocking(int fd, const char *tag, const char *fname) {
    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type   = F_WRLCK;   // bloqueo de escritura
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;         // 0 = hasta EOF (todo el archivo)

    printf("[%s] intentando bloquear (WRLCK) %s...\n", tag, fname);
    fflush(stdout);

    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        fprintf(stderr, "[%s] fcntl(F_SETLKW) en %s falló: errno=%d (%s)\n",
                tag, fname, errno, strerror(errno));
        exit(2);
    }

    printf("[%s] bloqueo conseguido en %s\n", tag, fname);
    fflush(stdout);
}

int main(void) {
    const char *f1 = "f1.lock";
    const char *f2 = "f2.lock";

    int fd1 = open(f1, O_CREAT | O_RDWR, 0644);
    int fd2 = open(f2, O_CREAT | O_RDWR, 0644);
    if (fd1 < 0 || fd2 < 0) { perror("open"); return 1; }

    // pipes para sincronizar (asegurar que cada uno bloquea su 1er archivo antes del 2º intento)
    int p_parent_ready[2], p_child_ready[2];
    if (pipe(p_parent_ready) < 0 || pipe(p_child_ready) < 0) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        // HIJO: bloquea f2, luego intenta f1
        close(p_parent_ready[0]); close(p_child_ready[1]);

        lock_write_blocking(fd2, "HIJO", f2);

        // avisar al padre: "ya bloqueé f2"
        if (write(p_parent_ready[1], "X", 1) != 1) { perror("write"); exit(1); }

        // esperar a que el padre haya bloqueado f1
        char c;
        if (read(p_child_ready[0], &c, 1) != 1) { perror("read"); exit(1); }

        // ahora intentar el segundo bloqueo (aquí puede aparecer EDEADLK)
        lock_write_blocking(fd1, "HIJO", f1);

        _exit(0);
    } else {
        // PADRE: bloquea f1, luego intenta f2
        close(p_parent_ready[1]); close(p_child_ready[0]);

        lock_write_blocking(fd1, "PADRE", f1);

        // esperar a que el hijo haya bloqueado f2
        char c;
        if (read(p_parent_ready[0], &c, 1) != 1) { perror("read"); return 1; }

        // avisar al hijo: "ya bloqueé f1"
        if (write(p_child_ready[1], "Y", 1) != 1) { perror("write"); return 1; }

        // ahora intentar el segundo bloqueo (aquí puede aparecer EDEADLK)
        lock_write_blocking(fd2, "PADRE", f2);

        int st;
        waitpid(pid, &st, 0);
    }

    return 0;
}
