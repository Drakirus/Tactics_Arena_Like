/**
 * \file action.c
 * \brief Module contenant toutes les actions du jeu (déplacement, attaque)
 * \author CHAMPION Pierre
 * \version 0.1
 * \date 18 décembre 2015
 *
 *
 */


//Ce module servira à créer la map de base dans laquelle les personnages vont se battre.

#include "../include/action.h"
#include "../include/tableau.h"
#include "../include/map.h"
#include "../include/perso.h"
#include "../include/list_attack.h"
#include "../include/SDL_isometric.h"


extern type_Map tMap;
extern t_context *ingame;

extern int map[i_taille_map][i_taille_map];
extern int i_perso_actuel;

/*
* ===========================================
*	FONCTION DE DÉPLACEMENT
* ===========================================
*/

/**
 * \fn void delay(int milliseconds)
 * \param milliseconds temps en milliseconds
 * \return
 * \brief delay est utilisé pour suspendre l'exécution d'un programme pendant un temps déterminé
 *
 */
void delay(int milliseconds){
  long pause;
  clock_t now,then;
  pause = milliseconds*(CLOCKS_PER_SEC/1000);
  now = then = clock();
  while( (now-then) < pause )
  now = clock();
}

/**
 * \fn void deplacement(int * PM_actuel)
 * \param PM_actuel
 * \return
 * \brief déplace le personnage avec un animation à l'écran
 *
 */
void deplacement(int * PM_actuel){
  pile *path = NULL; // move personnage
  int r,c; // move personnage
  int sortir; // move personnage
  path = getMovePerso(PM_actuel, tab_perso[i_perso_actuel].coord[0], tab_perso[i_perso_actuel].coord[1] );
  if (path == NULL) printf("Déplacement imposible\n");
  else{
    sortir = pop(&path, &r, &c);
    afficher_map();
    while (sortir != -1) {
      sortir = pop(&path, &r, &c);
      // fprintf(stderr, "%i %i r: %i c: %i\n",tab_perso[i_perso_actuel].coord[0], tab_perso[i_perso_actuel].coord[1] ,r,c );
      if (tab_perso[i_perso_actuel].coord[0] > r) {
        moveSpriteTo(ingame, tMap, UP_LEFT, tab_perso[i_perso_actuel].id );
        change_nombre(5, &tab_perso[i_perso_actuel], r);
      }
      if (tab_perso[i_perso_actuel].coord[0] < r) {
        moveSpriteTo(ingame, tMap, DOWN_RIGHT, tab_perso[i_perso_actuel].id );
        change_nombre(5, &tab_perso[i_perso_actuel], r);
      }
      if (tab_perso[i_perso_actuel].coord[1] > c) {
        moveSpriteTo(ingame, tMap, UP_RIGHT, tab_perso[i_perso_actuel].id );
        change_nombre(6, &tab_perso[i_perso_actuel], c);
      }
      if (tab_perso[i_perso_actuel].coord[1] < c) {
        moveSpriteTo(ingame, tMap, DOWN_LEFT, tab_perso[i_perso_actuel].id );
        change_nombre(6, &tab_perso[i_perso_actuel], c);        
      }    
    }
    afficher_map();
  }
}

/**
 * \fn pile *getMovePerso(int * PM_tour, int start_r,int start_c)
 * \param PM_tour
 * \param start_r
 * \param start_c
 * \return une pile contenant les coordonnées de déplacement
 * \brief demande à l'utilisateur des coordonnées pour déplacer le personnage actuel différent test sont effectué
 *
 */
