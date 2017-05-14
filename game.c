#include "game.h"

/* Initialise le vaisseau du joueur avec le type de vaisseau 'type' */
void init_my_vaisseau(plateau* plat, vaisseau_type* type){
	plat->my_vaisseau.type = *type;

	//Le vaisseau doit apparaitre au centre en bas de l'écran de jeu
	plat->my_vaisseau.pos_x = PLATEAU_LARGEUR / 2;
	plat->my_vaisseau.pos_y = PLATEAU_HAUTEUR - (type->h);

	plat->my_vaisseau.nombre_vie = type->nombre_vie;
	plat->my_vaisseau.cursor_cycle = 0;
	plat->my_vaisseau.last_shot = 0L;
}

/* Ajoute dans le plateau de jeu les vaisseaux qui doivent apparître au temps 'time' */
void add_apparitions(mod* mod, niveau* niv, plateau* plat, int time){
	long current_time = get_current_time();

	//Pour chaque apparitions
	for(int i = 0; i < niv->nombre_app; i++){

		//On vérifie si un vaisseau doit apparaitre au temps 'time'
		if(niv->apps[i].temps == time){
			
			//On vérifie que le type de vaisseau existe  
			if(niv->apps[i].type >= mod->nb_vaisseau_types){
				break;
			}

			//On créer un vaisseau et on l'ajoute au plateau
			vaisseau* v = malloc(sizeof(vaisseau));

			v->type = mod->types[niv->apps[i].type];
			v->pos_x = niv->apps[i].pos_x;
			v->pos_y = niv->apps[i].pos_y;
			v->nombre_vie = v->type.nombre_vie;
			v->cursor_cycle = 0;
			v->last_shot = current_time;

			//On place le vaisseau sur le plateau, dans le tableau des vaisseaux
			plat->vaisseaux[plat->nombre_vaisseaux] = *v;
			plat->nombre_vaisseaux++;
		}
	}

}

/* Met à jour la position de tous les vaisseaux dans le plateau
   Fait tirer les vaisseaux lorsqu'ils doivent tirer */
void update_positions(plateau* plat){
	long current_time = get_current_time();

	//Pour chaque vaisseau dans le plateau
	for(int i = 0; i < plat->nombre_vaisseaux; i++){
		vaisseau* v = &plat->vaisseaux[i];

		//On met à jour la position grâce au cycle de déplacement 
		v->pos_x += v->type.cycle_v[v->cursor_cycle];
		v->pos_y += v->type.cycle_h[v->cursor_cycle];

		//Si on arrive au bout du cycle, on reviens au début
		v->cursor_cycle++;
		if(v->cursor_cycle == v->type.longueur_cycle) v->cursor_cycle = 0;

		//Periode = (1 / Frequence)
		int periode = (int) (1 / v->type.frequence_tir);

		//Si le temps est dépassé, le vaisseau peut tirer
		if(v->last_shot + (periode * SECONDE) < current_time){

			//On créer un tir et on le rajoute au tableau des tirs 
			shot* s = malloc(sizeof(shot));
			s->symbole = v->type.symbole_tir;
			s->pos_y = v->pos_y + v->type.h;
			s->pos_x = v->pos_x + (v->type.l / 2);
			s->vitesse = v->type.vitesse_tir;
			s->last_move = current_time;
			s->puissance = v->type.puissance_tir;
			s->dir = BAS;
			plat->shots[plat->nombre_shots] = *s;
			plat->nombre_shots++; 

			//On set la dernier fois que le vaisseau à tirer au temps actuel
			//Car le vaisseau viens de tirer
			v->last_shot = current_time;
		} 
	}
}

/* On met à jour la position des tirs dans le plateau */
void update_positions_shots(plateau* plat){
	long current_time = get_current_time();
	
	//Pour chaque tir
	for(int i = 0; i < plat->nombre_shots; i++){
		shot* s = &plat->shots[i];

		//Si le délai avant que le tir puisse bouger est écoulé, le tir bouge
		if(current_time - s->last_move > (long) (SECONDE / s->vitesse)){
			switch(s->dir){
				case HAUT: s->pos_y--; break;
				case BAS: s->pos_y++; break;
				case GAUCHE: s->pos_x--; break;
				case DROITE: s->pos_x++; break;
				default: ;
			}

			//On set la dernier fois que le tir à bouger au temps actuel
			s->last_move = current_time;
		}
	}
}

