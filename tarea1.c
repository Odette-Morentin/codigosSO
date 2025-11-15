/*
tarea1.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
Probar tras la ejecuci�n del programa: $>cat archivo y $> od -c archivo
*/

#include<sys/types.h>  	//off
						//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<sys/stat.h> // permisos
#include<fcntl.h> //open
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h> //Read, write, lseek, close

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";

int main(int argc, char *argv[]) //arg cantidad de argumentos usados en la linea de comandos
{
int fd; //archivo

if( (fd=open("archivo",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) { //crea o vacia o da permisos de escritura | permiso de escritura y lectura para el dueño 
	printf("\nError %d en open",errno);
	perror("\nError en open");
	exit(-1);
}
if(write(fd,buf1,10) != 10) { //numero de archivo (identificadorr), lo que se va a escribir, cantidad de bytes asignados
	perror("\nError en primer write");
	exit(-1);
}
	//posiciona el puntero. Se brinca 40 bytes
if(lseek(fd,40,SEEK_SET) < 0) { //numero de archivo (identificador), cuánto desplaza el puntero, lo mueve desde el inicio
	perror("\nError en lseek");
	exit(-1);
}

	//vuelve a escribir en el archivo
if(write(fd,buf2,10) != 10) { //numero de archivo (identificador), texto que escribe, cantidad dedicada de bytes
	perror("\nError en segundo write");
	exit(-1);
}

return 0;
}
