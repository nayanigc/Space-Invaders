#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include "game.h"
#include "load.h"
#include "menu.h"

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

	/*mod m;
	load_mod("mod1", &m);
	*/
	lauch_menu();

	//play(&m, &m.niveaux[0]);

	if(restaurer_mode_canonique(&term) == -1) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}






