// proxy.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define CHUNK 1024
#define LOCKFILE "bloqueo"

static void lock_screen(int lockfd) {
    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // todo el archivo
    if (fcntl(lockfd, F_SETLKW, &fl) < 0) {
        perror("fcntl lock");
        exit(2);
    }
}

static void unlock_screen(int lockfd) {
    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(lockfd, F_SETLKW, &fl) < 0) {
        perror("fcntl unlock");
        exit(2);
    }
}

int main(void) {
    char buf[CHUNK];
    ssize_t n;

    FILE *tmp = tmpfile(); // se borra automáticamente al cerrar
    if (!tmp) {
        perror("tmpfile");
        return 1;
    }

    // (6) Leer del FIFO (stdin redirigido) hasta EOF
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        // (7) Guardar en temporal
        size_t w = fwrite(buf, 1, (size_t)n, tmp);
        if (w != (size_t)n) {
            perror("fwrite");
            fclose(tmp);
            return 1;
        }
    }
    if (n < 0) {
        perror("read stdin");
        fclose(tmp);
        return 1;
    }

    // Abrir archivo de bloqueo (lo crea el servidor, pero por si acaso)
    int lockfd = open(LOCKFILE, O_CREAT | O_RDWR, S_IRWXU);
    if (lockfd < 0) {
        perror("open bloqueo");
        fclose(tmp);
        return 1;
    }

    // (8) Exclusión mutua para imprimir
    lock_screen(lockfd);

    // Imprimir desde el temporal
    rewind(tmp);
    size_t r;
    while ((r = fread(buf, 1, sizeof(buf), tmp)) > 0) {
        ssize_t out = write(STDOUT_FILENO, buf, r);
        if (out < 0) {
            perror("write stdout");
            unlock_screen(lockfd);
            close(lockfd);
            fclose(tmp);
            return 1;
        }
    }
    // cierre “bonito” de impresión
    write(STDOUT_FILENO, "\n", 1);

    unlock_screen(lockfd);

    close(lockfd);
    fclose(tmp);
    return 0;
}
