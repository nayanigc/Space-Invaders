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
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>

#include "load.h"
#include "menu.h"


#define PLATEAU_LARGEUR 100
#define PLATEAU_HAUTEUR 30

#define SECONDE 1000000

typedef enum {
	HAUT,
	BAS,
	GAUCHE,
	DROITE
} direction;

typedef struct {
	vaisseau_type type;
	int pos_x;
	int pos_y;
	int nombre_vie;
	int cursor_cycle;
	long last_shot; //in µs
} vaisseau;

typedef struct {
	char symbole;
	int pos_x;
	int pos_y;
	int vitesse;
	long last_move; //in µs
	int puissance;
	direction dir;
} shot;


typedef enum {
	VIDE,
	ENNEMI,
	ME,
	SHOT
} case_contenu;


typedef struct {
 char cases[PLATEAU_HAUTEUR][PLATEAU_LARGEUR];
 case_contenu collision_cases[PLATEAU_HAUTEUR][PLATEAU_LARGEUR];
 int nombre_vaisseaux;
 vaisseau* vaisseaux;
 vaisseau my_vaisseau;
 int nombre_shots;
 shot* shots;
} plateau;


void init_my_vaisseau(plateau* plat, vaisseau_type* type);
void add_apparition(mod* mod, niveau* niv, plateau* plat, int time);
void update_positions(plateau* plat);
void update_positions_shots(plateau* plat);
bool handle_shot_collision(plateau* plat, vaisseau* v, int pos_y, int pos_x);
void remplir_plateau_shot(plateau* plat, shot* s);
bool remplir_plateau_vaisseau(plateau* plat, vaisseau* v, case_contenu contenu);
bool remplir_plateau(plateau* plat);
void affiche_plateau(plateau* plat);
long get_current_time();
bool remplir_affiche(plateau* plat);
bool play(mod* mod, niveau* niv);
void delete_plat(plateau* plat);

#endif