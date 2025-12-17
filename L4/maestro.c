#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int fd1[2], fd2[2];
    pid_t pid1, pid2;
    int lim_inf, lim_sup;
    int mid;
    int primo;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <lim_inf> <lim_sup>\n", argv[0]);
        return 1;
    }

    lim_inf = atoi(argv[1]);
    lim_sup = atoi(argv[2]);

    if (lim_inf > lim_sup) {
        fprintf(stderr, "Error: lim_inf > lim_sup\n");
        return 2;
    }

    mid = (lim_inf + lim_sup) / 2;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        perror("pipe");
        return 3;
    }

    /* ---------- HIJO 1 ---------- */
if ((pid1 = fork()) == 0) {
    close(fd1[0]);                     // No lee del pipe 1
    dup2(fd1[1], STDOUT_FILENO);        // stdout → pipe 1
    close(fd1[1]);

    char buf_inf[20], buf_sup[20];
    sprintf(buf_inf, "%d", lim_inf);
    sprintf(buf_sup, "%d", mid);

    execlp("./esclavo", "esclavo", buf_inf, buf_sup, NULL);

    perror("execlp hijo 1");
    exit(EXIT_FAILURE);
}

/* ---------- HIJO 2 ---------- */
if ((pid2 = fork()) == 0) {
    close(fd2[0]);                     // No lee del pipe 2
    dup2(fd2[1], STDOUT_FILENO);        // stdout → pipe 2
    close(fd2[1]);

    char buf_inf[20], buf_sup[20];
    sprintf(buf_inf, "%d", mid + 1);
    sprintf(buf_sup, "%d", lim_sup);

    execlp("./esclavo", "esclavo", buf_inf, buf_sup, NULL);

    perror("execlp hijo 2");
    exit(EXIT_FAILURE);
}


    /* ---------- Maestro ---------- */
    close(fd1[1]);
    close(fd2[1]);

    printf("Primos encontrados:\n");

    while (read(fd1[0], &primo, sizeof(int)) > 0) {
        printf("%d\n", primo);
    }

    while (read(fd2[0], &primo, sizeof(int)) > 0) {
        printf("%d\n", primo);
    }

    close(fd1[0]);
    close(fd2[0]);

    wait(NULL);
    wait(NULL);

    return 0;
}
