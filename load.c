#include "load.h"

#define BUFSIZE 2048
#define MAXLINES (BUFSIZE / 12)
#define MAXAPPS (MAXLINES - 1)

int split_on_string(char* s, char* lines[], char* split){
	char* line;
	lines[0]=s;
	int compteur = 1;
	while((line = strpbrk(s, split)) != NULL){
			if(strcmp(line, "\n") == 0) break;
			*line = '\0';
			s = line + 1;
			lines[compteur] = (char *) s;
			compteur++;
	}
	lines[compteur] = NULL;
	
	return EXIT_SUCCESS;
}

void create_niveau(niveau* niv){
	niv->nombre_app =0;
	niv->apps = malloc(MAXAPPS*sizeof(app_vaisseau));
}

void deleted_niveau(niveau* niv){
	free(niv->apps);
	free(niv);
}

int load_niveau (char* filename, niveau* n_to_load){ 

	int fd = open(filename,O_RDONLY);
	if ( fd == -1) {
		perror (filename);
		return EXIT_FAILURE;
	}
	create_niveau(n_to_load);
	char buffer[BUFSIZE];
	read (fd, buffer,BUFSIZE);

	char* lines[MAXLINES];
	split_on_string(buffer, lines, "\n");
	n_to_load->nombre_vaisseaux = atoi(lines[0]);
	int i = 1;
	char* line;
	while((line = lines[i]) != NULL){
		app_vaisseau app_v;
		char* items[5];
		split_on_string(line, items, " ");
		app_v.type=atoi(items[0]);
		app_v.temps=atoi(items[1]);
		app_v.pos_x=atoi(items[2]);
		app_v.pos_y=atoi(items[3]);
		n_to_load->apps[n_to_load->nombre_app]=app_v;
		n_to_load->nombre_app++;
		i++;
	}

 	close(fd);
	return EXIT_SUCCESS;
}


void delete_vaisseau(vaisseau_type* v){
	int i;
	free(v->cycle_h);
	free(v->cycle_v);
	for (i=0; i< v->h;i++){
		free(v->dessin[i]);
	}
	free(v->dessin);
	free(v);
}

int load_vaisseau (char* filename, vaisseau_type* v_to_load){ 

	int fd = open ( filename,O_RDONLY);
	if ( fd == -1) {
		perror (filename);
		return EXIT_FAILURE;
	}
	
	char buffer[BUFSIZE];
	read (fd, buffer,BUFSIZE);
	int i;

	char* lines[MAXLINES];
	char** items;
	items = malloc(sizeof(char*)*2);
	for (i=0; i<2;i++){
		items[i]=malloc(sizeof(char)*8);
	}
	split_on_string(buffer, lines, "\n");
	
	split_on_string(lines[0], items," ");
	v_to_load->l = atoi(items[0]);
	v_to_load->h = atoi(items[1]);

	v_to_load->longueur_cycle = atoi(lines[1]);

	items = realloc(items, (sizeof(char*) * v_to_load->longueur_cycle));
	for(i = 2; i < v_to_load->longueur_cycle; i++){
		items[i] = malloc(sizeof(char) * 8);
	}
	split_on_string(lines[2], items, " ");
	v_to_load->cycle_h = malloc(v_to_load->longueur_cycle * sizeof(int));
	for(i = 0; i < v_to_load->longueur_cycle; i++){
		v_to_load->cycle_h[i] = atoi(items[i]);
	}
	split_on_string(lines[3], items, " ");
	
	v_to_load->cycle_v = malloc(v_to_load->longueur_cycle * sizeof(int));
	for(i = 0; i < v_to_load->longueur_cycle; i++){
		v_to_load->cycle_v[i] = atoi(items[i]);
	}

	v_to_load->nombre_vie = atoi(lines[4]);
	v_to_load->frequence_tir =atof(lines[5]);
	v_to_load->vitesse_tir=atoi(lines[6]);
	v_to_load->puissance_tir=atoi(lines[7]);
	v_to_load->symbole_tir = *lines[8];
	v_to_load->dessin = malloc(v_to_load->h * sizeof(char));
	for(i = 0; i < v_to_load->h; i++){
		v_to_load->dessin[i] = malloc(v_to_load->l * sizeof(char));
	}

	for(i = 0; i < v_to_load->h; i++){
		for(int j = 0; j < v_to_load->l; j++){
			v_to_load->dessin[i][j] = (char) lines[9 + i][j];
		}
	}

 	close(fd);
	return EXIT_SUCCESS;
}




