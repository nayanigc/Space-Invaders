#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//structure d'un niveau 
typedef struct {
	int nombre_vaisseaux;
	app_vaisseau* apps; 
	vaisseau* vaisseaux;

}niveau;

typedef struct {
		
}

// apparition de vaisseau 

typedef struct {
	int type;
	int temps;
	int pos_x;	
	int pos_y;
	
}app_vaisseau;
