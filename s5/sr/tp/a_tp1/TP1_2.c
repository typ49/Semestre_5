/* SR | TP n°1 Part 2   |
 * Julien Gauthier      |
 * L3 TP1A              |
 */

// compilation : gcc -Wall -Werror -o TP1_2 TP1_2.c

/*
 * Le processus parent doit obtenir l’entier
 * genere aleatoirement entre deux bornes fixees (constantes) par son processus fils.
 * Il fera le calcul du carre de ce nombre, qu’il affichera sous la forme d’une expression
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>

#define BORNE_INF 0
#define BORNE_SUP 100

int main()
{
    // creation de la cle :
    key_t key = ftok("./", 'h');

    // creation du segment de memoire partagee :
    int shmID = shmget(key, sizeof(int) * 1024, IPC_CREAT | 0644);

    // obtention de l'adresse du segment :
    int *attachement = (int *)shmat(shmID, NULL, 0);
    // vérifier l'attachement :
    if (attachement == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // creation du processus fils :
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    // processus fils :
    if (pid == 0)
    {
        // creation nombre aleatoire :
        srand(time(NULL));
        int nombre = (rand() % (BORNE_SUP - BORNE_INF + 1)) + BORNE_INF;

        // ecriture dans le segment :
        *attachement = nombre;

        // detache le segment :
        if (shmdt(attachement) == -1)
        {
            perror("shmdt");
            exit(1);
        }

        // fin du processus fils :
        exit(0);
    }

    // processus pere :
    // attente de la fin du processus fils :
    wait(NULL);

    // mettre au carre le nombre du fils :
    int nombre = *attachement;
    int carre = nombre * nombre;

    // lecture dans le segment :
    printf("%d ^ 2 = %d\n", nombre, carre);

    // detache le segment :
    if (shmdt(attachement) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    // marquage du segment pour suppression :
    if (shmctl(shmID, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
