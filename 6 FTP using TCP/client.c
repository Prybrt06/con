#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
	int valread, client_socket;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr=inet_addr(SERVER_IP);

	if ((connect(client_socket, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	while(1){
		char message[100];
		printf("Enter the file name : ");
		fgets(message,sizeof(message),stdin);
		message[strcspn(message,"\n")] = 0;
		send(client_socket, message, sizeof(message), 0);
		while ((valread = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        	fwrite(buffer, 1, valread, stdout);
    	}
	}

	// closing the connected socket
	close(client_socket);
	return 0;
}
