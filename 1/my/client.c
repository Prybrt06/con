#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{

    char *ip = "127.0.0.1";
    int port = 5566;
    int sock;

    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    // create client socket

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[1] client socket  error ");
        exit(1);
    }

    printf("[1] TCP client socket created \n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    char message[100];
    fgets(message, sizeof(message), stdin);
    send(sock, message, strlen(message), 0);
    printf("connected to server\n");

    return 0;
}