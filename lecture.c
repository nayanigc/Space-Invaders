#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

//DEFINITION D'UNE LISTE pour stocket les éléments d'un repertoire
typedef struct list_t {
	char *name;
	int is_dir;
	struct list_t *next;
}list_t;


//PARCOURS RECURSIF DES REPERTOIRES
int recursive_dir (char *root, char *mods) { // root : repertoire qu'on avait avant
											// mods : repertoire dans lequel on veut rentré
	DIR *FD;
	struct dirent *f;
	char *new_root;
	//Concatenation new_root="root/mods" 
	if (root != NULL) {
		int root_len = strlen (root);
		int dirName_len = strlen (mods);
												   //longeur de root+longeur de direName+'\0'
		if ( ( new_root = malloc( sizeof(char*) * (root_len + dirName_len + 2) ) ) == NULL) {
			fprintf (stderr, "probleme avec malloc\n");
			exit (EXIT_FAILURE);
		}

		// on peut utiliser mmecpy ou strcat

		/*memcpy (new_root, root, root_len);
		new_root[root_len] = '/';
		memcpy (new_root + root_len + 1, mods, dirName_len);
		new_root[root_len + dirName_len + 1] = '\0';*/

		strcat(new_root, root);
		strcat(new_root, "/");
		strcat(new_root, mods);

	} else new_root = strdup (mods); //renvoie un pointeur sur une nlle chaîne de caractères dupliquée depuis mods

	//obtention du repertoire courant
	int rep_courant_len = 32;
	char *rep_courant;

	if ((rep_courant = malloc(rep_courant_len* sizeof *rep_courant)) == NULL){
		fprintf(stderr, "probleme avec malloc\n");
		exit(EXIT_FAILURE);
	}

	while ( (rep_courant = getcwd (rep_courant, rep_courant_len)) == NULL ) {
		if (ERANGE != errno) {
			fprintf (stderr, "probleme avec getcwd (errno= '%s')\n",strerror (errno));
			exit (EXIT_FAILURE);
		}
		rep_courant_len += 32;
		rep_courant = realloc (rep_courant, sizeof (char*) * rep_courant_len );
	}
	chdir (mods);

	//Remplir la liste avec les fichiers du repertoire courant
	if ( (FD = opendir (".")) == NULL) {
		fprintf (stderr, "opendir() impossible\n");
		return (-1);
	}

	list_t *names = NULL;
	list_t *sl;

	sl = names;

	while ((f = readdir (FD))) {
		// struct stat donne des informations sur le fichier
		struct stat st;

		list_t *n;

		if (!strcmp (f->d_name, ".")) continue;
		if (!strcmp (f->d_name, "..")) continue;
		// récupère l'état du fichier pointé par f->d_name et remplit st
		if (stat (f->d_name, &st)) continue;

		if ( (n = malloc (sizeof *n)) == NULL) {
			fprintf (stderr, "Plus assez de memoire\n");
			exit (EXIT_FAILURE);
		}

		n->name = strdup (f->d_name);

		// on utilise S_ISDIR pour savoir si c'est une repertoire
		if (S_ISDIR (st.st_mode)) n->is_dir = 1;
		else n->is_dir = 0;
		n->next = NULL;

		if (sl) {
			sl->next = n;
			sl = n;
		} else {
			names = n;
			sl = n;
		}
	}

	closedir (FD);

	//parcourt les fichiers et repertoire pour action

	for (sl= names; sl; sl = sl->next) {
		if (sl->is_dir) {
			printf("nom du repertoire : %s\n", sl->name);
			recursive_dir(new_root, sl->name);
		} else {
			printf("nom du fichier : %s\n", sl->name);
		} 
	}
	

	//Nettoyage
	free (new_root);

	while (names) {
	    list_t *prev;
	    free (names->name);
	    prev = names;
	    names = names->next;
	    free (prev);
	}

	chdir (rep_courant);
	free (rep_courant);
	return 0;
}


int main (int argc, char **argv) {
	if (argc >= 1) recursive_dir (NULL, argv[1]);
	return EXIT_SUCCESS;
}
