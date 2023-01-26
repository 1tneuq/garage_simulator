#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "proc.h"

int set_signal_handler(int signo, void (*handler)(int)) {
	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0 ;   
	return sigaction(signo, &sa, NULL);
}

pid_t generer_chef_atelier(int numero_ordre, int* nb_outils) {
    pid_t pid;
	int i;

	// Buffer contenant le numero d'ordre sous forme de string
	char num_buffer[10];

	// Buffer contenant les numéros d'outils sous forme de string
	char num_outils_buffer[4][10];

    switch((pid = fork())) {
		case -1:
			perror("erreur fork chef atelier");
			break;
		case 0:
			snprintf(num_buffer, sizeof(num_buffer), "%d", numero_ordre);

			for (i = 0; i<4; i++) {
				snprintf(num_outils_buffer[i], sizeof(num_outils_buffer[i]), "%d", nb_outils[i]);
			}

			if ((execl("./chef_atelier", "./chef_atelier", num_buffer, num_outils_buffer[0], num_outils_buffer[1], num_outils_buffer[2], num_outils_buffer[3], NULL)) == -1) {
				perror("exec chef atelier");
			}
			break;
		default:
			break;
	}

    return pid;
}

pid_t generer_client(int nb_chefs, key_t* cles) {
    pid_t pid;

	////////// TODO CONNAITRE NB DE CLES

    switch((pid = fork())) {
		case -1:
			perror("erreur fork client");
			break;
		case 0:
			if ((execl("./client", "./client", cles[0], NULL)) == -1) {
				perror("exec client");
			}
			break;
		default:
			break;
	}

    return pid;
}

pid_t generer_mecanicien(int numero_ordre) {
    pid_t pid;
	// Buffer contenant le numero d'ordre sous forme de string
	char num_buffer[10];

    switch((pid = fork())) {
		case -1:
			perror("erreur fork client");
			break;
		case 0:
			snprintf(num_buffer, sizeof(num_buffer), "%d", numero_ordre);
			if ((execl("./mecanicien", "./mecanicien", num_buffer, NULL)) == -1) {
				printf("erreur exec mecanicien %d\n", numero_ordre);
				exit(1);
			}
			break;
		default:
			break;
	}

    return pid;
}