/* Gestion de la collision entre un vaisseau et un tir
   On sais que la collision a eu lieu a la position (pos_y, pos_x) */
bool handle_shot_collision(plateau* plat, vaisseau* v, int pos_y, int pos_x){
	//Pour chaque tir sur le plateau de jeu
	for(int i = 0; i < plat->nombre_shots; i++){
		shot* s = &plat->shots[i];

		//Si c'est le tir qui est actuellement à la position de la collision
		if(s->pos_y == pos_y && s->pos_x == pos_x){
			
			//On décrémente la vie du vaisseau de la puissance du tir
			v->nombre_vie -= s->puissance;

			//On fait disparaitre le tir de manière artificiel
			s->pos_x = -PLATEAU_LARGEUR;
			s->pos_y = -PLATEAU_HAUTEUR;
			
			//Si le tir est mortel, (le vaisseau n'a plus de vie) on renvoie true
			if(v->nombre_vie <= 0){
				v->pos_x = -PLATEAU_LARGEUR;
				v->pos_y = -PLATEAU_HAUTEUR;
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}

/* Remplis le tableau cases du plateau avec les caractères du dessin du vaisseau 'v' 
   Vérifie si un collision a eu lieu */
bool remplir_plateau_vaisseau(plateau* plat, vaisseau* v, case_contenu contenu){
	//Si le vaisseau n'est pas 
	if(v->pos_y >= PLATEAU_HAUTEUR || v->pos_x >= PLATEAU_LARGEUR
		|| v->pos_y < 0 || v->pos_x < 0){
		return false;
	}

	//Une collision mortel a eu lieu
	bool collision_mortel = false;

	//Une collision a eu lieu entre un tir et a été gérée
	bool collision_handled = false;

	case_contenu current_case_contenu;

	//On parcours le dessin du du vaisseau
	for(int k = 0; k < v->type.h; k++){
		for(int l = 0; l < v->type.l; l++){
			//Si le caractère est sur le plateau on vérifie les collisions et on l'ajoute au tableau cases 
			if(v->pos_x + l < PLATEAU_LARGEUR && v->pos_y + k < PLATEAU_HAUTEUR
				&& v->pos_x + l >= 0 && v->pos_y + k >= 0){

				//On récupère le type de contenu déjà présent sur la case
				current_case_contenu = plat->collision_cases[v->pos_y + k][v->pos_x + l];
				switch(current_case_contenu){
					case VIDE: ; break;
					case ME: ; break;
					//Si on entre en collision avec un ennemi, la collision est mortelle
					case ENNEMI:
						//Si le vaisseau que l'on est en train de mettre sur le plateau est le notre 
						if(contenu == ME) collision_mortel = true; 
						break;
					//Si on entre en collision avec un tir, on gère la collision
					case SHOT: 
						if(contenu == ME){
							if(collision_handled) break;
							//On vérifie si la collision avec le tir a été mortelle pour le joueur 
							collision_mortel = handle_shot_collision(plat, &plat->my_vaisseau, v->pos_y + k, v->pos_x + l);
							if(collision_mortel) return true;
							collision_handled = true;	
						} else {
							//On vérifie si la collision est mortelle pour une vaisseau ennemi
							bool collision_mortel = handle_shot_collision(plat, v, v->pos_y + k, v->pos_x + l);
							if(collision_mortel) return false;
						}
						break;
				}

				//On ajoute le caratère sur le tableau cases
				plat->cases[v->pos_y + k][v->pos_x + l] = v->type.dessin[k][l];

				//On ajoute le type de contenu dans le tableau des collisions 
				plat->collision_cases[v->pos_y + k][v->pos_x + l] = contenu;
			}
		}
	}

	return collision_mortel;
}

/* Remplis le tableau cases du plateau avec le shot 's' */
void remplir_plateau_shot(plateau* plat, shot* s){
	//Si le shot est hors du plateau, on ne fait rien
	if(s->pos_x < 0 || s->pos_x >= PLATEAU_LARGEUR
		|| s->pos_y < 0 || s->pos_y >= PLATEAU_HAUTEUR){
		return;
	}

	//On ajoute le symbole du shot sur le plateau
	plat->cases[s->pos_y][s->pos_x] = s->symbole;

	//On complète la grille de collision
	plat->collision_cases[s->pos_y][s->pos_x] = SHOT;
}

/* Remplis le tableau cases du tableau avec les caractères a afficher pour chaque position */
bool remplir_plateau(plateau* plat){
	//On initialise la plateau avec des '.', il n'y a rien dessus au départ
	for(int k = 0; k < PLATEAU_HAUTEUR; k++){
		for(int l = 0; l < PLATEAU_LARGEUR; l++){
			plat->cases[k][l] = '.';
			plat->collision_cases[k][l] = VIDE;
		}
	}

	//On ajoute les tirs
	for(int i = 0; i < plat->nombre_shots; i++){
		remplir_plateau_shot(plat, &plat->shots[i]);
	}

	//On ajoute les vaisseaux ennemis
	for(int i = 0; i < plat->nombre_vaisseaux; i++){
		remplir_plateau_vaisseau(plat, &plat->vaisseaux[i], ENNEMI);
	}
	
	//On ajoute le vaisseau du joueur, la fonction retourne si une collision mortelle a eu lieu
	return remplir_plateau_vaisseau(plat, &plat->my_vaisseau, ME);
}

/* Parcours le tableau cases du plateau et remplis un buffer contenant tout les caractères pour l'afficher à l'écran */
void affiche_plateau(plateau* plat){
	//Nombre de retour à la ligne à effecter entre chaque affichage
	int retours = 30;
	int total = 0;
	char* nb_vie = malloc(50 * sizeof(char));
	sprintf(nb_vie, "VIE: %d\n", plat->my_vaisseau.nombre_vie);
	total += strlen(nb_vie);
	//Allocation du buffer du nombre de caractère du plateau 
	//Plus un caractère en plus sur chaque ligne pour les '\n'
	//Plus le nombre de '\n' a afficher entre chaque écran
	char* plat_buffer = malloc(((PLATEAU_HAUTEUR + 1) * PLATEAU_LARGEUR + retours + total) * sizeof(char));
	
	for(int i = 0; i < retours; i++){
		plat_buffer[i] = '\n';
		total++;
	}

	//Nombre de retours à la ligne effectués
	int compteurLR = 0;
	for(int i = 0; i < PLATEAU_HAUTEUR; i++){
		for(int j = 0; j < PLATEAU_LARGEUR; j++){
			plat_buffer[i * PLATEAU_LARGEUR + j + compteurLR + retours] = plat->cases[i][j];
			total++;
		}
		//Après chaque ligne, un retour à la ligne
		plat_buffer[(i + 1) * PLATEAU_LARGEUR + compteurLR + retours] = '\n';
		compteurLR++;
		total++;
	}
	for(int i = 0; i < strlen(nb_vie); i++){
		plat_buffer[total++] = nb_vie[i];
	}
	write(STDOUT_FILENO, plat_buffer, total);
	free(plat_buffer);
	free(nb_vie);
}
// notre vaisseau //
void fire_shot(plateau* plat){
	vaisseau* v = &plat->my_vaisseau;

	//On créer un tir et on le rajoute au tableau des tirs 
	shot* s = malloc(sizeof(shot));
	s->symbole = v->type.symbole_tir;
	s->pos_y = v->pos_y - 1;
	s->pos_x = v->pos_x + (v->type.l / 2);
	s->vitesse = v->type.vitesse_tir;
	s->last_move = get_current_time();
	s->puissance = v->type.puissance_tir;
	s->dir = HAUT;

	plat->shots[plat->nombre_shots] = *s;
	plat->nombre_shots++;
}

/* Déplace le vaisseau du joueur dans la direction 'd' */
void move_my_vaisseau(plateau* plat, direction d){
	switch(d){
		case HAUT: 

		break;
		case BAS: 

		break;
		case GAUCHE:
			if(plat->my_vaisseau.pos_x > 0) plat->my_vaisseau.pos_x--;
		break;
		case DROITE: 
			if(plat->my_vaisseau.pos_x + plat->my_vaisseau.type.l < PLATEAU_LARGEUR) 
				plat->my_vaisseau.pos_x++;
		break;
		default: ;
	}
}

/* Retourne le temps en µs écoulé depuis une certaine date fixe (1 janvier 1970) */
long get_current_time() {
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    return SECONDE * spec.tv_sec + (spec.tv_nsec / 1000l);
}

/* Met à jour puis affiche le plateau 'plat'
   Si une collision est détectée afficher "PERDU" et retourne true */
bool remplir_affiche(plateau* plat){
	bool collision_detectee = remplir_plateau(plat);
	
	affiche_plateau(plat);
	
	if(collision_detectee){
		write(STDOUT_FILENO, "\nPERDU\n", 8);
		return true;
	}

	return false;
}

bool check_win(plateau* plat){
	bool win = true;
	for(int i = 0; i < plat->nombre_vaisseaux; i++){
		if(plat->vaisseaux[i].nombre_vie > 0) win = false;
	}


	return win;
}

/* Lance le niveau 'niv' appartenant au mod 'mod' */
bool play(mod* mod, niveau* niv){

	//Créer et initialize un plateau vide
	plateau* plat = malloc(sizeof(plateau));
	plat->nombre_vaisseaux = 0;
	plat->vaisseaux = malloc(50 * sizeof(vaisseau));
	plat->nombre_shots = 0;
	plat->shots = malloc(1000 * sizeof(shot));

	bool win = false;

	//Un compteur d'unités de temps utilisé pour faire apparaitre les vaisseaux
	int time = 0;

	//Elements utilisés par la fonction poll qui va surveillé l'entrée standard
	char buffer[1024];
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events=POLLIN;
	int timeout_msecs = 100;

	//Contient Le temps actuel
	long current_time = 0;

	//Contient le temps lors du dernier appel a get_current_time 
	long last_time = get_current_time();

	//Le délai entre chaque mise à jour de la position des vaisseaux
	long delai = 2 * SECONDE;

	int max_time = 0;
	for(int i = 0; i < niv->nombre_app; i++){
		if(niv->apps[i].temps > max_time) max_time = niv->apps[i].temps;
	}

	while(true){

		//Au temps 0, on initialise le vaisseau du joueur avec le type 0
		if(time == 0) init_my_vaisseau(plat, &mod->types[0]);

		current_time = get_current_time();
		//Si le delais est écoulé
		if(current_time > last_time + delai){
			//On fait apparaitre les vaisseau qui doivent apparaitre à cet instant			
			add_apparitions(mod, niv, plat, time);
			
			//On met à jour la position des vaisseaux
			update_positions(plat);	

			time++;

			//On met le temps de derniere mise a jour au temps courant
			last_time = current_time;
		}

		//On affiche le plateau de jeu à l'écran
		if(remplir_affiche(plat)) break;

		//On met à jour la position des tirs
		update_positions_shots(plat);

		if(time > max_time && check_win(plat)){
			win = true;
			break;
		}
		
		//Poll bloque si aucun évenement n'arrive dans l'entrée standard
		//Ou si le time out se déclanche
		int res = poll(fds, 1, timeout_msecs);
		if (res>0){
			if (fds[0].revents & POLLIN){
				//On récupère la touche entrée
				int nread = read(fds[0].fd, buffer, 1024);
				if(nread > 0){
					if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 68){
						move_my_vaisseau(plat, GAUCHE);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 65){
						fire_shot(plat);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 67){
						move_my_vaisseau(plat, DROITE);
					} else if(buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 66){
						//Touche BAS 
					} else if(buffer[0] == 113){
						//La touche est 'q' on quitte le niveau
						break;
					}
				}
			}
		}	
	}

	//Libère la mémoire du plateau
	delete_plat(plat);

	return win;
}

void delete_plat(plateau* plat){
	free(plat->vaisseaux);
	free(plat->shots);
	free(plat);
}
