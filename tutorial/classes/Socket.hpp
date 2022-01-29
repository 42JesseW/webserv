#ifndef SOCKET_HPP

# define SOCKET_HPP
# define SOCK_TCP_T     0
# define SOCK_FD_EMPTY  -1
# define DFL_BACKLOG    10
# define SOCK_ERROR     -1

# define SA     struct sockaddr
# define SA_IN  struct sockaddr_in

# include <string>          // TODO maybe move to more common hpp
# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <netdb.h>
# include <sys/socket.h>

class Socket
{
private:
    int                 m_sock_fd;
    struct sockaddr_in  m_sock_addr;

public:
    Socket();
    Socket(Socket &sock);

    Socket& operator = (const Socket& sock);

    ~Socket();

    void        init(const short sin_port);
    const int   getFileDescriptor();

};

#endif