pile *getMovePerso(int * PM_tour, int start_r,int start_c){
  if (*PM_tour == 0) {
    return NULL;
  }
  int coord_r,coord_c;
  pile *path;
  int **DistancePath = createDistancePath(start_r, start_c);
  int distanceOfPath;
  int range_max = *PM_tour;
  int range_min = 0;
  int map_shadowcasting[i_taille_map][i_taille_map];
  int i,j;
  for(i = 0 ; i < i_taille_map ; i ++)
    for(j = 0 ; j < i_taille_map ; j ++)
       map_shadowcasting[i][j] = 0;

  afficher_map_accessible(DistancePath, map_shadowcasting, range_max, range_min, 0,0,0); // les trois 0 à la fin sont présent si un jour, un personnage n'est capable de ce déplacer uniquement sur les lignes
  do {
    printf("Choisissez les coordonnées (x y) (-1 -1 pour annuler): ");
    // scanf("%i%i", &coord_r, &coord_c);
    for (i = 0; i < i_taille_map; i++) {
      for (j = 0; j < i_taille_map; j++) {
        if (DistancePath[i][j] <= range_max && DistancePath[i][j] >= range_min && map_shadowcasting[i][j] == 0) {
          drawTileplace(ingame, tMap, i* TILE_W, j* TILE_H);
        }
      }
    }
    SDL_generate(ingame);
    GetClick(ingame, tMap, &coord_r, &coord_c);
    printf("%i%i\n", coord_r, coord_c);
    if (coord_r == -1 || coord_c == -1) {
      return NULL;
    }
    if (coord_r >= i_taille_map || coord_c >= i_taille_map || coord_r<0 || coord_c<0) {
      printf("Coordonnée non Valide\n");
    }else{
      if (coord_r < i_taille_map && coord_c < i_taille_map && coord_r>=0 && coord_c>=0) {
        distanceOfPath = distanceFrom(coord_r, coord_c, DistancePath);
        if (map[coord_r][coord_c] == 1) {
          printf("Déplacement non possible sur un obstacle\n");
        }
      }
      if ( distanceOfPath > *PM_tour) {
        printf("Trop loin\n");
      }
    }
  } while(coord_r > i_taille_map || coord_c > i_taille_map || coord_r<0 || coord_c<0 || distanceOfPath > *PM_tour ||  map[coord_r][coord_c] != 0);

  if (distanceOfPath>0) {
    *PM_tour -=distanceOfPath;
    //  displayBoard(i_taille_map, i_taille_map,DistancePath);
    path = getPath(DistancePath, coord_r, coord_c);
    freeBoard(DistancePath, i_taille_map);
    return path;
  }
  freeBoard(DistancePath, i_taille_map);
  return NULL;
}
/*
* ===========================================
*	FONCTION D'ATTAQUE
* ===========================================
*/

/**
 * \fn void attaque(int * PA_tour)
 * \param PA_tour
 * \brief fonction qui applique les effect d'une attaque à un personnage, la le recul est géré, tout comme les dommages de zone, les heal ne dépace les pv max.
 *
 */
