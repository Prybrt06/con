#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 128

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    int opt=1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding");
        close(server_socket);
        exit(1);
    }

    char buffer[MAX_BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        memset(buffer,0,sizeof(buffer));
        int x=recvfrom(server_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        int n=atoi(buffer);
        printf("\nCLient sent %d\n",n);
        if(n<0) break;
        int ans=1;
        for(int i=2;i<=n;i++) ans*=i;
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"%d",ans);
        sendto(server_socket, buffer, n, 0, (struct sockaddr*)&client_addr, client_len);
    }

    printf("\n\nClosing Socket\n");
    close(server_socket);
    
    return 0;
}
