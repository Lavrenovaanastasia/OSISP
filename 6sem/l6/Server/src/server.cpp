#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "functions.h"

#define BUF_SIZE 2000

void *handle_client(void *arg) {
    int socket = *((int *)arg);
    char buf[BUF_SIZE];
    int len;

    while((len = recv(socket, buf, BUF_SIZE - 1, 0)) > 0) 
    {
        buf[len] = '\0'; // безопасно, т.к. BUF_SIZE-1
        std::string answ = encode(std::string(buf));
        send(socket, answ.c_str(), answ.length(), 0);
    }

    close(socket);
    free(arg); // освободить память new_sock
    return NULL;
}

int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(struct sockaddr_in);
    pthread_t client_thread;

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0) {
        perror("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    listen(server_sock, 5);
    printf("Server started on port %s. Waiting for incoming connections...\n", argv[1]);

    while((client_sock = accept(server_sock, (struct sockaddr *)&client, &c))) {
        printf("Connection accepted\n");

        int *new_sock = (int *)malloc(sizeof(int));
        *new_sock = client_sock;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
        }
    }

    if(client_sock < 0) {
        perror("Accept failed");
    }

    close(server_sock);
    return 0;
}