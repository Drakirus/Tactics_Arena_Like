#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Outil.h"
#include "save.h"

#define i_taille_map 10


int main() {
	srand(time(NULL));
	int grille[i_taille_map][i_taille_map];
	init_mat(grille);
  	int choix_menu = 0;
	do{
		printf("1 - Nouvelle Partie\n");
		printf("2 - Charger une partie\n");
		printf("3 - Quitter\n");
		scanf("%i", &choix_menu);
		switch(choix_menu){
			case 1: /*nouvelle partie*/ break;
			case 2: charge(grille); break;
			case 3: break;
		}
	}while(choix_menu != 3);
	return 0;
}
