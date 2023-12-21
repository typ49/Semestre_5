// SR   |   TP4_PART_1 | 2023-2024 | L3 informatique
// Julien gauhtier TP1A

// compilation : gcc -Wall -Werror -Wextra -pthread -o TP4_1 TP4_1.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

// -    -   -   -   -   -   -   -   -   -   -   -   -   -   -|
// Exercice 2 : Affichage concurent

typedef struct
{
    int nbAffichages; // Correction de la faute de frappe
} threadAffichage;

void *affichageIDThreads(void *arg)
{
    int nbAffichage = *((int *)arg);
    int i;
    for (i = 0; i < nbAffichage; i++)
    {
        printf("Thread ID(pthread_self) : %lu, Thread ID(syscall) : %lu Affichage n° : %d\n\n", pthread_self(),syscall(SYS_gettid), i + 1);
        sleep(2); // Augmentation du délai à 2 secondes
    }
    printf("Appuyez sur une touche pour terminer ce thread...\n");
    getchar(); // Remplacement de scanf par getchar pour une saisie plus simple
    return NULL;
}

int main()
{
    // ex 2

    // variables
    pthread_t thread1, thread2;
    int retour1, retour2;

    int nbAffichage1 = 5;
    int nbAffichage2 = 10;

    // creation des threads
    retour1 = pthread_create(&thread1, NULL, affichageIDThreads, (void *)&nbAffichage1);
    if (retour1 != 0)
    {
        printf("Erreur de creation de thread\n");
        exit(1);
    }
    retour2 = pthread_create(&thread2, NULL, affichageIDThreads, (void *)&nbAffichage2);
    if (retour2 != 0)
    {
        printf("Erreur de creation de thread\n");
        exit(1);
    }

    // attente de la fin des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}

// -    -   -   -   -   -   -   -   -   -   -   -   -   -   -|

// commande pour voir les id des threads : top -H
// un fois top lancé, appuyer sur o puis taper COMMAND=[nom du programme]

/**
 * Q1 Pour le programme de l’exercice 2 du TP3, question 2, visualiser à l’aide des
 * commandes système les identifiants des threads créés. Correspondent-ils aux identifiants affichés lors de léxécution du programme ? Justifier.
 *
 * Non, les identifiants affichés lors de l'exécution du programme ne correspondent pas aux identifiants des threads créés. car pthread_self() ne renvoie pas le même identifiant que syscall(SYS_gettid).
 */

/**
 * Rajouter dans ce programme, en précision de la valeur retournée par la fonction
 * pthread self, l’affichage du résultat retourné par l’instruction
 * syscall(SYS_gettid);
 * A comparer avec l’affichage de la commande ps précisant les identifiants des threads
 * d’un processus. Que pouvons-nous conclure ?
 * 
 * l'identifiant de thread renvoyé par pthread_self() est différent de celui renvoyé par syscall(SYS_gettid).
 */