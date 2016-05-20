#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/event.h>

int backlog = 10;

int main(int argc, char * * argv)
{
	struct addrinfo * addresses;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	int error = getaddrinfo(NULL, argv[1], &hints, &addresses);
	if (error)
	{
		fprintf(stderr, "getaddrinfo error: %s", gai_strerror(error));
		exit(EXIT_FAILURE);
	}

	struct addrinfo server_address = addresses[0];

	int server_socket = socket(server_address.ai_family, server_address.ai_socktype, server_address.ai_protocol);
	if (server_socket < 0)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	if (bind(server_socket, server_address.ai_addr, server_address.ai_addrlen) < 0)
	{
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, backlog) < 0)
	{
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	int queue = kqueue();
	struct kevent event;

	EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(queue, &event, 1, NULL, 0, NULL) == -1)
	{
		fprintf(stderr, "kevent")
	}

	struct kevent events[32];

	while (1)
	{
		int fd = kevent(queue, NULL, 0, events, 32, NULL);
		if (fd == -1)
		{
			perror("kevent");
			exit(EXIT_FAILURE);
		}
		
	}
}







