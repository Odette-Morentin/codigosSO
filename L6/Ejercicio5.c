#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd_src, fd_dst;
    struct stat sb;
    void *src_map, *dst_map;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <origen> <destino>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Abrir archivo origen */
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
        perror("open origen");
        return EXIT_FAILURE;
    }

    /* Obtener tamaño del archivo origen */
    if (fstat(fd_src, &sb) < 0) {
        perror("fstat");
        close(fd_src);
        return EXIT_FAILURE;
    }

    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "El archivo origen no es regular\n");
        close(fd_src);
        return EXIT_FAILURE;
    }

    /* Abrir / crear archivo destino */
    fd_dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0) {
        perror("open destino");
        close(fd_src);
        return EXIT_FAILURE;
    }

    /* Ajustar tamaño del destino */
    if (ftruncate(fd_dst, sb.st_size) < 0) {
        perror("ftruncate");
        close(fd_src);
        close(fd_dst);
        return EXIT_FAILURE;
    }

    /* Mapear archivo origen */
    src_map = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd_src, 0);
    if (src_map == MAP_FAILED) {
        perror("mmap origen");
        close(fd_src);
        close(fd_dst);
        return EXIT_FAILURE;
    }

    /* Mapear archivo destino */
    dst_map = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, fd_dst, 0);
    if (dst_map == MAP_FAILED) {
        perror("mmap destino");
        munmap(src_map, sb.st_size);
        close(fd_src);
        close(fd_dst);
        return EXIT_FAILURE;
    }

    /* Copiar contenido */
    memcpy(dst_map, src_map, sb.st_size);

    /* Sincronizar cambios */
    msync(dst_map, sb.st_size, MS_SYNC);

    /* Liberar recursos */
    munmap(src_map, sb.st_size);
    munmap(dst_map, sb.st_size);
    close(fd_src);
    close(fd_dst);

    return EXIT_SUCCESS;
}
