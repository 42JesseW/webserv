#include "web.h"

char    *bin2hex(const unsigned char *input, size_t len) {
    int     result_length;
    char    *result;
    char    *hexits = "0123456789ABCDEF";

    if (!input || len <= 0)
        return (NULL);
    result_length = (3 * len) + 1;
    result = calloc(result_length, sizeof(char));
    if (!result)
        return (NULL);
    for (int i = 0; i < len; i++) {
        result[i * 3]       = hexits[input[i] >> 4];
        result[(i * 3) + 1] = hexits[input[i] & 0x0F];
        result[(i * 3) + 2] = ' ';
    }
    return (result);
}

void    err_n_die(const char *fmt, ...) {
    int     errno_save;
    va_list ap;

    errno_save = errno;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (errno_save != 0) {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }

    va_end(ap);
    exit(EXIT_FAILURE);
}

int     setup_server(short port, int backlog) {
    int         listenfd;
    SA_IN       servaddr;


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
    if ( (listen(listenfd, backlog)) == SYS_ERROR) {
        err_n_die("Error listen to socket");
    }

    return (listenfd);
}

int accept_new_connection(int server_socket) {
    int     addr_size = sizeof(SA_IN);
    int     client_socket;
    SA_IN   client_addr;

    client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size);
    if (client_socket == SYS_ERROR) {
        err_n_die("Error accepting connection");
    }
    return (client_socket);
}
