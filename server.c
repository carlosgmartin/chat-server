#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>

int backlog = 10;
int buffer_size = 1024;

int main(int argc, char * * argv)
{
	// Create server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	in_port_t server_port = strtoul(argv[1], NULL, 0);
	server_address.sin_port = htons(server_port);

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

	fd_set sockets, read_sockets;
	FD_ZERO(&sockets);
	FD_SET(server_socket, &sockets);

	while (1)
	{
		read_sockets = sockets;
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
					socklen_t client_address_size = sizeof(client_address);
					int client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_size);
					if (client_socket < 0)
					{
						perror("accept() error");
						exit(errno);
					}

					// Client connected
					char* client_address_string = inet_ntoa(client_address.sin_addr);
					in_port_t client_port = ntohs(client_address.sin_port);
					printf("%s:%d connected\n", client_address_string, client_port);
					
					FD_SET(client_socket, &sockets);
				}
				else
				{
					char buffer[buffer_size];
					ssize_t bytes_received = recv(socket, buffer, sizeof(buffer), 0);
					if (bytes_received < 0)
					{
						perror("recv() error");
						exit(errno);
					}
					else if (bytes_received == 0)
					{
						struct sockaddr_in client_address;
						socklen_t client_address_size = sizeof(client_address);
						getpeername(socket, (struct sockaddr *) &client_address, &client_address_size);
						
						// Client disconnected
						char* client_address_string = inet_ntoa(client_address.sin_addr);
						in_port_t client_port = ntohs(client_address.sin_port);
						printf("%s:%d disconnected\n", client_address_string, client_port);
						
						close(socket);
						FD_CLR(socket, &sockets);
					}
					else
					{
						write(socket, buffer, bytes_received);

						// Print message from client
						fwrite(buffer, sizeof(char), bytes_received, stdout);
					}
				}
			}
		}
	}
}











