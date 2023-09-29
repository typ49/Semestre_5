// SR   |   TP3 | 2023-2024 | L3 informatique
// Julien gauhtier TP1A

// compilation : gcc -Wall -Werror -std=c99 -o TP3 TP3.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// -    -   -   -   -   -   -   -   -   -   -   -   -   -   -|
// Exercice 1 : fonction en parametre

// multiplication
int mult (int x, int fact) {
    return x*fact;
}

// addition
int add (int x, int fact) {
    return x+fact;
}

// Distribue une fontion sur un tableau
void map (int *tab, int size, int (*f)(int, int), int fact) {
    for (int i = 0; i < size; i++) {
        tab[i] = f(tab[i], fact);
    }

}


// -    -   -   -   -   -   -   -   -   -   -   -   -   -   -|
// Exercice 2 : Affichage concurent

typedef struct {
    int nbAfficahges;
} threadAffichage;

void* affichageIDThreads( void* arg) {
    int nbAffichage = *((int*) arg);
    int i;
    for (i=0; i< nbAffichage ;i++) {
        printf("Thread ID : %lu, Affichage n° : %d\n\n", pthread_self(), i+1);
        sleep(1);
    }
    scanf("%d", &i);
    return NULL;
}


// -    -   -   -   -   -   -   -   -   -   -   -   -   -   -|
// Main

int main() {

    // ex 1
    int tab[3] = {1,3,5};
    int tab2[3] = {1,3,5};
    map(tab, 3, mult, 10);
    map (tab2, 3, add, 10);
    for (int i = 0; i < 3; i++) {
        printf("%d\n", tab[i]);
    }
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf("%d\n", tab2[i]);
    }
    printf("\n");
    // ex 2

    // variables
    pthread_t thread1, thread2;
    int retour1, retour2;

    int nbAffichage1 = 5;
    int nbAffichage2 = 10;

    // creation des threads
    retour1 = pthread_create(&thread1, NULL, affichageIDThreads, (void*)&nbAffichage1);
    if (retour1 != 0) {
        printf("Erreur de creation de thread\n");
        exit(1);
    }
    retour2 = pthread_create(&thread2, NULL, affichageIDThreads, (void*)&nbAffichage2);
    if (retour2 != 0) {
        printf("Erreur de creation de thread\n");
        exit(1);
    }

    // attente de la fin des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}