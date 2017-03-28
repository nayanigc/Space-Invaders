#include "load.h"

#define BUFSIZE 2048
#define MAXLINES (BUFSIZE / 12)
#define MAXAPPS (MAXLINES - 1)

int split_on_string(char* s, char* lines[], char* split){
	char* line;
	lines[0]=s;
	int compteur = 1;
	while((line = strpbrk (s, split)) != NULL ){
			*line = '\0';
			s = line + 1;
			lines[compteur] = (char *) s;
			compteur++;
	}
	lines[compteur] = (char*) s;
	lines[compteur-1] = NULL;
// enleve les affichage //
	for(int i=0; i<compteur;i++){
		printf("lines[%d]=|%s| \n",i,lines[i]);
	}
	
	return EXIT_SUCCESS;
}

void create_niveau(niveau* niv){
	niv->nombre_app =0;
	niv->apps =  malloc(MAXAPPS*sizeof(app_vaisseau));
}

void deleted_niveau(niveau* niv){
	free(niv->apps);
	free(niv);
}

int load_niveau (char* filename, niveau* n_to_load){ 

	int fd = open ( filename,O_RDONLY);
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
		char* items[4];
		split_on_string(buffer, items, " ");
		app_v.type=atoi(items[0]);
		app_v.temps=atoi(items[1]);
		app_v.pos_x=atoi(items[2]);
		app_v.pos_y=atoi(items[3]);
		n_to_load->apps[n_to_load->nombre_app]=app_v;
		n_to_load->nombre_app++;
	}

 	close(fd);
	return EXIT_SUCCESS;
}


void deleted_vaisseau(vaisseau* v){
	int i;
	free(v->deplacement_horizontal);
	free(v->deplacement_vertical);
	for (i=0; i<v->h;i++){
		free(v->dessin[i]);
	}
	free(v->dessin);
	free(v);
}

int load_vaisseau (char* filename, vaisseau* v_to_load){ 

	int fd = open ( filename,O_RDONLY);
	if ( fd == -1) {
		perror (filename);
		return EXIT_FAILURE;
	}
	
	char buffer[BUFSIZE];
	read (fd, buffer,BUFSIZE);
	int i = 1;

	char* lines[MAXLINES];
	char** items;
	items = malloc(sizeof(char*)*2);
	for (i=0; i<2;i++){
		items[i]=malloc (sizeof(char)*8);
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
	v_to_load->deplacement_horizontal = malloc(v_to_load->longueur_cycle * sizeof(char));
	for(i = 0; i < v_to_load->longueur_cycle; i++){
		v_to_load->deplacement_horizontal[i] = atoi(items[i]);
	}
	split_on_string(lines[3], items, " ");
	v_to_load->deplacement_vertical = malloc(v_to_load->longueur_cycle * sizeof(char));
	for(i = 0; i < v_to_load->longueur_cycle; i++){
		v_to_load->deplacement_vertical[i] = atoi(items[i]);
	}

	v_to_load->nombre_vie = atoi(items[4]);
	v_to_load->frequence_tir =atof(items[5]);
	v_to_load->vitesse_tir=atoi(items[6]);
	v_to_load->puissance_tir=atoi(items[7]);
	v_to_load->symbole_tir=*items[8];
	
	v_to_load->dessin = malloc(v_to_load->h * (sizeof(char*)));
	for(i = 0; i < v_to_load->h; i++){
		v_to_load->dessin[i] = malloc(v_to_load->l * sizeof(char));
	}

	for(i = 0; i < v_to_load->h; i++){
		v_to_load->dessin[i] = items[9 + i];
	}

 	close(fd);
	return EXIT_SUCCESS;
}




