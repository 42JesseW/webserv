#ifndef WEB_H

# define WEB_H

# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>

#define DFL_HTTP_PORT   80
#define SERVER_PORT     18000
#define SYS_ERROR       -1
#define SOCK_TCP_T      0
#define DFL_BACKLOG     10

#define MAXLINE 4096
#define SA      struct sockaddr
#define SA_IN   struct sockaddr_in

void    err_n_die(const char *fmt, ...);
char    *bin2hex(const unsigned char *input, size_t len);
int     setup_server(short port, int backlog);
int     accept_new_connection(int server_socket);

#endif
