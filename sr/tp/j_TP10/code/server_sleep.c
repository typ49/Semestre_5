#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// compilation: gcc -Wall -Wextra -o server2 server.c

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

    // Temporisation pour observer l'état CLOSED
    sleep(2);

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

    // Temporisation pour observer l'état CLOSED (avec adresse attribuée)
    sleep(2);

    // Passage en mode écoute
    if (listen(listenfd, 10) < 0)
    {
        perror("Erreur de listen");
        return 1;
    }

    // Temporisation pour observer l'état LISTEN
    sleep(5);

    printf("Serveur en écoute sur le port %s\n", argv[1]);

    // Acceptation d'une connexion cliente
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
    if (connfd < 0)
    {
        perror("Erreur de accept");
        return 1;
    }

    // Temporisation pour observer l'état ESTABLISHED
    sleep(3);

    // Envoi d'un message au client
    if (send(connfd, sendline, strlen(sendline) + 1, 0) < 0)
    {
        perror("Erreur lors de l'envoi");
    }

    // Temporisation pour observer l'état ESTABLISHED (pendant la transmission)
    sleep(3);

    // Fermeture de la connexion avec le client
    close(connfd);

    // Temporisation pour observer les états de fermeture
    sleep(3);

    // Fermeture du socket d'écoute
    close(listenfd);

    // Temporisation pour observer après fermeture complète
    sleep(2);

    printf("Serveur arrêté après avoir servi un client.\n");

    return 0;
}
