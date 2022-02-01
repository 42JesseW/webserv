#include "Socket.hpp"

Socket::Socket() : m_sock_fd(SOCK_FD_EMPTY)
{

}

/* destroy current socket and copy all the data */
Socket::Socket(const Socket &sock)
{
    close(m_sock_fd);
    m_sock_fd = sock.m_sock_fd;
    std::memset(m_sock_addr.sin_zero, 0, 8);
    m_sock_addr.sin_addr = sock.m_sock_addr.sin_addr;
    m_sock_addr.sin_family = sock.m_sock_addr.sin_family;
    m_sock_addr.sin_port = sock.m_sock_addr.sin_port;
    m_sock_addr.sin_len = sock.m_sock_addr.sin_len;
}

Socket::~Socket()
{
    if (m_sock_fd != SOCK_FD_EMPTY)
    {
        close(m_sock_fd);
    }
}

Socket&     Socket::operator = (const Socket &sock)
{
    if (this != &sock)
    {
        *this = sock;
    }
    return (*this);
}

/*
** Initialise socket for listening on specified port.
** Use PF_INET instead of AF_INET when calling socket():
** https://beej.us/guide/bgnet/html/#system-calls-or-bust
*/

int        Socket::init(const std::string& address, const short sin_port)
{
    int yes = 1;

    m_sock_fd = socket(PF_INET, SOCK_STREAM, SOCK_TCP_T);
    if (m_sock_fd == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Socket::init -> " << address << ":" << sin_port << std::endl;
    std::memset(&m_sock_addr, 0, sizeof(m_sock_addr));
    m_sock_addr.sin_family      = AF_INET;
    m_sock_addr.sin_port        = htons(sin_port);
    m_sock_addr.sin_addr.s_addr = INADDR_ANY;       /* listen for anything 0.0.0.0 */
    if (address != "*")
        m_sock_addr.sin_addr.s_addr = inet_addr(address.c_str());

    if (m_sock_addr.sin_addr.s_addr == INADDR_NONE)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }
    
    if (setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    if (bind(m_sock_fd, (SA *)&m_sock_addr, (socklen_t)sizeof(m_sock_addr)) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    if (fcntl(m_sock_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    if (listen(m_sock_fd, DFL_BACKLOG) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}

int       Socket::getFileDescriptor() const
{
    return (m_sock_fd);
}

int      Socket::addSocketToQueue(void)
{
    // Initialize kqueue instance
    if ((kq = kqueue()) < 0)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    // Add listening socket to kqueue
    EV_SET(&evSet, m_sock_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    return (SOCK_SUCCESS);
}

int     Socket::receiveNewConnection(void)
{
    int fd;


}


int     Socket::pollingLoop(void)
{
    while (1)
    {
        if ((num_events = kevent(kq, NULL, 0, evList, KQ_MAX_EVENTS, NULL)) < 0)
        {
            /* do some error handling */
            std::exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; i++)
        {
            if (evList[i].ident = m_sock_fd)
                receiveNewConnection();
        }
    }
}
