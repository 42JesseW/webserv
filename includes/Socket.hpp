#ifndef SOCKET_HPP

# define SOCKET_HPP

# define SOCK_TCP_T         0
# define SOCK_FD_EMPTY      (-1)
# define SOCK_ERROR         (-1)
# define SOCK_SUCCESS       1

# define DFL_BACKLOG        128
# define DFL_SERVER_HOST    "*" /* translates to INADDR_ANY 0.0.0.0 */

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


class Socket
{
private:
    int             m_sock_fd;
    SA_IN           m_sock_addr;

    std::string     m_address;
    unsigned short  m_port;

public:
    Socket();
    Socket(const Socket &sock);
    ~Socket();

    Socket& operator = (const Socket& sock);

    void        setAddress(const std::string& address);
    void        setPort(unsigned short port);

    int         init(void);
    int&        getFd(void);

};

#endif