void attaque(int * PA_tour){

  if (*PA_tour == 0 ) {
    printf("plus de PA dispo\n");
    return;
  }
  int sortie=1; // boucle selection des attaque
  int attack=0; // choix de l'attaque
  int coord_r, coord_c; // coordonnées x et y d
  int map_shadowcasting[i_taille_map][i_taille_map]; // map qui affiche si une zone est cachée ou pas
  int i,j;
  int **DistancePath = createDistancePath( tab_perso[i_perso_actuel].coord[0],  tab_perso[i_perso_actuel].coord[1]); // creation des distances
  t_attak * tmp_att =NULL; // initialisation de l'ATTAQUE

  int range_max;
  int range_min;
  int ligne;
  do{
    displaylistAttack(tab_perso[i_perso_actuel].att);
    printf("Choisissez une attaque entre (-1 pour annuler) : ");
    attack =  choseAttak(ingame, tab_perso[i_perso_actuel].att);
    // scanf("%i", &attack);
    if (attack == -1 || attack == 0 ||getCountAttack(tab_perso[i_perso_actuel].att) == 0) {
      return;
    }
  }while( attack<1 || attack>getCountAttack(tab_perso[i_perso_actuel].att));

  for ( i = 0; i < i_taille_map ; i++) {
    for ( j = 0; j < i_taille_map ; j++) {
      map_shadowcasting[i][j] = map[i][j];
    }
  }

  shadowcasting(map_shadowcasting, tab_perso[i_perso_actuel].coord[0], tab_perso[i_perso_actuel].coord[1]);

  tmp_att = getAttack(tab_perso[i_perso_actuel].att, attack);

  if (tmp_att == NULL) {
    printf("Attaque non trouvée\n" );
    delay(1200);
    return;
  }
  range_max = tmp_att->range_max;
  range_min = tmp_att->range_min;
  ligne = tmp_att->only_line;
  afficher_map_accessible(DistancePath, map_shadowcasting, range_max, range_min, ligne , tab_perso[i_perso_actuel].coord[0] , tab_perso[i_perso_actuel].coord[1] );
  
  for (i = 0; i < i_taille_map; i++) {
    for (j = 0; j < i_taille_map; j++) {
      if (DistancePath[i][j] <= range_max && DistancePath[i][j] >= range_min && map_shadowcasting[i][j] == 0) {
        if (ligne == 1) {
          if (( tab_perso[i_perso_actuel].coord[0] == i || tab_perso[i_perso_actuel].coord[1] == j)) {
            // fprintf(stderr, "%i == %i / %i == %i\n",r_start, i , c_start , j );
            drawTileplace(ingame, tMap, i* TILE_W, j* TILE_H);
          }
        }else {
          drawTileplace(ingame, tMap, i* TILE_W, j* TILE_H);
        }
      }
    }
  }
  SDL_generate(ingame);

  while(sortie!=0){
    printf("Veuillez rentrer les coordonnées du personnages que vous voulez attaquer (-1 -1 pour annuler) : ");
    // scanf("%i%i", &coord_r, &coord_c);
    GetClick(ingame, tMap, &coord_r, &coord_c);

    if (coord_r == -1 || coord_c == -1) {
      return;
    }

    if (coord_r>=0 && coord_r<=i_taille_map && coord_c>=0 && coord_c<=i_taille_map) {
      if (0 != map_shadowcasting[coord_r][coord_c]) {
        printf("Ligne de vue obstruée\n" );
        delay(1200);
        return;
      }
    }
    // displaylistAttack(tab_perso[i_perso_actuel].att);

    if(coord_r>=0 && coord_r<=i_taille_map && coord_c>=0 && coord_c<=i_taille_map && !(map_shadowcasting[coord_r][coord_c] != 0)){

      if (*PA_tour < tmp_att->cost_PA ) {
        printf("pas assez de PA dispo\n");
        delay(1200);
        return;
      }
      if (distanceFrom(coord_r, coord_c, DistancePath) < tmp_att->range_min) {
        printf("Cible trop proche\n" );
        delay(1200);
        return;
      }
      if ( distanceFrom(coord_r, coord_c, DistancePath) > tmp_att->range_max) {
        printf("Cible trop loin\n" );
        delay(1200);
        return;
      }
      if (tmp_att->only_line == 1 &&
      (coord_r - tab_perso[i_perso_actuel].coord[0] != 0 &&  coord_c - tab_perso[i_perso_actuel].coord[1] != 0 )) {
        printf("Cible doit sur la même ligne\n" );
        delay(1200);
        return;
      }
      if (dealAttack(tmp_att, coord_r, coord_c)) {
        SDL_newText(ingame, NULL, "Cible touchee", colorGreenLight, 20,20);
        SDL_generate(ingame);
        
        SDL_Delay(2000);
        SDL_delText(ingame, ingame->nbText-1 );
        SDL_generate(ingame);
        
      }
      sortie = 0;

    }
  }
  *PA_tour -= tmp_att->cost_PA;
  freeBoard(DistancePath, i_taille_map);
}


