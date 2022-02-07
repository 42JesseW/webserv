#ifndef SOCKET_HPP

# define SOCKET_HPP
# define SOCK_TCP_T     0
# define SOCK_FD_EMPTY  -1
# define DFL_BACKLOG    128
# define SOCK_ERROR     -1
# define SOCK_SUCCESS   1

# define SA     struct sockaddr
# define SA_IN  struct sockaddr_in

# include <string>          // TODO maybe move to more common hpp
# include <iostream>

# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <netdb.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <stdio.h>         // to be removed //


class Socket
{
private:
    int                 m_sock_fd;
    struct sockaddr_in  m_sock_addr;

public:
    Socket();
    Socket(int sock_fd, struct sockaddr_in sock_addr);
    Socket(const Socket &sock);
    ~Socket();

    Socket& operator = (const Socket& sock);

    int         init(const std::string& address, const short sin_port);
    int         getFileDescriptor();

};

#endif
