#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main()
{
    int server_fd, clientA_fd, clientB_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    // perror("Setsockopt failed");
    // exit(EXIT_FAILURE);
    // }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server Created\n");

    if ((clientA_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("First client connected\n");

    if ((clientB_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Second client connected\n\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        // recv(clientA_fd, buffer, sizeof(buffer), 0);
        read(clientA_fd, buffer, 1024);
        printf("Client A: %s", buffer);

        send(clientB_fd, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));

        // recv(clientB_fd, buffer, sizeof(buffer), 0);
        read(clientB_fd, buffer, 1024);
        printf("Client B: %s\n", buffer);
        send(clientA_fd, buffer, strlen(buffer), 0);
    }

    return 0;
}
