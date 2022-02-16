#ifndef MAIN_HPP
# define MAIN_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <iostream>
# include <fstream>
#include <sstream>
# include <map>
# include <vector>

// # define SERVER_PORT 1800
# define MAXLINE 4096
# define SA struct sockaddr
# define forever for(;;)

void err_n_die(const char *fmt, ...);

#endif
