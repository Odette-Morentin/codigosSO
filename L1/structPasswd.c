
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

/* Estructura de 
   struct passwd {
   char *pw_name;    nombre de usuario 
   char *pw_passwd;  contraseña 
   uid_t pw_uid;     id. del usuario
   gid_t pw_gid;     id. del grupo primario 
   char *pw_gecos;   nombre real (información GECOS) 
   char *pw_dir;     directorio de inicio 
   char *pw_shell;  shell de inicio de sesión 
}; */

int main(void){
    uid_t my_uid = getuid();  // obtiene UID del usuario en ejecución
    struct passwd *pw = getpwuid(my_uid); // obtiene la entrada passwd

    if (pw == NULL){
        perror("getpwuid");
        return 1;
    }

    printf("Información del usuario:\n");
    printf("Nombre del usuario: %s\n",pw->pw_name);
    printf("Contraseña: %s\n", pw->pw_passwd);
    printf("Id del usuario: %d\n",pw->pw_uid);
    printf("Id del grupo primario: %d\n",pw->pw_gid);
    printf("Nombre real con gecos: %s\n",pw->pw_gecos);
    printf("Ruta del home: %s\n",pw->pw_dir);
    printf("Shell usada: %s,\n",pw->pw_shell);
}