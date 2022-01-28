
#include "web.h"
#include <sys/select.h>

void    handle_connection(int client_socket) {
    int         listenfd, n;
    SA_IN       servaddr;
    uint8_t     buff[MAXLINE + 1];
    uint8_t     recvline[MAXLINE + 1];

    SA_IN       addr;
    socklen_t   addr_len;

    memset(recvline, 0, MAXLINE);
    while ( (n = read(client_socket, recvline, MAXLINE - 1) ) > 0) {
        fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

        if (recvline[n - 1] == '\n') {
            break ;
        }
        memset(recvline, 0, MAXLINE);
    }
    if (n == SYS_ERROR) {
        err_n_die("Error when reading from socket");
    }

    /* create a response message with message OK status code 200 and body Hello */
    snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

    write(client_socket, (char *)buff, strlen((char *)buff));
    close(client_socket);
}

int     main(void)
{
    fd_set  current_sockets, ready_sockets;
    int     server_socket;
    int     client_socket;

    FD_ZERO(&current_sockets);

    server_socket = setup_server(SERVER_PORT, DFL_BACKLOG);
    FD_SET(server_socket, &current_sockets);
    for ( ; ; ) {
        /* reassign read_sockets because select() is destructive (i.e. removes fd's from the fd_set) */
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) == SYS_ERROR) {
            err_n_die("Select fails");
        }

        for (int sock_idx = 0; sock_idx < FD_SETSIZE; sock_idx++) {
            /* if a file descriptor is set then there are two cases */
            if (FD_ISSET(sock_idx, &ready_sockets)) {
                /* sock_idx is the server_socket */
                if (sock_idx == server_socket) {
                    /* there is a new connection */
                    client_socket = accept_new_connection(server_socket);

                    /* add the new client_socket to the list of sockets we're watching */
                    FD_SET(client_socket, &current_sockets);
                } else {
                    /* do whatever with the connection */
                    handle_connection(sock_idx);
                    FD_CLR(sock_idx, &current_sockets);
                }
            }
        }
    }

    return (EXIT_SUCCESS);
}
