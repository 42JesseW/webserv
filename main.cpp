/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/29 12:29:41 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/01 17:14:22 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"

int     main(void)
{
    // Create a socket
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket < 0)
    {
        err_n_die("Socket fail");
    }

    // Bind the socket to an ip address and port
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY; // Could also use inet_pton ...

    bind(listening_socket, (SA *)&server, sizeof(server));

    // Tell Windsock the socket is for listening
    listen(listening_socket, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int client_socket = accept(listening_socket, (SA *)&client, &clientSize);
    if (client_socket< 0)
    {
        err_n_die("Invalid client socket");
    }

    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXHOST);

    if (getnameinfo((SA *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port);
    }

    // Close listening socket
    close(listening_socket);

    // While loop: accept and echo mesage back to client
    char buff[4096];

    forever {
        memset(buff, 0, 4096);

        std::cout << "Waiting for client to connect" << std::endl;
        // Wait for client to send data
        int bytes_received = recv(client_socket, buff, 4096, 0);
        if (bytes_received < 0)
            err_n_die("Read error");
        else if (bytes_received == 0)
            break ;

        // Echo message back to client
        send(client_socket, buff, (bytes_received + 1), 0);
    }

    // Close the socket
    close(client_socket);
}

void err_n_die(const char *fmt, ...)
{
    int         errno_save;
    va_list     ap;

    errno_save = errno;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fflush(stdout);

    if (errno_save != 0)
    {
        fprintf(stdout, "(errno = %d): %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);
    exit(1);
}
