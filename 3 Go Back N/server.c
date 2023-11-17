// Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define window 5
#define frames 20

struct Frame {
    int seq_num;
    char data[256]; // Modify the size as needed
};

int main() {
    int sockfd, clientfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024] = {0};
    int opt=1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        // perror("Setsockopt failed");
        // exit(EXIT_FAILURE);
    // }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    if (clientfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    struct Frame received_frame;
    int track=-1,i;
    while(track<frames) {
        int j=0,flag=1;
        i=track;
        while(j++<window&&track<frames)
        {
            recv(clientfd, &received_frame, sizeof(received_frame), 0);
            printf("Received frame %d\n",received_frame.seq_num);
            if(flag&&received_frame.seq_num-i==1) i++;
            else flag=0;
            if(received_frame.seq_num==frames-1) break;
        }
        printf("\nAcknowledged frame from %d till %d\n\n",track+1,i);
        track=i;
        sprintf(buffer, "%d", track);
        send(clientfd, buffer, strlen(buffer), 0);
        memset(buffer,sizeof(buffer),0);
        if(track==frames-1) break;
    }

    close(clientfd);
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}

