#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9034"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	fd_set master; // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	int fdmax; // maximum file descriptor number

	int listener; // listening socket descriptor
	int newfd; // newly accepted socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	char buf[256];
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes = 1;
	int i, j, rv;

	struct addrinfo hints, *ai, *p;

	// Clear the master and temp sets
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	// Get a socket and bind it
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
	{
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit (1);
	}

	for (p =ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (listener < 0)
			continue ;

		// Lose the "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close (listener);
			continue ;
		}

		break ;
	}

	// If we get here, it means we are not bound
	if (p == NULL)
	{
		fprintf(stderr, "selectserver: failed to bind\n");
		exit (2);
	}

	freeaddrinfo(ai);

	// Listen
	if (listen(listener, 10) == -1)
	{
		perror("listen");
		exit (3);
	}

	// Add the listener to the master set
	FD_SET(listener, &master);

	// Keep track of the biggest file descriptor
	fdmax = listener;

	// main loop
	for (;;)
	{
		read_fds = master;
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit (4);
		}

		// Run through the existing connections looking for data to read
		for ( i = 0; i <= fdmax; i++)
		{
			// We got one
			if (FD_ISSET(i, &read_fds))
			{
				// Handle new connections
				if (i == listener)
				{
					addrlen = sizeof(remoteaddr);
					newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

					if (newfd == -1)
						perror("accept");
					else
					{
						// Add to master set
						FD_SET(newfd, &master);
						if (newfd > fdmax)
							fdmax = newfd;
						printf("selectserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);						
					}
				}
				// Handle data from a client
				else
				{
					// Connection closed or error
					if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
					{
						// Connection closed by client
						if (nbytes == 0)
							printf("selectserver: socket %d hung up\n", i);
						// Error
						else
							perror("recv");

						close(i);		
						FD_CLR(i, &master); // Remove from master set
					}
					// Got some data from client
					else
					{
						for (j = 0; j <= fdmax; j++)
						// Send to everyone
						{
							if (FD_ISSET(j, &master))
							{
								// Except the listener and ourselves
								if ( j != listener && j != i)
								{
									if (send(j, buf, nbytes, 0) == -1)
										perror("send");
								}
							}

						}
					}
				}

			}
		}
	}
	return (0);
}