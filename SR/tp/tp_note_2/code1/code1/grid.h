#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Une struct grid représente une grille de jeu carrée, sur laquelle un certain nombre de
 * bateaux sont positionnés. Dans ce jeu, un bateau occupe des cases alignées soit verticalement
 * soit horizontalement. La longueur d'un bateau est égale au nombre de cases qu'il occupe.
 *
 * Une struct grid est caractérisée par :
 * - la longueur de ses côtés, notée L
 * - un ensemble de L*L cases qui peuvent être occupées ou non par un bateau
 *
 */

#define CELL_EMPTY ' '
#define CELL_OCCUPIED 'B'

struct grid {
  char **data; // pointeur permettant d'accéder aux cellules de la grille
  size_t size; // largeur d'un côté de la grille
};

/**
 * Initialise les champs d'une struct grid, préalablement allouée, pour créer une grille carrée vide
 * de longueur de côté égale à size.
 *
 * @param self pointeur sur l'objet de type struct grid
 * @param size longueur d'un côté de la grille
 *
 */
void grid_create(struct grid *self, size_t size);

/**
 * Libère l'espace mémoire occupé par les cellules de la grille.
 *
 * @param self pointeur sur l'objet de type struct grid
 */
void grid_destroy(struct grid *self);

/**
 * Renvoie la longueur d'un côté de la grille.
 *
 * @param self pointeur sur l'objet de type struct grid
 */
size_t grid_get_size(const struct grid*self);

/**
 * Place un bateau sur la grille si et seulement si toutes les cellules qu'il doit occuper sont préalablement libres.
 *
 * ATTENTION : cette fonction ne doit être appelée que pour un bateau qui reste dans les limites de la grille.
 *
 * @param self pointeur sur l'objet de type struct grid
 * @param x abscisse de la cellule la plus à gauche du bateau
 * @param y ordonnée de la cellule la plus haute du bateau
 * @param length longueur ou nombre de cellules occupées par le bateau
 * @param vertical orientation du bateau : true pour vertical, false pour horizontal
 *
 * @return true si le bateau a pu être placé sur la grille, false sinon
 */
bool grid_place_boat_if_no_collision(struct grid *self, size_t x, size_t y, size_t length, bool vertical);

/**
 * Affiche une représentation de la grille sur stdout.
 *
 * @param self pointeur sur l'objet de type struct grid
 */
void grid_print(const struct grid *self);


#endif // GRID_H
