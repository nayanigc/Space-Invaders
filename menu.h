#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <poll.h>
#include <stdbool.h>

#include "load.h"

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

void load_menu(char* mods[]);
void display_menu(char* mods[], char* current_mod);
void lauch_menu();
void play_mod(mod* mod);
void load_mod(char* name, mod* mod);

//game. a besoin de la structure mod, on ne peut pas l'inclure au début
#include "game.h"

#endif
