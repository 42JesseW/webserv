#include "web.h"

int     main(int argc, char *argv[])
{
    int     sockfd, n;
    int     sendbytes;
    SA_IN   servaddr;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];
    
    if (argc != 2) {
        err_n_die("usage: %s <server address>", argv[0]);
    }

    /*
    ** Create a new socket structure:
    **  - Read data from or write data into
    **  - AF_INET
    **    Address Family - Internet (IPv4)
    **  - SOCK_STREAM
    **    Create a connection and send or receive stream of data
    **    Another option would be SOCK_DGRAM for chuncks of data.
    */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, SOCK_TCP_T)) == SYS_ERROR) {
        err_n_die("Error creating the socket");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(DFL_HTTP_PORT);

    // TODO replace with inet_addr function
    // if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    //     err_n_die("inet_pton error for %s ", argv[1]);
    // }

    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    if (servaddr.sin_addr.s_addr == INADDR_NONE) {
        err_n_die("inet_addr error for %s ", argv[1]);
    }

    /*
    ** Connect to the specified address
    */
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == SYS_ERROR) {
        err_n_die("connect failed");
    }

    /*
    ** Write data to the socket (i.e. prepare the message)
    ** TODO look into message format (RFC HTTP)
    */
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);
    if (write(sockfd, sendline, sendbytes) != sendbytes) {
        err_n_die("Error when writing to socket");
    }

    memset(recvline, 0, MAXLINE);
    while ( (n = read(sockfd, recvline, MAXLINE - 1)) > 0) {
        printf("%s", recvline);
    }
    if (n == SYS_ERROR) {
        err_n_die("Error when reading from socket");
    }
    return (EXIT_SUCCESS);
}