int  dealAttack(t_attak * tmp_att, int coord_r, int coord_c){
  int dx,dy; // pour les dommages de zone
  int splashRange = (tmp_att->splash_range)-1;
  int recul_r; // gestion du recu
  int recul_c;
  int HP;
  int shoot; // si le tir est effectuer
  int returnValue = 0;


  if (splashRange >= 1) {
    dy = 0;
    dy = 0;
    for (dx = -splashRange; dx <= splashRange; dx++) {
      for (dy = -splashRange; dy <= splashRange; dy++){
        // printf("looking for player in %i %i\n",coord_r+dx,coord_c+dy );
        if (coord_r+dx>=0 && coord_c+dy>=0 && coord_r+dx<i_taille_map && coord_c+dy<i_taille_map) {
          if (recherche_perso_tab(coord_r+dx, coord_c+dy) != -1) {

            augmente_nombre(1, &tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)], tmp_att->trait.HP_max );
            HP = tmp_att->trait.HP;
            if (HP>0) {
              while (tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)].i_HP_max < HP + tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)].i_HP ) {
                HP--;
                // printf("%i \n",HP );
              }
            }

            augmente_nombre(2, &tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)], HP );
            augmente_nombre(3, &tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)], tmp_att->trait.PA );
            augmente_nombre(4, &tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)], tmp_att->trait.PM );
            printf("\tCible touchée\n");
            afficher_perso(tab_perso[recherche_perso_tab(coord_r+dx, coord_c+dy)]);
            returnValue = 1;
          }
        }
      }
    }
  }else{ // une attaque de zone ne peut affectée la position d'un personnage

    // printf("looking for player in %i %i\n",coord_r,coord_c );
    if (coord_r>=0 && coord_c>=0 && coord_r<i_taille_map && coord_c<i_taille_map) {
      if (recherche_perso_tab(coord_r, coord_c) != -1) {

        augmente_nombre(1, &tab_perso[recherche_perso_tab(coord_r, coord_c)], tmp_att->trait.HP_max );
        HP = tmp_att->trait.HP;
        if (HP>0) {
          while (tab_perso[recherche_perso_tab(coord_r, coord_c)].i_HP_max < HP + tab_perso[recherche_perso_tab(coord_r, coord_c)].i_HP ) {
            HP--;
            // printf("%i \n",HP );
          }
        }

        augmente_nombre(2, &tab_perso[recherche_perso_tab(coord_r, coord_c)], HP );
        augmente_nombre(3, &tab_perso[recherche_perso_tab(coord_r, coord_c)], tmp_att->trait.PA );
        augmente_nombre(4, &tab_perso[recherche_perso_tab(coord_r, coord_c)], tmp_att->trait.PM );

        shoot=1;

        recul_r = tmp_att->trait.coord_r;
        recul_c = tmp_att->trait.coord_c;

        while (shoot) {
          // printf("%i\n", map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] - recul_r][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] - recul_c]);
          // printf("%i\n", map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] + recul_r][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] + recul_c]);
          if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] > tab_perso[i_perso_actuel].coord[0]) {
            if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] - recul_r >= 0 && tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] - recul_r < i_taille_map && map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] - recul_r][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] ] != 1) {
              returnValue = 1;
              augmente_nombre( 5 , &tab_perso[recherche_perso_tab(coord_r, coord_c)] , (recul_r * -1) );
              
              
              shoot =0;
            }
            recul_r++;

          }else
          if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] > tab_perso[i_perso_actuel].coord[1]) {
            if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] - recul_c >= 0 && tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] - recul_c < i_taille_map && map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] ][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] - recul_c] != 1 ) {
              returnValue = 1;
              augmente_nombre( 6 , &tab_perso[recherche_perso_tab(coord_r, coord_c)] , (recul_c * -1) );
              
              
              shoot =0;
            }
            recul_c++;

          }
          if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] < tab_perso[i_perso_actuel].coord[0]  ) {
            if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] + recul_r >= 0 && tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] + recul_r < i_taille_map && map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0] + recul_r][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1]] != 1 ) {
              returnValue = 1;
              augmente_nombre( 5 , &tab_perso[recherche_perso_tab(coord_r, coord_c)] , recul_r );
              
              
              shoot =0;

            }
            recul_r++;

          }else
          if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] < tab_perso[i_perso_actuel].coord[1] ) {
            if (tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] + recul_c >= 0 && tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] + recul_c < i_taille_map && map[tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[0]][tab_perso[recherche_perso_tab(coord_r, coord_c)].coord[1] + recul_c] != 1) {
              returnValue = 1;
              augmente_nombre( 6 , &tab_perso[recherche_perso_tab(coord_r, coord_c)] , recul_c );
              
              
              shoot =0;
            }
            recul_c++;
          }
        }
      }

    }
  }
  return returnValue;
}

/*
* ===========================================
*	FONCTION LIGNE DE VUE
http://krosmaster.herazade.com/
http://www.roguebasin.com/index.php?title=FOV_using_recursive_shadowcasting
* ===========================================
*/

/**
 * \fn float getSlope(float f_a, float f_b)
 * \param f_a
 * \param f_b
 * \return le résultat de la division
 * \brief Dans le but d'évité le cas de la division pas 0
 *
 */
float getSlope(float f_a, float f_b){ // Basic division function (with check)
  if(f_b == 0){
    return 99;
  }
  return f_a/f_b;
}

