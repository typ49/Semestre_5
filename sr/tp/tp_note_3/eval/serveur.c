#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // Pour gethostbyname

/* limites pour l'opérande de gauche */
#define MIN 0
#define MAX 100

/* constantes pour les opérateurs */
#define ADD 1      // 1
#define SUB ADD+1  // 2
#define MULT SUB+1 // 3
#define DIV MULT+1 // 4

/**
 * Structure d'une opération 
 */
 
 struct operation {
    int oprt;   /* opérateur binaire : + -> 1, - -> 2, * -> 3, / -> 4  */
    int oprd;   /* deuxième opérande, de droite */
 };
 
 
/**
 * Tire et renvoie un entier pseudo-aléatoire entre min et max bornes incluses.
 *
 * @param min valeur minimale de l'entier
 * @param max valeur maximale de l'entier
 */
int random_int(int min, int max){
    return (int)((rand()/(RAND_MAX+1.0))*(max-min+1)+min);
}

/**
 * Evalue l'expression qui applique sur l'opérande de gauche une opération binaire
 *
 * @param p_opd_g adresse de l'opérande de gauche
 * @param p_op adresse d'une opération binaire
 */
bool eval(int *p_opd_g, const struct operation *p_op) {
    switch (p_op->oprt) {
    	case ADD : *p_opd_g = *p_opd_g + p_op->oprd; break;
    	case SUB : *p_opd_g = *p_opd_g - p_op->oprd; break;
    	case MULT : *p_opd_g = *p_opd_g * p_op->oprd; break;
    	case DIV : if (p_op->oprd == 0) return false; 
    	           *p_opd_g = *p_opd_g / p_op->oprd; break;
    }	
    return true;	
}

/**
 * Affiche une opération (opérateur et opérande de droite).
 *
 * @param p_op adresse d'une opération
 */
void aff_op(const struct operation *p_op){
    switch (p_op->oprt) {
    	case ADD : printf("+"); break;
    	case SUB : printf("-"); break;
    	case MULT : printf("*"); break;
    	case DIV : printf("/"); break;
    }
    printf(" ");
    printf("%d", p_op->oprd);
    printf(" ");   
}


int main(int argc, char *argv[])
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char sendline[1024] = "Message du serveur";
    strcat(sendline, "\0");

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        return 1;
    }

    // Création du socket d'écoute
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("Erreur de création du socket");
        return 1;
    }

    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    // Association du socket avec l'adresse (bind)
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Erreur de bind");
        return 1;
    }

    // Passage en mode écoute
    if (listen(listenfd, 10) < 0)
    {
        perror("Erreur de listen");
        return 1;
    }

    printf("Serveur en écoute sur le port %s\n", argv[1]);

    // Acceptation d'une connexion cliente
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
    if (connfd < 0)
    {
        perror("Erreur de accept");
        return 1;
    }

    // recoit un nombre d'opération du client
    int nb_op;
    if (read(connfd, &nb_op, sizeof(nb_op)) < 0) {
        perror("Erreur lors de la réception du nombre d'opérations");
        return 1;
    }

    // recoit les opérations une par une et les mémorise dans l'ordre, les place dans une liste
    struct operation op_list[nb_op];
    for (int i = 0 ; i<nb_op ; i++) {
        if (read(connfd, &op_list[i], sizeof(op_list[i])) < 0) {
            perror("Erreur lors de la réception d'une opération");
            return 1;
        }
    }

    // évalue les opérations dans l'ordre et affiche le résultat
    int opd_g = random_int(MIN, MAX);
    // int opd_g = 10; // pour le test ((10 + 3) * 2) - 5 = 21
    printf("Opérande de gauche: %d\n", opd_g);
    for (int i = 0 ; i<nb_op ; i++) {
        printf("Opération %d: %d ", i+1, opd_g);
        aff_op(&op_list[i]);
        if (eval(&opd_g, &op_list[i])) {
            printf("= %d\n", opd_g);
        } else {
            printf("= ERREUR\n");
            return 1;
        }
    }

    // envoie le résultat de l'évaluation
    if (write(connfd, &opd_g, sizeof(opd_g)) < 0) {
        perror("Erreur lors de l'envoi du résultat");
        return 1;
    }

    // fermeture de la connexion
    close(connfd);
    close(listenfd);

    return 0;

}