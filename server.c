#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int backlog = 10;
int recv_buffer_size = 1024;
int send_buffer_size = 1024;

int main(int argc, char** argv)
{
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	int port = strtol(argv[1], NULL, 10);
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket() error");
		exit(errno);
	}

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



	char recv_buffer[recv_buffer_size];
	char send_buffer[send_buffer_size];
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
		char* client_address_string = inet_ntoa(client_address.sin_addr);
		uint16_t client_port = ntohs(client_address.sin_port);

		printf("%s:%d connected\n", client_address_string, client_port);
	
		ssize_t bytes_received = recv(client_socket, recv_buffer, sizeof(recv_buffer) - 1, 0);
		if (bytes_received < 0)
		{
			perror("recv() error");
			exit(errno);
		}
		recv_buffer[bytes_received] = '\0';

		snprintf(send_buffer, sizeof(send_buffer), "Server says: %s", recv_buffer);

		ssize_t bytes_sent = send(client_socket, send_buffer, strlen(send_buffer), 0);
		if (bytes_sent < 0)
		{
			perror("send() error");
			exit(errno);
		}

		close(client_socket);
	}




}