/**
 * \fn shadowcasting(int matrix[i_taille_map][i_taille_map], int i_player_x, int i_player_y)
 * \param matrix[i_taille_map][i_taille_map]
 * \param i_player_x
 * \param i_player_y
 * \brief crée une map ou les cellules attaquable sont égal à 0
 *
 */
void shadowcasting(int matrix[i_taille_map][i_taille_map], int i_player_x, int i_player_y){
  int i,j,cx,cy;
  float f_slope_min, f_slope_max, f_slope;
  for (i = 0; i < i_taille_map; i++) {
    for (j = 0; j < i_taille_map; j++) {
      if (matrix[i][j] != 0) {

        if (i_player_x <= i && i_player_y <= j ) { // en bas a droite

          f_slope_min = getSlope(((i_player_y+0.5)-j)	 , (i_player_x+0.5)-(i+1));
          f_slope_max = getSlope(((i_player_y+0.5)-(j+1))	 , (i_player_x+0.5)-i);
          // printf("\n %.2f slope min  %.2f slope max %i i %i j  en bas a droite\n", f_slope_min, f_slope_max,(j),(i));   // ICI

          for (cy = i; cy < i_taille_map; cy++) {
            for (cx = j; cx < i_taille_map; cx++) {
              // printf(" %i:x %i:y", cx,cy);
              f_slope = getSlope(((i_player_y)-cx), ((i_player_x)-cy));
              // printf(" slope %.2f \n",f_slope);
              if (cy != i || cx != j) {

                if((f_slope > f_slope_min ) && (f_slope < f_slope_max || (f_slope_max < 0) )  ){
                  // printf("slope %f\n", f_slope);
                  if (matrix[cy][cx] != 1) {
                    matrix[cy][cx] = 3;
                  }
                }
              }
            }
          }
        }

        if (i_player_x >= i && i_player_y >= j ) { // en haut a gauche

          f_slope_min = getSlope(((i_player_y+0.5)-j)	 , (i_player_x+0.5)-(i+1));
          f_slope_max = getSlope(((i_player_y+0.5)-(j+1))	 , (i_player_x+0.5)-i);
          // printf("\n %.2f slope min  %.2f slope max %i i %i j  en haut a gauche\n", f_slope_min, f_slope_max,(j),(i));   // ICI

          for (cy = i; cy > -1; cy--) {
            for (cx = j; cx > -1; cx--) {
              // printf(" %i:x %i:y", cx,cy);
              f_slope = getSlope(((i_player_y)-cx), ((i_player_x)-cy));
              // printf(" slope %.2f \n",f_slope);
              if (cy != i || cx != j) {

                if((f_slope < f_slope_min  || (f_slope_min < 0 ) ) && (f_slope > f_slope_max)){
                  // printf("slope %f\n", f_slope);
                  if (matrix[cy][cx] != 1) {
                    matrix[cy][cx] = 3;
                  }
                }
              }
            }
          }
        }

        if (i_player_x >= i && i_player_y <= j ) { // en haut a droite

          f_slope_max = getSlope(((i_player_y-0.5)-j)	 , (i_player_x+0.5)-(i+1));
          f_slope_min = getSlope(((i_player_y-0.5)-(j-1))	 , (i_player_x+0.5)-i);
          // printf("\n %.2f slope min  %.2f slope max %i i %i j  en haut a droite\n", f_slope_min, f_slope_max,(j),(i));   // ICI
          for (cy = i; cy > -1; cy--) {
            for (cx = j; cx < i_taille_map; cx++) {
              // printf(" %i:x %i:y", cx,cy);
              f_slope = getSlope(((i_player_y)-cx), ((i_player_x)-cy));
              // printf(" slope %.2f \n",f_slope);
              if (cy != i || cx != j) {

                if((f_slope < f_slope_min) && (f_slope > f_slope_max || (f_slope_max > 0) )){
                  // printf("slope %f\n", f_slope);
                  if (matrix[cy][cx] != 1) {
                    matrix[cy][cx] = 3;
                  }
                }
              }
            }
          }
        }

        if (i_player_x <= i && i_player_y >= j ) { // en bas a gauche

          f_slope_max = getSlope(((i_player_y+0.5)-j)	 , (i_player_x-0.5)-(i-1));
          f_slope_min = getSlope(((i_player_y+0.5)-(j+1))	 , (i_player_x-0.5)-i);
          // printf("\n %.2f slope min  %.2f slope max %i i %i j  en bas a gauche \n", f_slope_min, f_slope_max,(j),(i));   // ICI

          for (cy = i; cy < i_taille_map; cy++) {
            for (cx = j; cx > -1; cx--) {
              // printf(" %i:x %i:y", cx,cy);
              f_slope = getSlope(((i_player_y)-cx), ((i_player_x)-cy));
              // printf(" slope %.2f \n",f_slope);
              if (cy != i || cx != j) {

                if((f_slope < f_slope_min ) && (f_slope > f_slope_max || (f_slope_max > 0) )){
                  // printf("slope %f\n", f_slope);
                  if (matrix[cy][cx] != 1) {
                    matrix[cy][cx] = 3;
                  }
                }
              }
            }
          }
        }

      }
    }
  }
}
/*
* ===========================================
*	FONCTION PATHFINDING
* http://www.redblobgames.com/pathfinding/tower-defense/
* ===========================================
*/

