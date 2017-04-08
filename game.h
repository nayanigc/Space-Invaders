#ifndef GAME_H
#define GAME_H 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>

#include "load.h"
#include "lecture.h"

#define PLATEAU_LARGEUR 200
#define PLATEAU_HAUTEUR 200

typedef struct {
	vaisseau_type type;
	int pos_x;
	int pos_y;
	int nombre_vie;
} vaisseau;

typedef struct {
 char cases[PLATEAU_HAUTEUR][PLATEAU_LARGEUR];
 int nombre_vaisseaux;
 vaisseau* vaisseaux;
}plateau;

int play(niveau* niv, mod* mod);

#endif