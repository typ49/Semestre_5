// Authors: GAUTHIER Julien

// compilation : gcc -Wall TP_note_shm_v2.c -o TP_note_shm_v2 -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define SHM_SIZE 5 // Taille de la mémoire partagée

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Veuillez entrer une valeur positive pour n.\n");
        exit(1);
    }

    // Création de la mémoire partagée
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SHM_SIZE * sizeof(int), 0666|IPC_CREAT);
    int *data = (int *) shmat(shmid, NULL, 0);

    pid_t pid = fork();

    if (pid == 0) { // Processus fils
        srand(time(NULL));
        data[1] = 1 + rand() % (n - 1); // Générer m aléatoirement

        int sum = 0;
        for (int i = data[1] + 1; i <= n; i++) {
            sum += i;
        }
        data[2] = sum; // Stocker la somme partielle du fils

        shmdt(data); // Détacher la mémoire partagée
        exit(0);
    } else { // Processus parent
        while (data[1] == 0) { // Attendre que m soit défini
            usleep(1000); // Pause de 1 milliseconde
        }

        int sum = 0;
        for (int i = 1; i <= data[1]; i++) {
            sum += i;
        }
        data[3] = sum; // Stocker la somme partielle du parent

        wait(NULL); // Attendre que le fils termine

        printf("Somme finale: %d\n", data[2] + data[3]);

        shmdt(data); // Détacher la mémoire partagée
        shmctl(shmid, IPC_RMID, NULL); // Supprimer la mémoire partagée
    }

    return 0;
}
