/**
 * \file placement.c
 * \brief Module utilisé lors de l'initialisation du jeu. Les joueurs placent leur personnage.
 * \author MOK Modira
 * \version 0.1
 * \date 18 décembre 2015
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/placement.h"
#include "../include/map.h"
#include "../include/tableau.h"
#include "../include/perso.h"
#include "../include/SDL_isometric.h"

extern type_Map tMap;
extern t_context *ingame;


extern t_perso ensemble_perso[i_nombre_classe];
extern t_perso tab_perso[i_taille_tab_perso];
extern int map[i_taille_map][i_taille_map];

/**
 * \fn void placement_perso()
 * \brief Permet de placer les personnages dans la carte.
 *
 */
void placement_perso() //Cette fonction place les personnages des différents équipes dans les trois premières et dernières ligne de la map.
{
	int i,j;
	init_tab_perso(tab_perso);
	int i_classe; //Classe du personnage choisie.
	int coord[2];
	int b_sortie=1; //Condition de sortie
	int i_compteur=0;
	int i_nb_perso=0; //Nombre de perso à placer
	t_perso * per;
	per=malloc(sizeof(t_perso));
	int i_equipe=0; //0 pour l'équipe A et 1 pour l'équipe B
	while(i_nb_perso<4) //Ici on veut 10 personnages en tout.
	{
		while(b_sortie!=0)
		{
			if(i_equipe%2==0)
			{
				printf("\nEquipe A, à vous de placer un personnage : \n"); //La phase de choix de l'équipe A
				printf("Choisissez les coordonnées (ligne entre 0 et 2 et colonne entre 0 et 9): ");
				// scanf("%i%i", &coord[0], &coord[1]); //Les coordonnées sont comprises entre 0 et 9.
				for (i = 0; i < 3; i++) {
					for (j = 0; j < i_taille_map; j++) {
						drawTileplace(ingame, tMap, i* TILE_W, j* TILE_H);
					}
				}
				SDL_generate(ingame);
			  GetClickPlace(ingame, tMap, &coord[0], &coord[1]);
				printf("%i %i\n",coord[0],coord[1]  );
				if(coord[0]>=0 && coord[0]<3 && coord[1]>=0 && coord[1]<=9 && map[coord[0]][coord[1]]==0 && recherche_perso_tab(coord[0], coord[1])==-1) //On vérifie que les coordonnées sont bonnes
				{
					b_sortie=0;
				}
				else
				{
					printf("Les coordonnées ne sont pas bonnes.\n");
				}
			}
			else
			{
				printf("\nEquipe B, à vous de placer un personnage : \n"); //On passe à la phase de choix de l'équipe B
				printf("Choisissez les coordonnées (ligne entre 7 et 9 et colonne entre 0 et 9): ");
				// scanf("%i%i", &coord[0], &coord[1]); //Les coordonnées sont comprises entre 0 et 9.
				for (i = 7; i < i_taille_map; i++) {
					for (j = 0; j < i_taille_map; j++) {
						drawTileplace(ingame, tMap, i* TILE_W, j* TILE_H);
					}
				}
				SDL_generate(ingame);
				GetClickPlace(ingame, tMap, &coord[0], &coord[1]);
				printf("%i %i\n",coord[0],coord[1]  );
				if(coord[0]>=7 && coord[0]<=9 && coord[1]>=0 && coord[1]<=9 && recherche_perso_tab(coord[0], coord[1])==-1) //On vérifie aussi qu'il n'y ait personne dans cette case
				{
					b_sortie=0;
				}
				else
				{
					printf("Les coordonnées ne sont pas bonnes.\n");
				}
			}
		}
		b_sortie=1;

		while(b_sortie!=0)
		{
			printf("Choisissez la classe : \n");
			int i;
			printf("Vous pouvez placer un : 0 - %s\n", ensemble_perso[0].s_classe);
			for(i=1;i<i_nombre_classe;i++)
			{
				printf("\t\t\t%i - %s\n",i, ensemble_perso[i].s_classe);
			}
			printf("Choisissez la classe du personnage: ");
			// scanf("%i", &i_classe); //0 pour un guerrier, 1 pour un archer, 2 pour un mage
			i_classe = MenuPerso(ingame);
			i_classe--;
			if(i_classe<0 || i_classe>=i_nombre_classe)
			{
				printf("Le nombre de la classe saisi doit être compris entre 0 et %i.\n\n", i_nombre_classe-1);
			}
			else
			{
				if(i_equipe%2==0)
				creation(per, i_classe, 'A', coord[0], coord[1]);
				else
				creation(per, i_classe, 'B', coord[0], coord[1]);
				tab_perso[i_compteur]=*per; //On ajoute le perso au tableau.
				i_compteur++;
				b_sortie=0;
			}
		}
		b_sortie=1;
		i_equipe++;
		i_nb_perso++;
		afficher_map();
	}
}
