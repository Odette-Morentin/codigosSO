#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <errno.h>

int main(int argc, char *argv[]) {
   DIR *dir_ptr;
   struct dirent *entry;

   if (argc != 2) {   /* Comprobar el número de argumentos */
      fprintf(stderr, "Uso: %s <ruta_directorio>\n", argv[0]);
      return 1;
   }

   const char *directorio = argv[1];

   dir_ptr = opendir(directorio);  /* Abrir directorio */

   if (dir_ptr == NULL) {
      perror("Error al abrir el directorio");
      return 1;
   }

   printf("Contenido del directorio '%s':\n", directorio);
   printf("------------------------------------\n");

   while ((entry = readdir(dir_ptr)) != NULL) { /* Recorrer el directorio*/
      printf("%s\n", entry->d_name);
   }

   if (closedir(dir_ptr) == -1) {
      perror("Error al cerrar el directorio");
      return 1;
   }

   return 0;
}