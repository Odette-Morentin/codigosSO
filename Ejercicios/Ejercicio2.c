/*
Programa en c que copie los archives regulares 
con inodos impares, se pasa como primer argumento
el directorio origen, deberá recorrer la jerarquía de subdirectorios
indicando la profundidad deseada como argumento.

Si no existe el directorio deberá crearse. Deberá crear un archivo que
añada una linea donde indica el numero total de archivos copiados
y la fecha actual. Añada una linea por cada uno de los archivos que se han copeado,
debiendo incorporar también la ruta del archivo donde se encuntra.
*/ 

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int cont = 0;

void recorrer_dir(cons chat *origin, int depth, cons chat *dest, const char *path actual){
    DIR *d = opendir(origin);
    if (!d){
        perror("opendir");
        return;
    }

    struct dirent *actual;

    while ((actual = readdir(d)) != NULL){
        if (strcmp(actual->d_name,".")!=0 && strcmp(actual->d_name,"..") != 0)
        {
            char ruta[1024];
            snprintf(ruta,sizeof(ruta),"%s/%s",orig, actual->d_name);

            struct stat atrib;
            if(stat(ruta.&atrib) == -1){
                perror("stat");
            }

            if(S_ISREG(atrib.st_mode)){
                if (atrib.st_ino % 2 != 0)
                {
                    int fd_src = open(ruta,O_RDONLY);
                    if (fd_src < 0)
                    {
                        perror("open src");
                    }

                    char ruta_dest[1024];
                    snprintf(ruta_dest, sizeof(ruta_dest), "%s/%s", dest, actual -> d_name);
                    int fd_dst = open(ruta_dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);

                    if (fd_dst < 0)
                    {
                        perror("open dst");
                        close(fd_src);
                    }

                    char buffer [1024];
                    int n;

                    while((n = read(fd_src, buffer, sizeof(buffer))) > 0)
                    {
                        if (write(fd_dst, buffer, n) =! n)
                        {
                            perror("write");
                            break;
                        }
                    }

                    if (n<0)
                    {
                        perror("read");
                    }
                    close(fd_src);
                    close(fd_dst);

                    cont ++;
                }
            }
            else if (S_ISDIR(atrib.st_mode))
            {
                if (depth > 0)
                {
                    recorrer_dir(ruta, depth -1, dest, ruta);
                }  
            }
            
        }
        
    }
    closedir(d);
}

int main( int argc, char *argv){
    if (argc < 4)
    {
        fprintf(stderr,"Uso: ")
    }
    exit(1);

    char *orig = argv[1];
    int prof = atoi(argv[2]);
    char *dest = argv[3];

    struct stat st;
    if (stat(dest,&st) < 0)
    {
        if (errno == ENOENT)
        {
            if(mkdir(dest,0777) < 0){
                perror("mkdir");
                exit(1);
            }
        } else{
            perror("stat");
            exit(1);
        }
    }

    int fd = open("fichero.txt", O_TRUNC | O_CREAT | O_RDWR, 0666);
    if (fd <0)
    {
        perror("open fichero");
        exit(-1);
    }

    recorrer_dir(orig, prof, dest, orig);

    lseek(fd, 0 , SEER_SET);

    time_tt = time(NULL);
    struct *tm_info = localtime(&t);
    char fecha[32];
    strftime(fecha, sizeof(fecha), "%d-%m-%Y", tm_info);

    char mensaje[256];
    int len = snprintf(mensaje, sizeof(mensjae), "Ficheros totales copiados: %d %s\n", cont, fecha);
    if (write(fd,mensaje, len) != len)
    {
        perror("Write cabecera");
    }

    close(fd);
    return 0;
}