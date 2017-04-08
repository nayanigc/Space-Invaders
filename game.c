#include "game.h"

int play(niveau* niv, mod* mod){

	plateau plat;
	plat.nombre_vaisseaux = 0;
	plat.vaisseaux = malloc(niv->nombre_vaisseaux * sizeof(vaisseau));

	int time = 0;
	char buffer[1024];
	struct pollfd fds[1];
	int timeout_msecs=-1;
	fds[0].fd = 0;
	fds[0].events=POLLIN;
	while(true){

		//TODO Gérer les index out of bound
		//TODO découper en fonctions

		for(int i = 0; i < niv->nombre_app; i++){
			if(niv->apps[i].temps == time){
				vaisseau v;
				v.type = mod->types[niv->apps[i].type];
				v.pos_x = niv->apps[i].pos_x;
				v.pos_y = niv->apps[i].pos_y;
				v.nombre_vie = mod->types[niv->apps[i].type].nombre_vie;

				plat.vaisseaux[plat.nombre_vaisseaux] = v;
				plat.nombre_vaisseaux++;
			}
		}

		//Remplissage du tableaux plat.cases grâce à la liste des vaisseaux

		//Affichage

		int res = poll(fds,1,timeout_msecs);
		if (res>0){
			if (fds[0].revents & POLLIN){
				int nread = read(fds[0].fd, buffer, 1024);
				if(nread > 0){
					if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 68){
						//write(1, "GAUCHE\n", 8);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 65){
						//write(1, "HAUT\n", 6);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 67){
						//write(1, "DROITE\n", 8);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 66){
						//write(1, "BAS\n", 5);
					} else if(buffer[0] == 113){
						break;
					}
				}
			}
		}	
	}
	return 0;
}