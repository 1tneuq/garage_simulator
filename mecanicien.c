#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include "ipc.h"

int is_working = 0;

int main(int argc, char** argv) {
    /* ID de la file entre chefs et mecaniciens */
    int file_id, numero_ordre, sid, i;
    struct sembuf p[4], v[4];
    requete_t requete;
    reponse_t* reponse;

    if (argc < 2) {
        printf("USAGE: %s <numero_ordre>\n", argv[0]);
        exit(1);
    }

    if ((file_id = get_file(FICHIER_CLE, 1337)) < 0) {
        perror("get_file");
    }

    reponse = (reponse_t*)malloc(sizeof(reponse_t));

    numero_ordre = strtol(argv[1], NULL, 10);

    /* On inscrit le numéro du mécanicien dans la réponse pour que les chefs sachent de quel mécanicien il s'agit */
    reponse->type = 1;
    reponse->numero_ordre = numero_ordre;

    printf("[MECA %d] Récupération des sémaphores outils\n", numero_ordre);

    if ((sid = get_semaphores_outils(FICHIER_CLE, 1337, 4)) < 0) {
        perror("get semaphores outils");
    }

    printf("[MECA %d] ATTENTE DE TRAVAIL\n", numero_ordre);
    for (;;) {
        /* Réception du travail à faire */
        msgrcv(file_id, &requete, sizeof(requete_t) - sizeof(long), requete.type, 0);
    
        /* Si le type de requête est = à 1 */
        if (requete.type == 1) {
            /* Attribution du numero d'ordre du chef pour envoyer la réponse au chef dont vient la demande */
            reponse->type = requete.numero_ordre;
            //printf("[MECA %d] ADDR REPONSE %X\n", numero_ordre, reponse);
            //printf("[MECA %d] reponse->type %d reponse->numero_ordre %d\n", numero_ordre, reponse->type, reponse->numero_ordre);

            /* Réservation des outils */
            for (i = 0; i<4; i++) {
                p[i].sem_num = i;
                p[i].sem_op = requete.nb_outils_categorie[i] * (-1);
                p[i].sem_flg = 0;
            }
            semop(sid, p, 4);

            printf("[MECA %d] RESERVATION de [%d--%d--%d--%d] outils\n", numero_ordre,
            requete.nb_outils_categorie[0],
            requete.nb_outils_categorie[1],
            requete.nb_outils_categorie[2],
            requete.nb_outils_categorie[3]);


            /* Execution du travail */
            is_working = 1;
            printf("[MECA %d] DUREE DU TRAVAIL : %d SECONDES\n", numero_ordre, requete.duree);
            sleep(requete.duree);


            /* Remise à disposition des outils */
            for (i = 0; i<4; i++) {
                v[i].sem_num = i;
                v[i].sem_op = requete.nb_outils_categorie[i];
                v[i].sem_flg = 0;
            }
            semop(sid, v, 4);

            printf("[MECA %d] REMISE A DISPOSITOON de [%d--%d--%d--%d] outils\n", numero_ordre,
            requete.nb_outils_categorie[0],
            requete.nb_outils_categorie[1],
            requete.nb_outils_categorie[2],
            requete.nb_outils_categorie[3]);

            //printf("[MECA %d] reponse->type %d reponse->numero_ordre %d\n", numero_ordre, reponse->type, reponse->numero_ordre);


            /* Notifier les chefs d'atelier de la fin du traitement de la requete */
            if (msgsnd(file_id, &reponse, sizeof(reponse_t) - sizeof(long), 0) == -1) {
                printf("[MECA %d] MSGSND ERR\n", numero_ordre);
                perror("msgsnd");
            }
        }
        

        is_working = 0;
    }

    printf("[CHEF %d] FIN\n", numero_ordre);

    return 0;
} 