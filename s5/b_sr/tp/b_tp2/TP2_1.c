// SR | TP n°2 Part 1   |
// Julien Gauthier      |
// L3 TP1A              |

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

    // écriture dans le segment de mémoire partagée
    sprintf(shm_addr, "Hello SON !");
    printf("Message ecrit dans le segment de memoire partagee\n");

    // initialise le signal SIGUSR1
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);
    
    // retardement du signal
    sigprocmask(SIG_BLOCK, &sig, NULL);

    // création du processus fils
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0)
    {
        // processus fils

        // attache le segment de mémoire partagée
        char *attachement = (char *)shmat(shm_id, NULL, 0);
        if (attachement == (void *)-1)
        {
            perror("shmat");
            exit(1);
        }
        

        struct sigaction action;
        action.sa_handler = SIG_IGN;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGUSR1, &action, NULL);

        // sigset_t mask;
        // sigemptyset(&mask);

        // // attente du signal SIGUSR1
        // sigsuspend(&mask);


        // attend tout sauf SIGUSR1
        sigset_t inverseMask;
        sigfillset(&inverseMask);
        sigdelset(&inverseMask, SIGUSR1);

        // attente du signal SIGUSR1
        sigsuspend(&inverseMask);

        // lecture dans le segment de mémoire partagée
        printf("Message lu dans le segment de memoire partagee : %s\n", attachement);

        // détachement du segment de mémoire partagée
        if (shmdt(attachement) == -1)
        {
            perror("shmdt");
            exit(1);
        }

        // fin du processus fils
        exit(0);
    }
    /* processus pere */

    // envoie le signal SIGUSR1 au processus fils
    kill(pid, SIGUSR1);

    
    // attente de la fin du processus fils
    wait(NULL);

    // détachement du segment de mémoire partagée
    shmdt(shm_addr);

    // destruction du segment de mémoire partagée
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
