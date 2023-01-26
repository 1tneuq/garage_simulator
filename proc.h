#ifndef PROC_H
#define PROC_H

#include <sys/types.h>

int set_signal_handler(int signo, void (*handler)(int));
pid_t generer_chef_atelier(int numero_ordre, int* nb_outils);
pid_t generer_mecanicien(int numero_ordre);
pid_t generer_client(int nb_chefs, key_t* cles);

#endif