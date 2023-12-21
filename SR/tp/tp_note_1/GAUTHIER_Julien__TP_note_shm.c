// Julien GAUTHIER | SR | 27_09_2023

// compilation : gcc -Wall TP_note_shm.c -o TP_note_shm -lpthread

/**
 * @brief Un processus doit obtenir la somme d'un ensemble de n nombres, où n est donné par un argument en ligne de commande.
 * Pour simplifier, nous considérons que les nombres sont les entiers consécutif dans l'intervalle [1,n].
 * Cette simplification permet à l'utilisateur de l'application de vérifier le résultat du calcul : la somme des n premiers entiers est n(n+1)/2.
 * Ce processus délègue partiellement le traitement à son processus fils.
 * Le processus fils génère aléatoirement une valeur entière m entre 1 et n-1 (bornes incluses) et calcule la somme des entiers consecutifs dans l'intervalle [m+1,n].
 * Le processus parent somme les nombres restants de l'intervalle [1,m] et calcule la somme finale en additionnant les deux sommes partielles.
 *
 * Contrainte : le calcul des sommes dans les différents processus doit être fait en concurence, afin de minimiser au maximum les attentes.
 */

/**
 * Q0 : Quelles informations doivent-être partager entre les processus ?
 * les informations à partager sont :
 * - la valeur de n
 * - la valeur de m
 * - la valeur de la somme partielle du processus fils
 * - la valeur de la somme partielle du processus parent
 * - la valeur de la somme finale
 */

/**
 * Q1 : Donner le code du programme réalisant la somme telle que l'énoncé l'indique.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE 5 // taille de la mémoire partagée

/// @brief Union utilisée pour les appels semctl dans les sémaphores System V.
union semUnion
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/// @brief Initialise un sémaphore avec une valeur initiale de 0
/// Cette fonction utilise semctl pour définir la valeur initiale du sémaphore.
/// @param semid L'identifiant du sémaphore à initialiser
void initialize_semaphore(int semid)
{
    union semUnion arg;
    arg.val = 0; // valeur initiale du sémaphore
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("Erreur lors de l'initialisation du sémaphore");
        exit(1);
    }
}

/// @brief Incrémente (signale) le sémaphore
/// Cette fonction utilise semop pour augmenter la valeur du sémaphore de 1.
/// Cela peut être utilisé pour signaler qu'une ressource est disponible ou qu'une condition est satisfaite.
/// @param semid L'identifiant du sémaphore signalé
void signal_semaphore(int semid)
{
    struct sembuf semOperation;
    semOperation.sem_num = 0;
    semOperation.sem_op = 1;
    semOperation.sem_flg = 0;
    if (semop(semid, &semOperation, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

/// @brief décrémente (attend) le sémaphore
/// Cette fonction utilise semop pour diminuer la valeur du sémaphore de 1.
/// Si la valeur du sémaphore est déjà à 0, le processus appelant est bloqué jusqu'à ce que la valeur du sémaphore soit augmentée par un autre processus.
/// Cela peut être utilisé pour attendre qu'une ressource soit disponible ou qu'une condition soit satisfaite.
/// @param semid L'identifiant du sémaphore à attendre
void wait_semaphore(int semid)
{
    struct sembuf semOperation;
    semOperation.sem_num = 0;
    semOperation.sem_op = -1;
    semOperation.sem_flg = 0;
    if (semop(semid, &semOperation, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

/// @brief Retourne un nombre aléatoire entre limInf et limSup
/// @param limInf
/// @param limSup
/// @return le nombre aléatoire
int randomNumber(int limInf, int limSup)
{
    srand(time(NULL));
    return (int)((rand() / (RAND_MAX + 1.0)) * (limSup - limInf + 1) + limInf);
}

int main(int argc, char *argv[])
{

    if (argc != 2) // controle des paramètres
    {
        printf("Usage : %s <n>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]); // nombre d'entiers à sommer
    if (n <= 0)
    {
        printf("Veuillez entrer une valeur positive pour n. \n");
        exit(1);
    }

    // création de la mémoire partagée
    key_t key = ftok("./", 'h');
    int shm_id = shmget(key, SHM_SIZE * sizeof(int), 0666 | IPC_CREAT);
    int *data = (int *)shmat(shm_id, NULL, 0);

    // création du sémaphore
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    initialize_semaphore(semid);

    data[0] = n; // valeur de n

    // création du processus fils
    pid_t pid = fork();

    if (pid == 0) // processus fils
    {
        int m = randomNumber(1, n - 1); // valeur de m
        data[1] = m;
        printf("m = %d\n", m);

        signal_semaphore(semid); // déblocage du processus parent

        int sum = 0;
        for (int i = data[1] + 1; i <= data[0]; i++)
        {
            sum += i;
        }
        data[2] = sum; // valeur de la somme partielle du processus fils
        printf("somme partiel du processus fils : %d\n", data[2]);

        shmdt(data); // détachement de la mémoire partagée
        exit(0);
    }
    else
    { // processus parent
        wait_semaphore(semid);

        int sum = 0;
        for (int i = 1; i <= data[1]; i++)
        {
            sum += i;
        }
        data[3] = sum; // valeur de la somme partielle du processus parent
        printf("somme partiel du processus parent : %d\n", data[3]);

        wait(NULL); // attente de la fin du processus fils

        data[4] = data[2] + data[3]; // valeur de la somme finale

        printf("La somme des %d premiers entiers est %d\n", data[0], data[4]);

        shmdt(data);                    // détachement de la mémoire partagée
        shmctl(shm_id, IPC_RMID, NULL); // destruction de la mémoire partagée
        semctl(semid, 0, IPC_RMID);     // destruction du sémaphore
    }
    return 0;
}

/**
 * Q2 : Quel est la commande système qui permet d'obtenir exclusivement le moment où un segment de mémoire, donné par son identifiant, à été détaché pour lea dernière fois ?
 * ipcs -m -t
 *              -m : affiche les informations sur les segments de mémoire partagée
 *              -t : affiche le temps de détachement du segment
 *
 * si on connait l'identifiant du segment, on peut faire
 * ipcs -m -t | grep <id>
 */