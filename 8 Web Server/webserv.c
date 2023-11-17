#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const char* html_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<html><body>"
    "<h1>Hello, World!</h1>"
    "<p>This is a simple HTML page served by a C socket server.</p>"
    "</body></html>";

int main() {
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    int client_socket;
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);
        send(client_socket, html_response, strlen(html_response), 0);
        close(client_socket);
    }

    close(server_socket);

    return 0;
}
