#include "grid.h"

#include <stdio.h>
#include <stdlib.h>

void grid_create(struct grid *self, size_t size){
    self->size = size;
    if (size == 0){
        self->data = NULL;
        return;
    }
    self->data = calloc(size, sizeof(char*));
    for (size_t y = 0; y < size; ++y){
        self->data[y] = calloc(size, sizeof(char));
        for (size_t x = 0; x < size; ++x){
            self->data[y][x] = CELL_EMPTY;
        }
    }
}

void grid_destroy(struct grid *self){
    for (size_t y = 0; y < self->size; ++y){
        free(self->data[y]);
    }
    free(self->data);
    self->data = NULL;
    self->size = 0;
}

size_t grid_get_size(const struct grid*self){
    return self->size;
}

bool grid_place_boat_if_no_collision(struct grid *self, size_t x, size_t y, size_t length, bool vertical){
    bool all_cells_empty = true;
    if (vertical){
        for(size_t j = y; j < y + length && all_cells_empty; ++j){
            if (self->data[j][x] != CELL_EMPTY){
                all_cells_empty = false;
            }
        }
        if (all_cells_empty){
            for(size_t j = y; j < y + length; ++j){
                self->data[j][x] = CELL_OCCUPIED;
            }
        }
    }
    else {
        for(size_t i = x; i < x + length && all_cells_empty; ++i){
            if (self->data[y][i] != CELL_EMPTY){
                all_cells_empty = false;
            }
        }
        if (all_cells_empty){
            for(size_t i = x; i < x + length; ++i){
                self->data[y][i] = CELL_OCCUPIED;
            }
        }
    }
    return all_cells_empty;
}

void grid_print(const struct grid *self){
    printf("\n");
    printf("\t   ");
    for (size_t x = 0; x < self->size; ++x){
        printf("%zu ", x);
    }
    printf("\n");
    printf("\t   ");
    for (size_t x = 0; x < self->size; ++x){
        printf("- ");
    }
    printf("\n");

    for (size_t y = 0; y < self->size ; ++y){
        /* Affichage d'une ligne */
        printf("\t%zu |", y);
        for (size_t x = 0; x < self->size; ++x){
            printf("%c", self->data[y][x]);
            if (x != self->size -1){
                printf(" ");
            }
        }
        printf("| %zu\n", y);
    }
    printf("\t   ");
    for (size_t x = 0; x < self->size; ++x){
        printf("- ");
    }
    printf("\n");
    printf("\t   ");
    for (size_t x = 0; x < self->size; ++x){
        printf("%zu ", x);
    }
    printf("\n\n");
}
