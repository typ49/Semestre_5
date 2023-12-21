// SR   |   TP6 | 2023-2024 | L3 Computer Science
// Julien Gauhtier TP1A

// Compilation: gcc -Wall -Werror -Wextra -o TP6 TP6.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#define MAX_VALUE 100
#define MIN_VALUE 0

// Structure to store the sum of even and odd numbers
struct SumData {
    unsigned long evenSum;
    unsigned long oddSum;
};

// Function to generate random numbers and update the sums
void sumRandom(int n, struct SumData *data) {
    for (int i = 0; i < n; i++) {
        int val = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
        if (val % 2 == 0) {
            data->evenSum += val;
        } else {
            data->oddSum += val;
        }
    }
}

// Semaphore P operation (wait or down)
void P(int semid) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

// Semaphore V operation (signal or up)
void V(int semid) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    srand(time(NULL));  // Initialize random number generator

    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(struct SumData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment
    struct SumData *shared_data = (struct SumData *)shmat(shmid, NULL, 0);
    if (shared_data == (struct SumData *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize the sums to zero
    shared_data->evenSum = 0;
    shared_data->oddSum = 0;

    // Create semaphore
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    semctl(semid, 0, SETVAL, 1);  // Initialize semaphore value to 1

    // Create child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        srand(time(NULL) + 1);  // Ensure different random sequence than parent
        P(semid);  // Wait on semaphore
        sumRandom(n, shared_data);  // Generate random numbers and update sums
        V(semid);  // Signal semaphore
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        P(semid);  // Wait on semaphore
        sumRandom(n, shared_data);  // Generate random numbers and update sums
        V(semid);  // Signal semaphore

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

        // Remove semaphore
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}
