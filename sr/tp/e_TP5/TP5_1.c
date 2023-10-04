// SR   |   TP5_PART_1 | 2023-2024 | L3 informatique
// Julien gauhtier TP1A

// compilation : gcc -Wall -Werror -Wextra -pthread -o TP5_1 TP5_1.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROWS 5
#define COLS 5
#define MAX_VALUE 100

struct Matrix
{
    int rows;
    int cols;
    int **data;
};

struct ThreadArgs
{
    int *row;
    int nbElements;
    int sum;
};

// Ajout d'un mutex global pour synchroniser l'accès à la somme finale
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex = mutal exclusion
int globalSum = 0; // Somme finale partagée par tous les threads

void *computeRowSum(void *arg)
{
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)arg;
    int *row = threadArgs->row;
    int sum = 0;

    for (int j = 0; j < threadArgs->nbElements; j++)
    {
        sum += row[j];
    }

    // Utilisation du mutex pour ajouter la somme de la ligne à la somme finale
    pthread_mutex_lock(&sum_mutex);
    globalSum += sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}


void *initializeRow(void *arg)
{
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)arg;
    int *row = threadArgs->row;

    for (int j = 0; j < threadArgs->nbElements; j++)
    {
        row[j] = rand() % MAX_VALUE;
    }

    return NULL;
}

void affiche_MATRIX(struct Matrix matrix)
{
    int aff_rows = matrix.rows * 2 + (matrix.rows - 1);
    printf("+");
    for (int i = 0; i < aff_rows; i++)
    {
        printf("-");
    }
    printf("+\n");
    for (int i = 0; i < matrix.cols; i++)
    {
        printf("|");
        for (int j = 0; j < matrix.rows; j++)
        {
            printf("%2d|", matrix.data[i][j]);
        }
        printf("\n");
    }
    printf("+");
    for (int i = 0; i < aff_rows; i++)
    {
        printf("-");
    }
    printf("+\n");
}

int main()
{
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires
    struct Matrix matrix;
    matrix.rows = ROWS;
    matrix.cols = COLS;
    matrix.data = malloc(ROWS * sizeof(int *));

    for (int i = 0; i < ROWS; i++)
    {
        matrix.data[i] = (int *)malloc(COLS * sizeof(int));
    }

    pthread_t threads[ROWS];
    struct ThreadArgs threadArgs[ROWS];

    // Créer un thread pour chaque ligne de la matrice pour l'initialiser
    for (int i = 0; i < ROWS; i++)
    {
        threadArgs[i].row = matrix.data[i];
        threadArgs[i].nbElements = COLS;
        pthread_create(&threads[i], NULL, initializeRow, &threadArgs[i]);
    }

    // Attendre que tous les threads d'initialisation aient terminé
    for (int i = 0; i < ROWS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Créer des threads pour calculer la somme des éléments de chaque ligne
    for (int i = 0; i < ROWS; i++)
    {
        pthread_create(&threads[i], NULL, computeRowSum, &threadArgs[i]);
    }

    // Attendre que tous les threads de somme aient terminé
    for (int i = 0; i < ROWS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Afficher la matrice
    affiche_MATRIX(matrix);

    printf("Somme totale : %d\n", globalSum); // Utilisation de globalSum

    for (int i = 0; i < ROWS; i++)
    {
        free(matrix.data[i]);
    }

    free(matrix.data);

    return 0;
}
