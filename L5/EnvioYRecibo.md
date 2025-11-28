envioSignal.c manda señales


reciboSignal.c las recibe y actúa


Ambos se comunican solo mediante señales POSIX


Usan las mismas señales (SIGUSR1, SIGUSR2, SIGTERM)


Ambos usan la biblioteca <signal.h>


Ambos trabajan con PIDs


Ambos forman un sistema de IPC basado en señales


Primero se ejecuta recibo señal y después envío señal