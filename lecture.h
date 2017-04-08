#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "load.h"

//DEFINITION D'UNE LISTE pour stocket les éléments d'un repertoire
typedef struct list_t {
	char *name;
	int is_dir;
	struct list_t *next;
}list_t;

typedef struct {
	//le nom du mod
	char* name;

	//Le nombre de types de vaisseaux (taille du tableau 'types')
	int nb_vaisseau_types;
	//Le tableau contenant l'ensemble des types de vaisseaux
	vaisseau_type* types;

	//Le nombre de niveaux (taille du tabelau 'niveau')
	int nb_niveau;
	//Le tableau contenant tous les niveaux du mod
	niveau* niveaux;

	//TODO Ajouter des scores pour le mods ou des stats (à définir)
} mod;

int recursive_dir (char *root, char *mods);

#endif