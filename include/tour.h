#ifndef __TOUR_H__
#define __TOUR_H__

#include "map.h"
#include "perso.h"

void menu_principal();
void tour();
void actio(t_perso *perso);
void partie();
int victoire();

void partieIA();
void call_IA_play(char src[50]);
#endif
