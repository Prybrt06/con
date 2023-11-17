// Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define window 5
#define frames 20

struct Frame {
    int seq_num;
    char data[256]; // Modify the size as needed
};

int main() {
    srand(time(NULL));
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    struct Frame frame;
    frame.seq_num = 0;
    int i=0;
    int flag=1,error=rand()%frames; 
    //error flag
    for (; i < frames;i++) {
        memset(buffer,sizeof(buffer),0);
        int j=0;
        while(j++<window&&i<frames)
        {
            // error
            int random_number = rand();
            if(flag&&i==error) {
                i=random_number%frames;
                flag=0;
            }
            frame.seq_num = i++;
            send(sockfd, &frame, sizeof(frame), 0);
        }
        printf("Sent frame till %d\n", frame.seq_num);
        read(sockfd, buffer, 1024);
        i=atoi(buffer);
        printf("Acknowledged frame till %d\n", i);
    }

    close(sockfd);
    return 0;
}

