#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // Pour gethostbyname

/* limites pour les opérandes */
#define MIN 0
#define MAX 100

/* constantes pour les opérateurs */
#define ADD 1        // 1
#define SUB ADD + 1  // 2
#define MULT SUB + 1 // 3
#define DIV MULT + 1 // 4

/**
 * Structure d'une opération
 */
struct operation
{
    int oprt; /* opérateur binaire : + -> 1, - -> 2, * -> 3, / -> 4  */
    int oprd; /* deuxième opérande, de droite */
};

/**
 * Tire et renvoie un entier pseudo-aléatoire entre min et max bornes incluses.
 *
 * @param min valeur minimale de l'entier
 * @param max valeur maximale de l'entier
 */
int gen_int_rand(int min, int max)
{
    return (int)((rand() / (RAND_MAX + 1.0)) * (max - min + 1) + min);
}

/**
 * Génère aléatoirement une opération (opérateur et opérande de droite).
 *
 * @param p_op adresse d'une opération
 */
void gen_op(struct operation *p_op)
{
    p_op->oprt = gen_int_rand(ADD, DIV);
    p_op->oprd = gen_int_rand(MIN, MAX);
}

/**
 * Affiche une opération (opérateur et opérande de droite).
 *
 * @param p_op adresse d'une opération
 */
void aff_op(const struct operation *p_op)
{
    switch (p_op->oprt)
    {
    case ADD:
        printf("+");
        break;
    case SUB:
        printf("-");
        break;
    case MULT:
        printf("*");
        break;
    case DIV:
        printf("/");
        break;
    }
    printf(" ");
    printf("%d", p_op->oprd);
    printf(" ");
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *host;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
        return 1;
    }

    // Résolution du nom d'hôte
    if ((host = gethostbyname(argv[1])) == NULL)
    {
        perror("Erreur gethostbyname");
        return 1;
    }

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Erreur de création du socket");
        return 1;
    }

    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    memcpy(&servaddr.sin_addr, host->h_addr_list[0], host->h_length);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Erreur de connexion");
        return 1;
    }

    // envoie d'un nombre d'opérations à évaluer
    int nb_op = gen_int_rand(MIN, MAX);
    // int nb_op = 3; // test pour ((10 + 3) * 2) - 5 = 21
    printf("Nombre d'opérations à évaluer: %d\n", nb_op);
    if (write(sockfd, &nb_op, sizeof(nb_op)) < 0)
    {
        perror("Erreur lors de l'envoi du nombre d'opérations");
        return 1;
    }

    // génération de nb_op opérations et envoie une par une au server
    struct operation op;
    for (int i = 0; i < nb_op; i++)
    {
        gen_op(&op);
        aff_op(&op);
        printf("\n");
        if (write(sockfd, &op, sizeof(op)) < 0)
        {
            perror("Erreur lors de l'envoi d'une opération");
            return 1;
        }
    }

    // test ((10 + 3) * 2) - 5 = 21
    /*
    op.oprt = ADD;
    op.oprd = 3;
    if (write(sockfd, &op, sizeof(op)) < 0) {
        perror("Erreur lors de l'envoi d'une opération");
        return 1;
    }
    op.oprt = MULT;
    op.oprd = 2;
    if (write(sockfd, &op, sizeof(op)) < 0) {
        perror("Erreur lors de l'envoi d'une opération");
        return 1;
    }
    op.oprt = SUB;
    op.oprd = 5;
    if (write(sockfd, &op, sizeof(op)) < 0) {
        perror("Erreur lors de l'envoi d'une opération");
        return 1;
    }
    */

    // recoit le résultat de l'évaluation
    int result;
    if (read(sockfd, &result, sizeof(result)) < 0)
    {
        perror("Erreur lors de la réception du résultat");
        return 1;
    }

    // affiche le résultat
    printf("--------------------------------------");
    printf("\n");
    printf("Résultat: %d\n", result);

    // fermeture du socket
    close(sockfd);

    return 0;
}