#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Error in socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in connection");
        exit(1);
    }

    while (1)
    {
        printf("Enter a command (or 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the command to the server
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit\n") == 0)
        {
            printf("Client exiting...\n");
            break;
        }

        // Receive and display the result from the server
        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received <= 0)
            {
                break; // Exit the loop if no more data is received
            }

            if (strcmp(buffer, "CMD_EXEC_COMPLETE") == 0)
            {
                break; // Exit the loop when the command execution is complete
            }

            printf("%s", buffer);
        }
    }

    close(client_socket);
    return 0;
}