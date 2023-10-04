// SR   |   TP5_PART_2 | 2023-2024 | L3 informatique
// Julien gauhtier TP1A

// compilation : gcc -Wall -Werror -Wextra -pthread -o TP5_2 TP5_2.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h> // Ajout pour les sémaphores POSIX

#define ROWS 5
#define COLS 5
#define MAX_VALUE 100

struct Matrix {
    int rows;
    int cols;
    int **data;
};

struct ThreadArgs {
    int *row;
    int nbElements;
    int *sum; // Pointeur vers la somme finale
    sem_t *semaphore; // Pointeur vers le sémaphore
};

sem_t sum_semaphore; // Sémaphore pour synchroniser l'accès à la somme finale

void *computeRowSum(void *arg) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)arg;
    int *row = threadArgs->row;
    int sum = 0;

    for (int j = 0; j < threadArgs->nbElements; j++) {
        sum += row[j];
    }

    // Utilisation du sémaphore pour ajouter la somme de la ligne à la somme finale
    sem_wait(threadArgs->semaphore);
    *(threadArgs->sum) += sum;
    sem_post(threadArgs->semaphore);

    return NULL;
}

void *initializeRow(void *arg) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)arg;
    int *row = threadArgs->row;

    for (int j = 0; j < threadArgs->nbElements; j++) {
        row[j] = rand() % MAX_VALUE;
    }

    return NULL;
}

void affiche_MATRIX(struct Matrix matrix) {
    int aff_rows = matrix.rows * 2 + (matrix.rows - 1);
    printf("+");
    for (int i = 0; i < aff_rows; i++) {
        printf("-");
    }
    printf("+\n");
    for (int i = 0; i < matrix.cols; i++) {
        printf("|");
        for (int j = 0; j < matrix.rows; j++) {
            printf("%2d|", matrix.data[i][j]);
        }
        printf("\n");
    }
    printf("+");
    for (int i = 0; i < aff_rows; i++) {
        printf("-");
    }
    printf("+\n");
}

int main() {
    srand(time(NULL));
    struct Matrix matrix;
    matrix.rows = ROWS;
    matrix.cols = COLS;
    matrix.data = malloc(ROWS * sizeof(int *));

    int localGlobalSum = 0; // Somme finale locale (pas globale)

    sem_init(&sum_semaphore, 0, 1); // Initialisation du sémaphore

    for (int i = 0; i < ROWS; i++) {
        matrix.data[i] = (int *)malloc(COLS * sizeof(int));
    }

    pthread_t threads[ROWS];
    struct ThreadArgs threadArgs[ROWS];

    // Créer un thread pour chaque ligne de la matrice pour l'initialiser
    for (int i = 0; i < ROWS; i++) {
        threadArgs[i].row = matrix.data[i];
        threadArgs[i].nbElements = COLS;
        threadArgs[i].sum = &localGlobalSum; // Passer un pointeur vers la somme finale
        threadArgs[i].semaphore = &sum_semaphore; // Passer un pointeur vers le sémaphore
        pthread_create(&threads[i], NULL, initializeRow, &threadArgs[i]);
    }

    // Attendre que tous les threads d'initialisation aient terminé
    for (int i = 0; i < ROWS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Créer des threads pour calculer la somme des éléments de chaque ligne
    for (int i = 0; i < ROWS; i++) {
        pthread_create(&threads[i], NULL, computeRowSum, &threadArgs[i]);
    }

    // Attendre que tous les threads de somme aient terminé
    for (int i = 0; i < ROWS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Afficher la matrice
    affiche_MATRIX(matrix);

    printf("Somme totale : %d\n", localGlobalSum); // Utilisation de localGlobalSum

    for (int i = 0; i < ROWS; i++) {
        free(matrix.data[i]);
    }

    free(matrix.data);

    sem_destroy(&sum_semaphore); // Destruction du sémaphore

    return 0;
}