/**
 * \fn int setAdjacent(int **mat, int i, int j, int value)
 * \param mat
 * \param i
 * \param j
 * \param value
 * \return le nombre de case changée
 *
 */
int setAdjacent(int **mat, int i, int j, int value) {
  /*
  * setAdjacent is a function call by (only) *createDistancePath*, it takes a *X* and *Y* coordinates, and print *value* to here direct neighbor.
  * *mat* is the output matix. This function checks if it can place a neighbor or not (overflow), and if the new neighbor can have other(exemple : the corner).
  */
  int dx,dy;
  int nbUpdate = 0;
  for (dx = (i <= 0 ? 0 : -1); dx <= (i >= i_taille_map-1 ? 0 : 1); dx++)
  for (dy = (j <= 0 ? 0 : -1); dy <= (j >= i_taille_map-1 ? 0 : 1); dy++){
    // printf("coord adja %i %i\n",dx+i,dy+j );
    if (abs(dx) != abs(dy) && mat[dx+i][dy+j] == -1) {
      mat[dx+i][dy+j] = value;
      nbUpdate++;
    }
  }
  return nbUpdate;
}

/**
 * \fn int **createDistancePath(int init_x, int init_y)
 * \param init_x
 * \param init_y
 * \return retourne une matrice contenant les distance d'un points A à un point B
 *
 */
int **createDistancePath(int init_x, int init_y){
  /*
  * *createDistancePath* return a matrix fill of distances, the center point have *init_x* and *init_y* coordinate.
  * if the matrix return have for one coordinate a value of 0 it mean that the cells is unreachable, (surrender by obstacle or stand on it)
  */
  int **map_path_finding =allocateBoard(i_taille_map,i_taille_map,map);

  int row=0, columns=0, actual=0, nbUpdate = 1;

  map_path_finding[init_x][init_y] = 0;

  // init mat : point de départ = valeur_actuelle
  // displayBoard(i_taille_map,i_taille_map,map_path_finding);


  // tant que cible non atteinte ou matrice pas pleine
  while(nbUpdate != 0){
    nbUpdate = 0;
    // parcours de la matrice
    for ( row=0; row<i_taille_map; row++)
    for( columns=0; columns<i_taille_map; columns++){

      // si case = valeur_actuelle
      if (map_path_finding[row][columns] == actual) {
        // fprintf(stderr, "%i == %i",map_path_finding[row][columns],  actual);
        // case_adjacente = vleur_actuelle +1
        nbUpdate += setAdjacent(map_path_finding, row, columns, actual+1);

        // displayBoard(i_taille_map,i_taille_map,map_path_finding);
        // printf("\n");

      }
    }
    // fprintf(stderr, "update %i\n", nbUpdate);
    actual++;
  }
  return map_path_finding;
}

/**
 * \fn void displayBoard(int r, int c, int **arr)
 * \param r
 * \param c
 * \param arr
 * \brief affiche une matrice définie en pointer de pointer. pour le debuging
 *
 */
void displayBoard(int r, int c, int **arr){ // Display a matrix of size r,c for debuging
  int i,j;
  for (i = 0; i <  r; i++){
    for (j = 0; j < c; j++){
      if (arr[i][j] == -2)
      printf("   ");
      else
      printf("%2d ", arr[i][j]);
    }
    printf("\n" );
  }
}

