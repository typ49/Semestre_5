
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // Pour gethostbyname

// compilation: gcc -Wall -Wextra -o client2 client.c

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char recvline[1024];
    struct hostent *host;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
        return 1;
    }

    // Résolution du nom d'hôte
    if ((host = gethostbyname(argv[1])) == NULL) {
        perror("Erreur gethostbyname");
        return 1;
    }

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erreur de création du socket");
        return 1;
    }

    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    memcpy(&servaddr.sin_addr, host->h_addr_list[0], host->h_length);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Erreur de connexion");
        return 1;
    }
    
    // Temporisation après la connexion
    sleep(5);

    // Réception du message du serveur
    ssize_t n = recv(sockfd, recvline, sizeof(recvline)-1, 0);
    if (n < 0) {
        perror("Erreur lors de la réception");
        return 1;
    }
    recvline[n] = '\0'; // Ajout du caractère de fin de chaîne

    printf("Message reçu du serveur: %s\n", recvline);
    
    // Temporisation avant la fermeture
    sleep(5);

    close(sockfd);
    return 0;
}