#include "web.h"

int     main(void)
{
    int         listenfd, connfd, n;
    SA_IN       servaddr;
    uint8_t     buff[MAXLINE + 1];
    uint8_t     recvline[MAXLINE + 1];

    SA_IN       addr;
    socklen_t   addr_len;

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, SOCK_TCP_T)) == SYS_ERROR) {
        err_n_die("Failed to create socket");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);    /* listen for anything */
    servaddr.sin_port           = htons(SERVER_PORT);   /* list on specified port */

    if ( (bind(listenfd, (SA *)&servaddr, sizeof(servaddr))) == SYS_ERROR) {
        err_n_die("Error binding to socket");
    }
    
    /*
    ** The backlog parameter defines the maximum length for the queue of pending connections.
    ** If a connection request arrives with the queue full, the client may receive an error
    ** with an indication of ECONNREFUSED.  Alternatively, if the underlying protocol supports
    ** retransmission, the request may be ignored so that retries may succeed.
    */
    if ( (listen(listenfd, DFL_BACKLOG)) == SYS_ERROR) {
        err_n_die("Error listen to socket");
    }

    /* EVENT_LOOP */
    for ( ; ; ) {
        fprintf(stdout, "Waiting for a connection on port %d\n", SERVER_PORT);
        fflush(stdout);

        /*
        ** provide the listenfd when accepting connections and receive an NEW SOCKET fd when a connection is found
        **
        ** The argument address is a result parameter that is filled in with the address of the connecting
        ** entity, as known to the communications layer.  The exact format of the address parameter is
        ** determined by the domain in which the communication is occurring.  The address_len is a value-result
        ** parameter; it should initially contain the amount of space pointed to by address; on return it will
        ** contain the actual length (in bytes) of the address returned.  This call is used with connection-based
        ** socket types, currently with SOCK_STREAM.
        **
        ** Accept is going to WAIT untill a connection is found and the listen SOCKET will still be listening
        */
        connfd = accept(listenfd, (SA *)NULL, NULL);

        memset(recvline, 0, MAXLINE);
        while ( (n = read(connfd, recvline, MAXLINE - 1) ) > 0) {
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

        write(connfd, (char *)buff, strlen((char *)buff));
        close(connfd);
    }

    return (EXIT_SUCCESS);
}
