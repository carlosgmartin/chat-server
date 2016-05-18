#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

static int backlog = 5;

int main(int argc, char** argv)
{
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket() error");
		exit(errno);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	int port = strtol(argv[1], NULL, 10);
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
	{
		perror("bind() error");
		exit(errno);
	}

	if (listen(server_socket, backlog) < 0)
	{
		perror("listen() error");
		exit(errno);
	}

	while (1)
	{
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);

		int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_length);
		if (client_socket < 0)
		{
			perror("accept() error");
			exit(errno);
		}
	}





}









