/*
  envioSignal.c
  Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
  Utilización de la llamada kill para enviar una señal:
  0: SIGTERM
  1: SIGUSR1
  2: SIGUSR2
  a un proceso cuyo identificador de proceso es PID.
  SINTAXIS: envioSignal [012] <PID>
*/

#include <sys/types.h>   /* Tipos de datos POSIX (pid_t, etc.) */
#include <limits.h>      /* Constantes como LONG_MIN y LONG_MAX */
#include <unistd.h>      /* Funciones POSIX como getpid(), fork(), etc. */
#include <sys/stat.h>
#include <stdio.h>       /* Entrada/Salida estándar */
#include <stdlib.h>      /* Conversión de tipos, gestión de memoria, exit() */
#include <signal.h>      /* Definición de señales: SIGTERM, SIGUSR1, SIGUSR2 */
#include <errno.h>       /* Manejo de errores (errno) */

int main(int argc, char *argv[])
{
   long int pid;  /* Variable para guardar el PID convertido de texto */
   int signal;    /* Variable para almacenar la señal elegida */

   /* Verificación del número de argumentos */
   if(argc < 3) {
      printf("\nSintaxis de ejecución: envioSignal [012] <PID>\n\n");
      exit(-1);
   }

   /* Conversión del PID desde el argumento argv[2] */
   pid = strtol(argv[2], NULL, 10);

   /* Comprobación de errores en la conversión (desbordamiento) */
   if(pid == LONG_MIN || pid == LONG_MAX)
   {
      if(pid == LONG_MIN)
         printf("\nError: desbordamiento inferior LONG_MIN %ld", pid);
      else
         printf("\nError: desbordamiento superior LONG_MAX %ld", pid);

      perror("\nError en strtol");
      exit(-1);
   }

   /* Conversión del tipo de señal (0,1,2) desde el argumento argv[1] */
   signal = atoi(argv[1]);

   /* Selección de la señal a enviar según el argumento */
   switch(signal) {
      case 0: /* SIGTERM */
         kill(pid, SIGTERM);
         break;

      case 1: /* SIGUSR1 */
         kill(pid, SIGUSR1);
         break;

      case 2: /* SIGUSR2 */
         kill(pid, SIGUSR2);
         break;

      default: /* Si el valor no está entre 0 y 2 */
         printf("\nNo puedo enviar ese tipo de señal");
   }

   return 0;
}
