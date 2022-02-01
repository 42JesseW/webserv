/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/29 12:29:41 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/01 13:17:40 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"

// int main(void) {
//     int                     listenfd, connfd, n;
//     struct sockaddr_in      servaddr;
//     uint8_t                 buff[MAXLINE + 1];
//     uint8_t                 recvline[MAXLINE + 1];

//     if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//         err_n_die("Socket error");

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     servaddr.sin_port = htons(SERVER_PORT);

//     if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
//         err_n_die("Bind error");

//     if ((listen(listenfd, 10)) < 0)
//         err_n_die("Listen error");

//     forever {
//         struct sockaddr_in  addr;
//         socklen_t           addr_len;
// 		char				client_address[MAXLINE + 1];

//         printf("Waiting for a connection on port %d\n", SERVER_PORT);
//         fflush(stdout);
//         connfd = accept(listenfd, (SA *) &addr, &addr_len);

// 		// Getting the IP address from client
// 		inet_ntop(AF_INET, &addr, client_address, MAXLINE);
// 		printf("Client connecting: %s\n", client_address);

//         memset(recvline, 0, MAXLINE);

//         while ((n = recv(connfd, recvline, MAXLINE - 1, 0)) > 0)
//         {
//             fprintf(stdout, "\n%s\n\n", recvline);
//             if (recvline[n - 1] == '\n')
//                 break ;
//             memset(recvline, 0, MAXLINE);
//         }

//         if (n < 0)
//             err_n_die("Read error");

//         snprintf((char *)buff, sizeof(buff), "HTTP 1.1 200 OK\r\n\r\nHoi Lor");
//         send(connfd, (char *)buff, strlen((char *)buff), 0);
//         close(connfd);
//     }
//     exit (0);
// }

int     main(void)
{

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
