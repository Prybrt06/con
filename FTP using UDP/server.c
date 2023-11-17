#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        error("Error creating socket");
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        error("Error binding socket");
    }

    printf("UDP File Server is listening on port %d...\n", PORT);

    while (1)
    {
        // Receive file name from the client
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len == -1)
        {
            error("Error receiving file name");
        }

        buffer[recv_len] = "\0"; // Null-terminate the received data (file name)

        // Open the file for writing
        FILE *file = fopen(buffer, "wb");
        if (!file)
        {
            error("Error opening file for writing");
        }

        // Receive file content from the client and write to the file
        while (1)
        {
            recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
            if (recv_len <= 0)
            {
                break; // End of file transfer
            }

            // Write the received data to the file
            fwrite(buffer, 1, recv_len, file);
        }

        printf("File transfer completed.\n");

        // Close the file
        fclose(file);
    }

    // Close the socket (this will never be reached in this example)
    close(sockfd);

    return 0;
}
