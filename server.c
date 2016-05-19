#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

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

	fd_set active_sockets, read_sockets;
	FD_ZERO(&active_sockets);
	FD_SET(server_socket, &active_sockets);

	while (1)
	{
		read_sockets = active_sockets;
		if (select(FD_SETSIZE, &read_sockets, NULL, NULL, NULL) < 0)
		{
			perror("select() error");
			exit(errno);
		}

		for (int socket = 0; socket < FD_SETSIZE; ++socket)
		{
			if (FD_ISSET(socket, &read_sockets))
			{
				if (socket == server_socket)
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
				
					FD_SET(client_socket, &active_sockets);
				}
				else
				{
					ssize_t bytes_received = recv(socket, recv_buffer, sizeof(recv_buffer) - 1, 0);
					if (bytes_received < 0)
					{
						perror("recv() error");
						exit(errno);
					}
					else if (bytes_received == 0)
					{
						close(socket);
						FD_CLR(socket, &active_sockets);
					}
					else
					{
						recv_buffer[bytes_received] = '\0';
						printf("%s", recv_buffer);
					}
				}
			}
		}

		/*
		snprintf(send_buffer, sizeof(send_buffer), "Server says: %s", recv_buffer);

		ssize_t bytes_sent = send(client_socket, send_buffer, strlen(send_buffer), 0);
		if (bytes_sent < 0)
		{
			perror("send() error");
			exit(errno);
		}

		close(client_socket);
		*/
	}




}









