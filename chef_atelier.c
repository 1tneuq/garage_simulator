#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ipc.h"
#include "types.h"

int main(int argc, char** argv) {
    int i, file_id, numero_ordre, nb_outils[4], *niveau_occupation;
    requete_t* requete;
    reponse_t* reponse;

    if (argc < 6) {
        printf("USAGE : %s <numero_ordre> <nb_1> <nb_2> <nb_3> <nb_4>", argv[0]);
        return 1;
    }

    numero_ordre = strtol(argv[1], NULL, 10);
    for (i=0; i<4; i++) {
        nb_outils[i] = strtol(argv[2+i], NULL, 10);
    }

    requete = (requete_t*)malloc(sizeof(requete_t));
    reponse = (reponse_t*)malloc(sizeof(reponse_t));

    sleep(1);

    /* Génération du seed pour rand() */
    srand(getpid());

    /* Récupération IPC */
    printf("[CHEF %d] Récupération IPC\n", numero_ordre);
    file_id = get_file(FICHIER_CLE, 1337);

    for (;;) {
        /* Préparation de la requête */
        requete->type = 1;
        requete->numero_ordre = numero_ordre;
        for (i=0; i<4; i++) {
            requete->nb_outils_categorie[i] = (rand()%nb_outils[i]) + 1;
        } 

        /* Attente d'un client */
        printf("[CHEF %d] ATTENTE D'UN CLIENT\n", numero_ordre);

        /* Envoi du travail aux mécaniciens */
        requete->duree = rand()%3+1;
        printf("[CHEF %d] DEMANDE DE TRAVAIL DE %d SECONDES ENVOYEE\n", numero_ordre, requete->duree);
        msgsnd(file_id, requete, sizeof(requete_t) - sizeof(long), 0);

        sleep(requete->duree);
        while (1) {

            //printf("[CHEF %d] size struct %d size reponse %d\n", numero_ordre, sizeof(reponse_t), sizeof(reponse));
            if (msgrcv(file_id, reponse, sizeof(reponse_t) - sizeof(long), numero_ordre, 0) != -1) {

                printf("[CHEF %d] TRAVAIL RECU\n", numero_ordre);

                /* Notification au client */

                break;
            }
            else {
                perror("msgrcv");
                //printf("reponse = %d, %d\n", reponse.type, reponse.numero_ordre);
                //printf("[CHEF %d] msgrcv err\n", numero_ordre);
                sleep(3);
            }
        }
        reponse->type = 0;

    }

    printf("[CHEF %d] FIN\n", numero_ordre);
    return 0;
}