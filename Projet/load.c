#include "load.h"

#define BUFSIZE 2048
#define MAXLINES (BUFSIZE / 12)

int split_on_string(char* s, char* lines[], char* split){
	char* line;
	lines[0]=s;
	int compteur = 1;
	while((line = strpbrk (s, split)) != NULL ){
			//printf("espace détecté : %d\n",compteur);
			*line = '\0';
			s = line + 1;
			lines[compteur] = (char *) s;
			//printf("argv[%d] = %s\n",compteur,s);
			//printf("argv : %s\n", argv[compteur]);
			compteur++;
	}
	lines[compteur] = (char*) s;
	lines[compteur++] = NULL;

	for(int i=0; i<compteur;i++){
		printf("argv[%d]=|%s| \n",i,argv[i]);
	}
	
	return EXIT_SUCCESS;
}


int load_niveau (char* filename, niveau* n_to_load){ 
	int fd = open ( filename,O_RDONLY);
	if ( fd == -1) {
		perror (filename)
		return EXIT_FAILURE;
	}
	int buffer[BUFSIZE];
	read (fd, buffer,BUFSIZE);

	char* lines[MAXLINES];
	split_on_string(buffer, lines, "\n");

	n_to_load->nombre_vaisseaux = atoi(lines[0]);
	int i = 1;
	while((line = lines[i]) != NULL){
		app_vaisseau app_v;
			
	}

 	close(fd);
	return EXIT_SUCCESS;
}



