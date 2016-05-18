#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

static int backlog = 5;

int main(int argc, char** argv)
{
	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor < 0)
	{
		perror("socket() error");
		exit(errno);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	int port = strtol(argv[1], NULL, 10);
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_descriptor, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		perror("bind() error");
		exit(errno);
	}

	if (listen(socket_descriptor, backlog) < 0)
	{
		perror("listen() error");
		exit(errno);
	}

	





}









