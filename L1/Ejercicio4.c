//Prueba tu macro con este código:

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define S_ISREG2(mode) /* aquí el código de tu macro */

int main(int argc, char *argv[]) {
   if (argc != 2) {
      fprintf(stderr, "Uso: %s <directorio>\n", argv[0]);
      return 1;
   }

   DIR *dir = opendir(argv[1]);
   if (!dir) {
      perror("opendir");
      return 1;
   }

   struct dirent *entry;
   struct stat st;
   char path[1024];

   while ((entry = readdir(dir)) != NULL) {
      snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);
      if (stat(path, &st) == 0) {
         if (S_ISREG2(st.st_mode)) {
            printf("%s es un archivo regular\n", entry->d_name);
         } else {
            printf("%s NO es un archivo regular\n", entry->d_name);
         }
      } else {
         perror("stat");
      }
   }

   closedir(dir);
   return 0;
}