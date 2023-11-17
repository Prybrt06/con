#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    char *filename = "file.txt";
    // printf("Enter the filename : ");
    // scanf("%s",&filename);

    printf("Hello");

    // return 0;
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *file;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        error("Error creating socket");
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Send the file name to the server
    if (sendto(sockfd, filename, strlen(filename), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        error("Error sending file name");
    }

    printf("Hello");

    // Open the file for reading
    file = fopen(filename, "rb");
    if (!file)
    {
        error("Error opening file for reading");
    }

    // Read and send file content to the server
    while (1)
    {
        size_t read_len = fread(buffer, 1, BUFFER_SIZE, file);
        if (read_len == 0)
        {
            break; // End of file
        }

        // Send the read data to the server
        if (sendto(sockfd, buffer, read_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            error("Error sending file content");
        }
    }

    printf("File transfer completed.\n");

    // Close the file
    fclose(file);

    // Close the socket
    close(sockfd);

    return 0;
}
