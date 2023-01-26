#ifndef TYPES_H
#define TYPES_H

typedef struct 
{
    long type;
    int numero_ordre;
	int duree;
    int nb_outils_categorie[4];
} requete_t;

typedef struct {
    long type;
    unsigned short numero_ordre;
} reponse_t;

#endif