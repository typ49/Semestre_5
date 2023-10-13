// SR   |   TP6_V3 | 2023-2024 | L3 Computer Science
// Julien Gauhtier TP1A

// Comilation: gcc -Wall -Wextra -o tp6_v3 tp6_v3.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>

#define MAX_VALUE 1000
#define MIN_VALUE 0

// Structure to store the sum of even and odd numbers
struct SharedData {
    unsigned long evenSum;
    unsigned long oddSum;
    sem_t semaphore;
};

// Function to generate random numbers and update the sums
void sumRandom(int n, struct SharedData *data) {
    for (int i = 0; i < n; i++) {
        int val = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
        if (val % 2 == 0) {
            data->evenSum += val;
        } else {
            data->oddSum += val;
        }
    }
}

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment
    struct SharedData *shared_data = (struct SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (struct SharedData *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize the sums to zero
    shared_data->evenSum = 0;
    shared_data->oddSum = 0;

    // Initialize semaphore
    sem_init(&shared_data->semaphore, 1, 1);

    // Create child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Initialize random number generator using XOR with PID for uniqueness
    srand(time(NULL) ^ getpid());

    if (pid == 0) {  // Child process
        sem_wait(&shared_data->semaphore);
        sumRandom(n, shared_data);
        sem_post(&shared_data->semaphore);
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        sem_wait(&shared_data->semaphore);
        sumRandom(n, shared_data);
        sem_post(&shared_data->semaphore);

        wait(NULL);  // Wait for child process to finish

        // Print the results
        printf("Sum of even numbers: %lu\n", shared_data->evenSum);
        printf("Sum of odd numbers: %lu\n", shared_data->oddSum);

        // Detach and remove shared memory segment
        if (shmdt(shared_data) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }

        // Destroy semaphore
        sem_destroy(&shared_data->semaphore);
    }

    return 0;
}
