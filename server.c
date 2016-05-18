#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

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
}