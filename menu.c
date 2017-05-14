#include "menu.h"

void load_menu(char* mods[]){

	int compteur = 0;

	DIR* dir = opendir("mods");
	chdir("mods");

	struct dirent* d;
	while((d = readdir(dir)) != NULL){
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue; 

		struct stat s;
		lstat(d->d_name, &s);
		if(S_ISDIR(s.st_mode)){
			mods[compteur++] = d->d_name;
		}
	}
	mods[compteur] = NULL;

	chdir("..");
}

void display_menu(char* mods[], char* current_mod){
	char buffer[1024];
	int k = 0;
	int i = 0;
	int j = 0;
	for(int l = 0; l < 30; l++) buffer[k++] = '\n';
	while(mods[i] != NULL){
		j = 0;
		if(strcmp(mods[i], current_mod) == 0){
			buffer[k++] = '-';
			buffer[k++] = '>';	
		} else {
			buffer[k++] = ' ';
			buffer[k++] = ' ';
		}
		buffer[k++] = ' ';
		while(mods[i][j] != '\0'){
			buffer[k++] = mods[i][j];
			j++;
		}
		buffer[k++] = '\n';
		buffer[k++] = '\n';
		i++;
	}

	write(STDOUT_FILENO, buffer, k);
}

void lauch_menu(){
	char* mods[100];
	load_menu(mods);

	int cursor = 0;
	int nb_mod = 0;
	while(mods[nb_mod] != NULL){
		nb_mod++;
	}

	char buffer[1024];
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events=POLLIN;

	display_menu(mods, mods[cursor]);
	while(true){
		int res = poll(fds, 1, -1);
		if (res>0){
			if (fds[0].revents & POLLIN){
				//On récupère la touche entrée
				int nread = read(fds[0].fd, buffer, 1024);
				if(nread > 0){
					if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 65){
						if(cursor > 0) cursor--;
						display_menu(mods, mods[cursor]);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 66){
						if(cursor < nb_mod - 1) cursor++;
						display_menu(mods, mods[cursor]);
					} else if(buffer[0] == '\n'){
						mod m;
						load_mod(mods[cursor], &m);
						play_mod(&m);
						break;
					} else if(buffer[0] == 113){
						//La touche est 'q' on quitte le niveau
						break;
					}
				}
			}
		}
	}
}

void play_mod(mod* mod){
	bool mod_victory = true;
	bool victory = false;

	for(int i = 0; i < mod->nb_niveau; i++){
		victory = play(mod, &mod->niveaux[i]);
		if(!victory){
			mod_victory = false;
			break;
		}
	}

	if(mod_victory){
		write(STDOUT_FILENO, "MOD WIN !!!\n", 13);
	} else {
		write(STDOUT_FILENO, "MOD LOOSE ...\n", 15);
	}	
}

void load_mod(char* name, mod* mod){
	mod->name = name;
	mod->nb_vaisseau_types = 0;
	mod->types = malloc(50 * sizeof(vaisseau_type));
	mod->nb_niveau = 0;
	mod->niveaux = malloc(50 * sizeof(niveau));

	chdir("mods");
	chdir(name);

	DIR* dir = opendir("niveaux");
	chdir("niveaux");

	struct dirent* d;
	while((d = readdir(dir)) != NULL){
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue; 

		struct stat s;
		lstat(d->d_name, &s);
		if(S_ISREG(s.st_mode)){
			niveau n;
			load_niveau(d->d_name, &n);
			mod->niveaux[mod->nb_niveau] = n;
			mod->nb_niveau++;
		}
	}
	closedir(dir);

	dir = opendir("vaisseaux");
	chdir("vaisseaux");
	
	while((d = readdir(dir)) != NULL){
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue; 

		struct stat s;
		lstat(d->d_name, &s);
		if(S_ISREG(s.st_mode)){
			vaisseau_type v;
			load_vaisseau(d->d_name, &v);
			mod->types[mod->nb_vaisseau_types] = v;
			mod->nb_vaisseau_types++;
		}
	}
	closedir(dir);
	
	chdir("..");
	chdir("..");
	chdir("..");
}