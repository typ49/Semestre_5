/* SR | TP n°1 Part 1   |
 * Julien Gauthier      |
 * L3 TP1A              |
 */

// compilation : gcc -Wall -Werror -o TP1_1 TP1_1.c

/*
 * programme ou un processus fils communique un message au processus parent,
 * il s'affiche apres reception.
 * Le processus parent est celui qui cree le segment de memoire partagee.
 * Des que la communication est finie,
 * c’est aussi le processus parent qui marque le segment pour la suppression.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

int main()
{
    // creation de la cle :
    key_t key = ftok("./", 'h');

    // creation du segment de memoire partagee :
    int shmID = shmget(key, sizeof(char) * 1024, IPC_CREAT | 0644);

    // obtention de l'adresse du segment :
    char *p = (char *)shmat(shmID, NULL, 0);
    // vérifier l'attachement :
    if (p == (void *)-1)
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
        // ecriture dans le segment :
        sprintf(p, "Bonjour papa !");

        // detache le segment :
        if (shmdt(p) == -1)
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

    // lecture dans le segment :
    printf("%s\n", p);

    // detache le segment :
    if (shmdt(p) == -1)
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

/*
 * Question n°1 :  Comment s’assurer, dans le processus parent, que le message ait ete ecrit avant de
 * l’acceder ?
 *
 * On peut utiliser un wait(NULL) pour attendre la fin du processus fils.
 */

/*
 * Question n°2 : Verifier, par des commandes systeme, que le segment de memoire partagee a ete
 * bien supprime. Indiquer la demarche (avec les commandes impliquees).
 *
 * On peut utiliser la commande ipcs -m pour verifier que le segment de memoire partagee a ete bien
 * supprime.
 */

/*
 * Question n°3 :  Quelles instructions pourraient etre commentees dans le programme afin d’empecher
 * la suppression du segment de memoire partagee ? Expliquer et le faire. Grace a des
 * commandes systeme, identifier le segment et supprimer-le. Indiquer les commandes
 * utilisees.
 *
 * On peut commenter les lignes 50 et 51 pour empecher la suppression du segment de memoire partagee.
 * Pour identifier le segment, on peut utiliser la commande ipcs -m. Pour le supprimer, on peut utiliser
 * la commande ipcrm -m <id>.
 */

/*
 * Question n°4 : D’un point de vue d’un ingenieur systeme, quelles sont les conditions qui doivent
 * etre remplies (outre les droits) pour s’assurer qu’un segment de memoire partagee
 * puisse etre supprime par des commandes systeme ? Donner les commandes systeme
 * qui permettent de s’en assurer
 *
 * Pour s’assurer qu’un segment de memoire partagee puisse etre supprime par des commandes systeme,
 * il faut que le segment soit detache de tous les processus qui y sont attaches. Pour s’en assurer, on
 * peut utiliser la commande ipcs -m.
 */