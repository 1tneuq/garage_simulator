#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/types.h>
#include <assert.h>
#include "proc.h"
#include "ipc.h"

/* ID de la file */
int file_msg;

void arret(int signal_id) {
    /* ARRET 
    SUPRESSION DES PROCESSUS ET IPC
    */
   int i;
   int sid=semget(ftok(FICHIER_CLE, 1337), 4, 0);
   for (i = 0; i<4; i++) {
       semctl(sid, 0, IPC_RMID);
   }

   if (msgctl(file_msg, IPC_RMID, NULL) == -1) {
       perror("supp ipc");
   }
   printf("Arret main\n");

   exit(0);
}

int main(int argc, char** argv) {
    /* clé de la file */
    key_t cle;
    /* Chaque indice du tableau nb_outils représente une catégorie */
    int nb_chefs, nb_mecaniciens, nb_outils[4];

    int i;

    printf("PID MAIN %d\n", getpid());

    if (argc < 7) {
        printf("USAGE: %s <nb_chefs> <nb_mecaniciens> <nb_1> <nb_2> <nb_3> <nb_4>\n", argv[0]);
        return EXIT_FAILURE;
    }


    printf("[MAIN] Démarrage du garage\n");

    cle = ftok(FICHIER_CLE, 1337);

    /* On attribue les valeurs données en argument */
    nb_chefs = strtol(argv[1], NULL, 10);
    nb_mecaniciens = strtol(argv[2], NULL, 10);
    for (i=0; i<4; i++) {
        nb_outils[i] = strtol(argv[3+i], NULL, 10);
    }

    printf("[MAIN] Redirection du signal\n");

    /* Redirection du signal SIGINT vers la fonction arret */
    if (set_signal_handler(SIGINT, arret) == -1) {
        perror("ERREUR SET SIGNAL");
    }

    printf("[MAIN] Création de la file de messages méca/chefs\n");

    if ((file_msg=creer_file(FICHIER_CLE, 1337)) < 0) {
        perror("creer file");
    }

    printf("[MAIN] Création des sémaphores pour les outils\n");

    if (creer_semaphore_outils(FICHIER_CLE, 1337, 4, nb_outils) < 0) {
        perror("creer sem");
    }

    printf("[MAIN] Création du segment de mémoire partagé\n");

    if (creer_segment_memoire(cle, nb_chefs*sizeof(int)) < 0) {
        perror("creer segment mémoire");
    }

    printf("[MAIN] Génération des mécaniciens...\n");

    /* Génération de i mécaniciens */
    for (i = 0; i<nb_mecaniciens; i++) {
        generer_mecanicien(i);
    }

    printf("[MAIN] Génération des chefs de projet...\n");

    /* Génération de i chefs d'atelier */
    for (i = 0; i<nb_chefs; i++) {
        generer_chef_atelier(i, nb_outils);
    }

    /* Génération des clients */
    for(;;) {
        
    }

    return EXIT_SUCCESS;
}