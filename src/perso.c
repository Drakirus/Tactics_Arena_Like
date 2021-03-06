/**
 * \file perso.c
 * \brief Module gérant la création des personnages et de leurs caractéristiques, ainsi que des changements relatifs à leur caractéristiques
 * \author MOK Modira, CHAMPION Pierre
 * \version 0.1
 * \date 18 décembre 2015
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/perso.h"

#include "../include/SDL_isometric.h"

extern type_Map tMap;
extern t_context *ingame;
/**
 * \def Les personnages que l'on a créé
 **/
t_perso ensemble_perso[i_nombre_classe]=
{// "perso_name" / HP_max / HP / Points d'action / Points de mouvement / ligne / colonnes / team / placé ou pas / LISTE de ses attaques
	{"Guerrier",50,50,3,3,{0,0},'X', NULL, "Guerrier.png", -1},
	{"Archer",30,30,3,3,{0,0},'X', NULL, "Archer.png", -1},
	{"Mage",20,20,2,2,{0,0},'X', NULL, "Mage.png", -1},
	{"Voleur",10,10,4,4,{0,0},'X', NULL, "Voleur.png", -1},
};

/*Sert pour les tests*/
/**
 * \fn void afficher_perso(t_perso perso)
 * \param perso Le perso que l'on veut afficher
 * \brief Affiche le personnage avec toutes ses caractéristiques dans la console
 *
 */
void afficher_perso(t_perso perso){
	printf("%s %i/%iHP %i %i [%i,%i] %c\n", perso.s_classe, perso.i_HP, perso.i_HP_max, perso.i_PA, perso.i_PM, perso.coord[0], perso.coord[1], perso.c_team);
}

/*Affichage dans la console*/
/**
 * \fn void affichage_perso(t_perso perso)
 * \param perso Le personnage que l'on veut affciher
 * \brief Affiche le personnage avec le nom de sa classe, ses HP, HP maximum et ses coordonnées.
 *
 */
void affichage_perso(t_perso perso){
	printf("%s %i/%iHP [%i,%i] \n", perso.s_classe, perso.i_HP, perso.i_HP_max, perso.coord[0], perso.coord[1]);
	// displaylistAttack(perso.att);
}

/**
 * \fn void initialisation_perso(int a, t_perso * per)
 * \param a Ce paramètre permet de savoir à quel personnage on doit initialiser le personnage en question
 * \param per Le personnagne qui est normalement vide.
 * \brief Le personnage en paramètre a maintenant les caractéristiques du personnage de la classe que désigne a.
 *
 */
void initialisation_perso(int a, t_perso * per) //On affecte à un t_perso les données initiales présentes dans ensemble_perso
{
	strcpy(per->s_classe,ensemble_perso[a].s_classe);
	per->i_HP_max=ensemble_perso[a].i_HP_max;
	per->i_HP=ensemble_perso[a].i_HP;
	per->i_PA=ensemble_perso[a].i_PA;
	per->i_PM=ensemble_perso[a].i_PM;
	per->coord[0]=ensemble_perso[a].coord[0];
	per->coord[1]=ensemble_perso[a].coord[1];
	per->c_team=ensemble_perso[a].c_team;
	strcpy(per->file,ensemble_perso[a].file);
	per->id=ensemble_perso[a].id;
	init_attack(per);
}

// attention : il faut que le perso passe en param ait sa classe de correctement fixee
/**
 * \fn int init_attack(t_perso *per)
 * \param per
 * \return
 * \brief
 *
 */
int init_attack(t_perso *per){
	per->att = creer_liste_attack(); // creation du conteneur
  // "attack_name" / range_max / range_min / cost_PA / splash_range / only_line / Nb trait / "Nom du trait" / valeur du trait
	if( strcmp(per->s_classe, "Guerrier" ) == 0){ //guerrier
		pushAttack(per->att, createAttack("Coup d'epee", 1, 0, 3, 1, 0, 1, "HP", -5));// copie d'une attack par defaut d'un guerrier
	}else if(strcmp(per->s_classe, "Archer") == 0 ){ //Archer
		pushAttack(per->att, createAttack("Pousse toi d'la", 5, 2, 3, 1, 1, 3, "HP", -1,"coord_r", -2,"coord_c", -2));// copie d'une attack par defaut d'un archer
		pushAttack(per->att, createAttack("Tir a l'arc", 5, 2, 3, 1, 0, 2, "HP", -10, "HP_max", -2));// copie d'une attack par defaut d'un archer
	}else if(strcmp(per->s_classe, "Mage" ) == 0){ //Mage
		pushAttack(per->att, createAttack("Boule De Feu", 3, 0, 2, 2, 1, 1, "HP", -3));// copie d'une attack par defaut d'un mage
		pushAttack(per->att,createAttack("Sort de Soin", 3, 0, 2, 1, 0, 1, "HP", 6));
	}else if (strcmp(per->s_classe, "Voleur" ) == 0){ //Voleur
		pushAttack(per->att,createAttack("Lenteur", 1, 0, 4, 1, 0, 1, "PM", -1));
		pushAttack(per->att,createAttack("Coup de Babouche", 2, 0, 2, 1, 0, 2, "HP", -5, "PA", 2));
	} else{
		return 0;
	}
	return 1;
}

