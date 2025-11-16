#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    // --- Archivo 1: sin cambiar umask ---
    int fd1 = open("normal.txt", O_CREAT | O_WRONLY, 0666); 
    if (fd1 < 0) {
        perror("open normal.txt");
        return 1;
    }
    close(fd1);

    // --- Archivo 2: cambiando umask ---
    umask(0022);   // bloquea escritura para grupo y otros

    int fd2 = open("con_umask.txt", O_CREAT | O_WRONLY, 0666);
    if (fd2 < 0) {
        perror("open con_umask.txt");
        return 1;
    }
    close(fd2);

    printf("Archivos creados. Usa 'ls -l' para ver los permisos.\n");

    return 0;
}

/*
Permiso 0666 y 0022

brirlor = 4, w = 2, x = 1
0 -> octal, 6 -> Permiso de usuario, 6 -> permiso de grupo, 6 -> Otros
rw- rw- rw- = Nadie puede ejecutarlo, solo leerlo y escribir

r = 4, w = 2, x = 1
0 -> octal, 0 -> Permiso de usuario, 2 -> Permiso de grupo, 2 ->  Otros
--- -w- -w- = Quita permiso de escritura

Bloqueo de permisos 0022 (umask bloquea)
    rw- rw- rw-  __
    --- -w- -w-  
------------------
    rw- r-- r-- -> 0644

    r = 4, w = 2, x = 1
0 -> octal, 6 -> Permiso de usuario, 2 -> permiso de grupo, 2 -> Otros
rw- r-- r-- = Solo el usuario escribe y todos leen (Permisos estándar en linux)
*/
