#ifndef __PERSO_H__
#define __PERSO_H__

#define i_nombre_classe 4

#include "./list_attack.h"
#include "../include/list_attack.h"

typedef struct
{
	char s_classe[20];
	int i_HP_max;
	int i_HP;
	int i_PA;
	int i_PM;
	int coord[2];
	char c_team;
	list_attack * att;
	char file[20];
	int id;
}t_perso;

void afficher_perso(t_perso perso);
void affichage_perso(t_perso perso);
void creation(t_perso * per, int i_classe, char c_team, int x, int y);
void initialisation_perso(int a, t_perso * per);
void augmente_nombre(int a, t_perso * per, int i_montant);
void change_nombre(int a, t_perso * per, int i_montant);
int est_mort(t_perso perso);
int init_attack(t_perso *per);

#endif