/**
 * \fn int **allocateBoard(int r, int c, int from[r][c])
 * \param r
 * \param c
 * \param from[r][c]
 * \return creé une matrice de pointer de pointer
 * \brief
 *
 */
int **allocateBoard(int r, int c, int from[r][c]){// create a pointer array to a pointer array and return it
  int **arr;
  int i,j;

  arr = (int **)malloc(r * sizeof(int *));
  for (i=0; i<r; i++)
  arr[i] = (int *)malloc(c * sizeof(int));

  for (i = 0; i < r; i++)
  for (j = 0; j < c; j++){
    if (from[i][j] == 1)
    arr[i][j]=-2;
    else
    arr[i][j] = -1; // *(*(arr+i)+j)
  }
  return arr;
}

/**
 * \fn int distanceFrom(int to_x, int to_y,   int **DistancePath)
 * \param to_x
 * \param to_y
 * \param DistancePath
 * \return la valeur d'une case dans la matrice des distances
 * \brief pour une facilitée déconcertante
 *
 */
int distanceFrom(int to_x, int to_y, 	int **DistancePath){ //My teammates have so much chance
  return DistancePath[to_x][to_y];
}

/**
 * \fn void freeBoard(int **arr, int Rows)
 * \param arr
 * \param Rows
 * \brief https://www.youtube.com/watch?v=wQP9XZc2Y_c
 *
 */
void freeBoard(int **arr, int Rows){ //free the  dynamically allocated memory
  int row;
  // free de toutes les lignes
  for (row = 0; row < Rows; row++) {
    free(arr[row]);
  }
  // free de toutes les colones
  free(arr);
}

/**
 * \fn pile *getPath(int **DistancePath, int i, int j)
 * \param DistancePath
 * \param i
 * \param j
 * \return une pile de coordonnées
 * \brief   this function return the shortest path to a *i* *j* coordinate, where *DistancePath* is the distances matrix.
 the path (coordinate) is stored in a pile and return.
 *
 */
pile *getPath(int **DistancePath, int i, int j){
  /*
  * this function return the shortest path to a *i* *j* coordinate, where *DistancePath* is the distances matrix.
  * the path (coordinate) is stored in a pile and return.
  */
  pile *path = NULL;
  int dx,dy;
  int actual = DistancePath[i][j];
  actual--;
  push(&path, i, j);
  while(DistancePath[i][j] != 0){
    for (dx = (i <= 0 ? 0 : -1); dx <= (i >= i_taille_map-1 ? 0 : 1); dx++)
    for (dy = (j <= 0 ? 0 : -1); dy <= (j >= i_taille_map-1 ? 0 : 1); dy++){

      actual = DistancePath[i][j];
      actual--;
      if (abs(dx) != abs(dy) && actual == DistancePath[dx+i][dy+j]) {
        i = dx+i;
        j = dy+j;
        dx = 2;
        dy = 2;
        push(&path, i, j);
      }
    }
  }
  return path;
}
/*
* ===========================================
* FONCTION GESTION PILE
* ===========================================
*/

/**
 * \fn pile * createPile()
 * \return le prochain élémént de la pile
 *
 */
pile * createPile(){
  pile *tmp = malloc(sizeof(pile));
  if(!tmp) exit(EXIT_FAILURE); /* If the allocation failed. */
  return tmp;
}

/**
 * \fn void push(pile **p, int r, int c)
 * \param p
 * \param r
 * \param c
 * \return remplie la pile avec des coordonnées
 *
 */
void push(pile **p, int r, int c){
  pile *tmp = createPile();
  tmp->row = r;
  tmp->column = c;
  tmp->prec = *p;
  *p = tmp;
}

/**
 * \fn int pop(pile **p, int * r, int * c)
 * \param p
 * \param r
 * \param c
 * \return -1 si la pile est vide sinon 1
 * \brief dépile la pile avec des coordonnées
 *
 */
int pop(pile **p, int * r, int * c){
  pile *tmp;
  if(!*p) return -1;     /* Returns -1 if the pile is empty. */
  tmp = (*p)->prec;
  *r = (*p)->row;
  *c = (*p)->column;
  free(*p);
  *p = tmp;       /* The pointer points to the last element. */
  return 1;
}
