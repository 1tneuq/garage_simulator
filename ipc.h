#ifndef IPC_H
#define IPC_H

#define FICHIER_CLE "cle.key"

#define ERREUR_GENERATION_CLE -1
#define ERREUR_CREATION_FILE -2
#define ERREUR_RECEPTION_FILE -3
#define ERREUR_CREATION_SEMAPHORE_OUTILS -4
#define ERREUR_CREATION_SEGMENT -5
#define ERREUR_RECEPTION_SEGMENT -6

#include <sys/types.h>

int creer_file(char* fichier_cle, int id_projet);
int get_file(char* fichier_cle, int id_projet);

int creer_semaphore_outils(char* fichier_cle, int id_projet, int nb_categories_outils, int* nb_outils_categorie);
int get_semaphores_outils(char* fichier_cle, int id_projet, int nb_categories_outils);

int creer_segment_memoire(key_t cle, size_t taille);
int get_segment_memoire(key_t cle, size_t taille);

#endif