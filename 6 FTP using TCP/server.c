#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
  
    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_socket, SOL_SOCKET,SO_REUSEADDR, &opt,sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_socket, (struct sockaddr*)&address,sizeof(address))< 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((client_socket= accept(server_socket, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("\nListening for requests\n");

    while(1){
        char buf[SIZE],buf2[SIZE];
        recv(client_socket,buf,sizeof(buf),0);
        strcpy(buf2,buf);
        FILE *fptr;
        if ((fptr = fopen(buf,"r")) == NULL){
            printf("Error! opening file");
            exit(1);
        }
        memset(buf,0,1024);
        while(fgets(buf,sizeof(buf),fptr)!=NULL) {
            if(send(client_socket, buf, sizeof(buf), 0) < 0) {
                perror("Sending failed");
                exit(EXIT_FAILURE);
            }
            memset(buf,0,1024);
        }
        if (fptr != NULL)
			fclose(fptr);
        printf("\nSent %s\n",buf2);
    }
  
    // closing the connected socket
    close(client_socket);
    // closing the listening socket
    shutdown(server_socket, SHUT_RDWR);
    return 0;
}