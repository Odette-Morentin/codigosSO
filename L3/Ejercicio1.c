#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * Programa que crea un proceso hijo para verificar paridad
 * y donde el proceso padre verifica divisibilidad por 4.
 * Uso: ./checker <numero_entero>
 */
int main(int argc, char *argv[]) {
    int numero;
    pid_t pid;
    int estado; // Para capturar el estado del hijo

    // 1. Verificación de argumentos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero_entero>\n", argv[0]);
        return 1;
    }

    // 2. Conversión del argumento a entero
    // strtol se usa para una conversión robusta
    numero = (int)strtol(argv[1], NULL, 10);

    printf("--- Iniciando comprobaciones para el número: %d ---\n", numero);

    // 3. Creación del proceso hijo
    pid = fork();

    if (pid < 0) {
        // Error al crear el proceso
        perror("Error al crear el proceso hijo (fork)");
        return 1;
    } 
    
    // --- 4. Bloque de código del PROCESO HIJO (pid == 0) ---
    else if (pid == 0) {
        printf("[Hijo (PID %d)] Comprobando paridad...\n", getpid());
        
        // Comprobar si es par o impar (divisible por 2)
        if (numero % 2 == 0) {
            printf("[Hijo (PID %d)]: El número %d es PAR.\n", getpid(), numero);
        } else {
            printf("[Hijo (PID %d)]: El número %d es IMPAR.\n", getpid(), numero);
        }
        
        // El hijo termina su ejecución
        exit(0);
    } 
    
    // --- 5. Bloque de código del PROCESO PADRE (pid > 0) ---
    else {
        printf("[Padre (PID %d)] Comprobando divisibilidad por 4...\n", getpid());
        
        // Comprobar si es divisible por 4
        if (numero % 4 == 0) {
            printf("[Padre (PID %d)]: El número %d es DIVISIBLE por 4.\n", getpid(), numero);
        } else {
            printf("[Padre (PID %d)]: El número %d NO es divisible por 4.\n", getpid(), numero);
        }
        
        // Esperar a que el proceso hijo termine
        if (waitpid(pid, &estado, 0) == -1) {
            perror("Error esperando al hijo (waitpid)");
        } else {
            printf("[Padre (PID %d)]: Proceso hijo (PID %d) terminado.\n", getpid(), pid);
        }
    }

    return 0;
}