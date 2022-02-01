#ifndef SOCKET_HPP

# define SOCKET_HPP
# define SOCK_TCP_T     0
# define SOCK_FD_EMPTY  -1
# define DFL_BACKLOG    10
# define SOCK_ERROR     -1
# define SOCK_SUCCESS   1

# define SA     struct sockaddr
# define SA_IN  struct sockaddr_in

# define KQ_MAX_EVENTS 32

# include <string>          // TODO maybe move to more common hpp
# include <iostream>

# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <fcntl.h>


class Socket
{
private:
    int                 m_sock_fd;
    struct sockaddr_in  m_sock_addr;

    // kqueue-related attributes
    int                 kq;
    int                 num_events;
    struct kevent       evSet;
    struct kevent       evList[KQ_MAX_EVENTS];

    // new_fd from accept() to be used for send() and recv()
    int                 accept_fd;

public:
    Socket();
    Socket(const Socket &sock);
    ~Socket();

    Socket& operator = (const Socket& sock);

    int         init(const std::string& address, const short sin_port);
    int         getFileDescriptor() const;
    int         addSocketToQueue(void);
    int         pollingLoop(void);
    int         receiveNewConnection(void);
    int         clientDisconnected(void);
    int         readMsg(void);

};

#endif
