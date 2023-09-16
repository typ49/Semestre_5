// SR | TP n°2 Part 1_bis   |
// Julien Gauthier          |
// L3 TP1A                  |

// compilation : gcc -Wall -Werror -o TP2_1 TP2_1.c

/*
 * Ecrire un programme ou un processus parent communique un message au processus fils
 * qui l’affiche apres l’avoir accede depuis la memoire partagee.
 * La technique utilisee dans le TP1 pour s’assurer de la disponibilite du message envoye par le processus parent,
 * chez le processus fils, est valable dans ce cas ?
 * Indication. Utiliser des signaux et la fonction sigsuspend afin d’assurer de maniere
 * absolue l’anteriorite de l’ecriture par rapport a la lecture.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>

int main()
{

    // creation d'un segment de memoire partagee
    int shm_id = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0666);

    // attachement du segment de memoire partagee
    char *shm_addr = shmat(shm_id, NULL, 0);


    // création du processus fils
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    // écriture dans le segment de mémoire partagée
    sprintf(shm_addr, "Hello SON !");
    printf("Message ecrit dans le segment de memoire partagee\n");

    // initialise le signal SIGUSR1
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);


    // envoie le signal SIGUSR1 au processus fils
    kill(pid, SIGUSR1);

    // initialise le signal SIGUSR1
    sigaction(SIGUSR1, &sig, NULL);

    // attente du signal SIGUSR1
    sigsuspend(&sig);

    // lecture du segment de mémoire partagée
    printf("Message lu dans le segment de memoire partagee : %s\n", shm_addr);

    // détachement du segment de mémoire partagée
    shmdt(shm_addr);

    // destruction du segment de mémoire partagée
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}