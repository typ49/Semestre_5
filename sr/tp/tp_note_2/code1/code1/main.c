#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "grid.h"

pthread_mutex_t grid_mutex; // Mutex for thread safety
int boat_count = 0;         // Number of boats placed

struct ThreadArgs
{
    int nb_attempts;
    size_t boat_length;
    struct grid *g;
};

void print_usage(const char *cmd)
{
    fprintf(stderr, "Usage : %s NB_ATTEMPTS BOAT_LENGTH...\n", cmd);
}

int random_int(int min, int max)
{
    return (int)((rand() / (RAND_MAX + 1.0)) * (max - min + 1) + min);
}

void *place_boat(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int nb_attempts = args->nb_attempts;
    size_t boat_length = args->boat_length;
    struct grid *g = args->g;

    int actual_attempts = 0; // To count the valid attempts

    while (actual_attempts < nb_attempts)
    {
        bool vertical = (bool)random_int(0, 1);
        size_t max_x = vertical ? grid_get_size(g) - 1 : grid_get_size(g) - boat_length;
        size_t max_y = vertical ? grid_get_size(g) - boat_length : grid_get_size(g) - 1;

        size_t x = random_int(0, max_x);
        size_t y = random_int(0, max_y);

        // Check if the boat placement would be out of bounds
        if ((vertical && y + boat_length > grid_get_size(g)) ||
            (!vertical && x + boat_length > grid_get_size(g)))
        {
            continue; // Skip this iteration without counting it as an attempt
        }

        actual_attempts++; // Increment the valid attempt counter

        pthread_mutex_lock(&grid_mutex); // Lock the mutex

        if (grid_place_boat_if_no_collision(g, x, y, boat_length, vertical))
        {
            const char *orientation = vertical ? "vertically" : "horizontally";
            printf("Boat of length %3zu placed at (%zu,%zu) %s in %d attempts.\n", boat_length, x, y, orientation, actual_attempts);
            boat_count++;
            pthread_mutex_unlock(&grid_mutex); // Unlock the mutex
            free(args);
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(&grid_mutex); // Unlock the mutex
    }
    printf("Boat of length %3zu could not be placed in %d attempts.\n", boat_length, actual_attempts);
    free(args);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));                     // Initialize random seed
    pthread_mutex_init(&grid_mutex, NULL); // Initialize the mutex

    if (argc < 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    const int grid_size = 10;

    size_t *boat_lengths = calloc(argc - 2, sizeof(size_t));
    for (int i = 2; i < argc; ++i)
    {
        int l = atoi(argv[i]);
        if (l < 1 || l > grid_size)
        {
            free(boat_lengths);
            fprintf(stderr, "All boat lengths must be greater than or equal to 1 and less than or equal to %d.\n", grid_size);
            print_usage(argv[0]);
            return 3;
        }
        boat_lengths[i - 2] = l;
    }

    struct grid g;
    grid_create(&g, grid_size);

    size_t nb_attempts = atoi(argv[1]);

    pthread_t *threads = malloc((argc - 2) * sizeof(pthread_t));
    for (int i = 0; i < argc - 2; ++i)
    {
        struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
        args->nb_attempts = nb_attempts;
        args->boat_length = boat_lengths[i];
        args->g = &g;
        int result = pthread_create(&threads[i], NULL, place_boat, args);
        if (result != 0)
        {
            fprintf(stderr, "Error creating thread.\n");
            return 4;
        }
    }

    for (int i = 0; i < argc - 2; ++i)
    {
        int result = pthread_join(threads[i], NULL);
        if (result != 0)
        {
            fprintf(stderr, "Error joining thread.\n");
            return 5;
        }
        pthread_detach(threads[i]);
    }

    free(threads);
    free(boat_lengths);

    grid_print(&g);
    printf("Total number of boats placed: %d\n", boat_count);
    grid_destroy(&g);

    pthread_mutex_destroy(&grid_mutex); // Destroy the mutex

    return 0;
}
