#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main()
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char *ip = "127.0.0.1";
	char buffer[1024] = {0};
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	// while (1)
	// {
		char message[100];
		fgets(message, sizeof(message), stdin);
		send(client_fd, message, strlen(message), 0);
		valread = read(client_fd, buffer, 1024);
		printf("%s\n", buffer);
	// }

	// closing the connected socket
	close(client_fd);
	return 0;
}
