/*
 * reciboSignal.c
 * Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
 * Utilización de la llamada sigaction para cambiar el comportamiento del proceso
 * frente a la recepción de una señal.
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
