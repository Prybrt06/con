#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 128

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    char buffer[MAX_BUFFER_SIZE];
    printf("\nEnter a negative number to close the program\n");

    while(1) {
        printf("\nEnter the number: ");
        int n;
        scanf(" %d",&n);
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"%d",n);
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(n<0) break;
        memset(buffer,0,sizeof(buffer));
        int x = recvfrom(client_socket, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
        int ans=atoi(buffer);
        printf("\nFactorial of %d is : %d\n",n,ans);
    }

    printf("\nClosing Socket\n");
    close(client_socket);
    
    return 0;
}