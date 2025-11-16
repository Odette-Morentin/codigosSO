/*Estructura de 
    struct group {
   char *gr_name;   nombre del grupo
   char *gr_passwd; contraseña del grupo
   gid_t gr_gid;    ID del grupo
   char **gr_mem;   lista de miembros del grupo
}; */

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

int main(void) {

    //Conseguir el usuario actual
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (!pw) {
        perror("getpwuid");
        return 1;
    }

    printf("Usuario: %s\n", pw->pw_name);
    printf("Grupo primario (GID): %d\n\n", pw->pw_gid);

    struct group *gr = getgrgid(pw->pw_gid); //Buscar grupo por ID
    if (!gr) {
        perror("getgrgid");
        return 1;
    }

    printf("Información del grupo:\n");
    printf("Nombre del grupo: %s\n", gr->gr_name);
    printf("Contraseña del grupo: %s\n", gr->gr_passwd);
    printf("GID: %d\n", gr->gr_gid);

    printf("Miembros del grupo:\n");
    if (gr->gr_mem[0] == NULL) {
        printf("  (No hay miembros extra, solo usuario primario)\n");
    } else {
        for (int i = 0; gr->gr_mem[i] != NULL; i++) {
            printf("  %s\n", gr->gr_mem[i]);
        }
    }

    return 0;
}

/*
Buscar 
struct group *gr = getgrnam("sudo");

if (gr) {
    printf("GID del grupo sudo: %d\n", gr->gr_gid);
}
    */
