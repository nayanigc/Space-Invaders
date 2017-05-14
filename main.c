#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include "game.h"
#include "load.h"

int mode_non_canonique(struct termios* old_term){
	struct termios new_term;
	if(tcgetattr(fileno(stdin), old_term) == -1){
		perror("Erreur: impossible de passer en mode non canonique");
		return EXIT_FAILURE;	
	}
	new_term.c_iflag = old_term->c_iflag;
	new_term.c_oflag = old_term->c_oflag;
	new_term.c_cflag = old_term->c_cflag;
	new_term.c_lflag = 0;
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
	if(tcsetattr(fileno(stdin), TCSANOW, &new_term) == -1){
		perror("Erreur: impossible de passer en mode non canonique");
		return EXIT_FAILURE;	
	}
	return EXIT_SUCCESS;
}

int restaurer_mode_canonique(struct termios* term){
	if(tcsetattr(fileno(stdin), TCSANOW, term) == -1){
		perror("Erreur: impossible de restaurer le mode canonique");
		return EXIT_FAILURE;	
	}
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
	struct termios term;
	if(mode_non_canonique(&term) == -1) return EXIT_FAILURE;

	niveau n0;
	load_niveau("mods/mod1/niveaux/0", &n0);
	//print_niveau(&n0);
	
	vaisseau_type v0;
	load_vaisseau("mods/mod1/niveaux/vaisseaux/0", &v0);
	//print_vaisseau(&v0);
	
	mod* m;

	m = malloc(sizeof(mod));

	m->nb_vaisseau_types = 1;
	m->types = malloc(1 * sizeof(vaisseau_type));
	m->types[0] = v0;

	m->nb_niveau = 1;
	m->niveaux = malloc(1 * sizeof(niveau));
	m->niveaux[0] = n0;


	play(m, &n0);

	if(restaurer_mode_canonique(&term) == -1) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}






