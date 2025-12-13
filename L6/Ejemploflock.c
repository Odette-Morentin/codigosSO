struct flock {
    short l_type;    /* Tipo de cerrojo: F_RDLCK, F_WRLCK, F_UNLCK.
     F_RDLCK para un cerrojo de lectura, F_WRLCK para un cerrojo de escritura, y F_UNLCK que elimina un cerrojo.*/

    short l_whence;  /* Interpretar l_start: SEEK_SET, SEEK_CURR, SEEK_END 
    SEEK_SET para el inicio del archivo, SEEK_CUR para la posición actual y SEEK_END para el final del archivo*/

    off_t l_start;   /* Desplazamiento donde se inicia el bloqueo 
    Rango de bytes*/

    off_t l_len;     /* Numero bytes bloqueados: 0 significa “hasta EOF” 
    Rango de bytes*/

    pid_t l_pid;     /* Proceso que previene nuestro bloqueo (solo F_GETLK) */
};

struct flock mi_bloqueo;
... /* ajustar campos de mi_bloqueo para describir el cerrojo a usar */
fcntl(fd, orden, &mi_bloqueo);