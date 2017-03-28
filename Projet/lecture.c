#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>



int ParcourRepertoire (DIR *repertoire, const char *path) {
	struct dirent *dir;
	while ((dir = readdir(repertoire))!=NULL){
		if (dir == NULL) { /*printf(" %s popo\n", dir); */return 0; }
		else {
			printf("le nom du fichier lu est : %s\n", dir->d_name);
			struct stat buf;
			lstat(path, &buf);
           	if ((buf.st_mode & S_IFMT) == S_IFDIR) {
	            printf("TEST\n");
	            DIR * sous_rep = opendir("dir->d_name");
	            printf(" %s\n", path);
	            strcat("popo", "caca");
	            printf(" %s\n", path);
	            strcat(path, "/");
				printf(" %s\n", path);
				strcat(path, dir->d_name);
				printf(" %s\n", path);
				ParcourRepertoire(sous_rep, path);
           	}
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	// ouverture du repertoire contenant les MODS
	DIR * MODS = opendir("mods");
	// Si le dossier n'a pas pu être ouvert
	if (MODS == NULL) 
		exit(1);
	//sinon 
	/* je parcours chaque dossier du repertoire*/
	ParcourRepertoire (MODS, "mods");
	return EXIT_SUCCESS;
}


