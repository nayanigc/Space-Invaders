#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#ifndef LOAD_H
#define LOAD_H
//structure vaisseau 

typedef struct{
	int l; 
	int h;
	int longueur_cycle;
	int* deplacement_horizontal ;
	int* deplacement_vertical;
	int nombre_vie;
	float frequence_tir;
	int vitesse_tir;// en cases ou en seconde
	int puissance_tir;
	char symbole_tir; // ex :"."
	char** dessin;

}vaisseau_type;
// apparition de vaisseau 

typedef struct {
	int type;
	int temps;
	int pos_x;
	int pos_y;
	
}app_vaisseau;

//structure d'un niveau 
typedef struct {
	int nombre_vaisseaux;
	int nombre_app;
	app_vaisseau* apps;
}niveau;

int split_on_string(char* s, char* lines[], char* split);
int load_niveau (char* filename, niveau* n_to_load);
void print_niveau(niveau* niv);
void deleted_niveau(niveau* niv);
void create_niveau(niveau* niv);
void delete_vaisseau(vaisseau_type* v);
void print_vaisseau(vaisseau_type* v);
int load_vaisseau(char* filename, vaisseau_type* v_to_load);

#endif