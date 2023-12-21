// SR   |   TP3 | 2023-2024 | L3 informatique
// Julien gauhtier TP1A

// compilation : gcc -Wall -Werror -std=c99 -o TP3_alea TP3_alea_tab.c -lpthread


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


// Exercice n°3 : Générateur de tableau unidimensionnel aléatoire

struct Tab {
    int* tab;
    int size;

};

struct LimiteAlea {
    int min;
    int max;
};

struct ThreadInitTab{
    struct Tab tab;
    struct LimiteAlea limite;
};

void* init_tab(void* arg) {
    struct ThreadInitTab* threadInitTab = (struct ThreadInitTab*) arg;
    int min = threadInitTab->limite.min;
    int max = threadInitTab->limite.max;
    int size = threadInitTab->tab.size;
    int* tab = threadInitTab->tab.tab;
    for (int i = 0; i < size; i++) {
        tab[i] = rand()%(max-min) + min;
    }
    return NULL;
}

void print_tab(struct Tab tab) {
    int size = tab.size;
    int* tab2 = tab.tab;
    for (int i = 0; i < size; i++) {
        printf("%d ", tab2[i]);
    }
    printf("\n");
}

int main (int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage : ./TP3_alea <taille du tableau> <borne inf> <borne sup>\n");
        return 1;
    }
    int size = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);
    if (min > max) {
        printf("La borne inf doit être inférieure à la borne sup\n");
        return 1;
    }
    struct Tab tab;
    tab.size = size;
    tab.tab = malloc(size*sizeof(int));
    struct LimiteAlea limite;
    limite.min = min;
    limite.max = max;
    struct ThreadInitTab threadInitTab;
    threadInitTab.tab = tab;
    threadInitTab.limite = limite;
    pthread_t thread;
    pthread_create(&thread, NULL, init_tab, &threadInitTab);
    pthread_join(thread, NULL);
    print_tab(tab);
    free(tab.tab);
    return 0;
}