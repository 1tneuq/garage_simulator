#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include "ipc.h"


int creer_file(char* fichier_cle, int id_projet) {
    int id_file;
    key_t k;

    if ((k=ftok(fichier_cle, id_projet)) == -1) {
        return ERREUR_GENERATION_CLE;
    }

    // Génération de la file de messages
    if ((id_file=msgget(k, IPC_CREAT | 0600)) == -1) {
        return ERREUR_CREATION_FILE;
    }

    return id_file;
}

int get_file(char* fichier_cle, int id_projet) {
    int id_file;
    key_t k;

    if ((k=ftok(fichier_cle, id_projet)) == -1) {
        return ERREUR_GENERATION_CLE;
    }

    // Génération de la file de messages
    if ((id_file=msgget(k, 0)) == -1) {
        return ERREUR_RECEPTION_FILE;
    }

    return id_file;
}

/*
nb_categories_outils le nombre de catégories d'outils disponibles
nb_outils_categorie le nombre d'outils de chaque catégorie disponibles
*/
int creer_semaphore_outils(char* fichier_cle, int id_projet, int nb_categories_outils, int* nb_outils_categorie) {
    int sid, i;

    /* Si la création échoue */
    if ((sid = semget(ftok(fichier_cle, id_projet), nb_categories_outils, 0666|IPC_CREAT)) == -1) {
        return ERREUR_CREATION_SEMAPHORE_OUTILS;
    }
	
    for (i = 0; i<nb_categories_outils; i++) {
        /* On initialise les semaphores avec le nombre d'outils correspondant */
        semctl(sid, i, SETVAL, nb_outils_categorie[i]);
    }

    return sid;
}

int get_semaphores_outils(char* fichier_cle, int id_projet, int nb_categories_outils) {
    int sid;

    /* Si la réception échoue */
    if ((sid = semget(ftok(fichier_cle, id_projet), nb_categories_outils, 0)) == -1) {
        return ERREUR_CREATION_SEMAPHORE_OUTILS;
    }

    return sid;
}

int creer_segment_memoire(key_t cle, size_t taille) {
    int shmid;

    if ((shmid = shmget(cle, taille, IPC_CREAT|0666)) == -1) {
        return ERREUR_CREATION_SEGMENT;
    }

    return shmid;
}

int get_segment_memoire(key_t cle, size_t taille) {
    int shmid;

    if ((shmid = shmget(cle, taille, 0)) == -1) {
        return ERREUR_RECEPTION_SEGMENT;
    }

    return shmid;
}