/**
 * \fn void augmente_nombre(int a, t_perso * per, int i_montant)
 * \param a Ce paramètre indique quelle caractéristique du personnage on désire modifier
 * \param per Le personnage que l'on souhaite modifier
 * \param i_montant De combien le paramètre a change
 * \brief On ajoute à la caractèristique le montant du paramètre i_montant
 *
 */
void augmente_nombre(int a, t_perso * per, int i_montant) //Augmente ou soustrait une valeur numérique d'un perso, 1=i_HP_max, 2=i_HP, 3=i_PA, 4=i_PM, 5=coord[0], 6=coord[1]
{
	switch(a)
	{
		case 1 : per->i_HP_max+=i_montant; break;
		case 2 : per->i_HP+=i_montant; break;
		case 3 : per->i_PA+=i_montant; break;
		case 4 : per->i_PM+=i_montant; break;
		case 5 : per->coord[0]+=i_montant; break;
		case 6 : per->coord[1]+=i_montant; break;
	}
}

/**
 * \fn void change_nombre(int a, t_perso * per, int i_montant)
 * \param a Ce paramètre indique quelle caractéristique du personnage on désire modifier
 * \param per Le personnage que l'on souhaite modifier
 * \param i_montant De combien le paramètre a change
 * \brief On ajoute pas, on change la caractéristiques par le paramètre i_montant
 *
 */
void change_nombre(int a, t_perso * per, int i_montant) //Change une valeur numérique d'un perso, 1=i_HP_max, 2=i_HP, 3=i_PA, 4=i_PM, 5=coord[0], 6=coord[1]
{
	switch(a)
	{
		case 1 : per->i_HP_max=i_montant; break;
		case 2 : per->i_HP=i_montant; break;
		case 3 : per->i_PA=i_montant; break;
		case 4 : per->i_PM=i_montant; break;
		case 5 : per->coord[0]=i_montant; break;
		case 6 : per->coord[1]=i_montant; break;
	}
}

/**
 * \fn void creation(t_perso * per, int i_classe, char c_team, int x, int y)
 * \param per Le personnage que l'on souhaite créer
 * \param i_classe La classe du personnage
 * \param c_team Son équipe
 * \param x Sa coordonnée en abscisse
 * \param y Sa coordonnée en ordonnée
 * \brief On prendre normalement en paramètre un personnage vide, auquel on ajoute son placement dans la carte, son équipe et sa clase
 *
 */
void creation(t_perso * per, int i_classe, char c_team, int x, int y) //On change juste les coordonnées et l'équipe du personnage.
{
	initialisation_perso(i_classe, per);
	per->coord[0]=x;
	per->coord[1]=y;
	per->c_team=c_team;
	// printf("%s\n", per->file);
	int posX = x * TILE_W, posY = y * TILE_H;
	toIso(tMap, &posX, &posY);
	posX += offsetX(tMap);
	posY += offsetY();
	SDL_newSprite(ingame, per->file, colorGreenLight, 100, 78, posX, posY-(78/1.5), 5, 1, 0);
	SDL_setOnLayer(ingame, SPRITE, ingame->nbSprite-1, 10);
	per->id = ingame->nbSprite-1;
	
}

/**
 * \fn int est_mort(t_perso perso)
 * \param perso Le personnage en question
 * \return 1 si le personnage est mort
 * \return 0 si le personnage est vivant
 * \brief Permet de savoir si un personnage est mort ou non.
 *
 */
/*retourne 1 si le le personnage est mort, 0 sinon */
int est_mort(t_perso perso){
	if(perso.i_HP == 0) return 1;
	else return 0;
}
