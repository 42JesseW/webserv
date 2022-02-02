#include "../includes/Socket.hpp"

Socket::Socket() : m_sock_fd(SOCK_FD_EMPTY)
{

}

/* destroy current socket and copy all the data */
Socket::Socket(const Socket &sock)
{
    *this = sock;
}

/* create new socket object from existing data */
Socket::Socket(int sock_fd, struct sockaddr_in sock_addr) :
    m_sock_fd(sock_fd),
    m_sock_addr(sock_addr)
{

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
    if (this != &sock && m_sock_fd != sock.m_sock_fd)
    {
        close(m_sock_fd);
        m_sock_fd = sock.m_sock_fd;
        std::memcpy(m_sock_addr.sin_zero, sock.m_sock_addr.sin_zero, 8);
        m_sock_addr.sin_addr = sock.m_sock_addr.sin_addr;
        m_sock_addr.sin_family = sock.m_sock_addr.sin_family;
        m_sock_addr.sin_port = sock.m_sock_addr.sin_port;
        m_sock_addr.sin_len = sock.m_sock_addr.sin_len;
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
    int yes;

    m_sock_fd = socket(PF_INET, SOCK_STREAM, SOCK_TCP_T);
    if (m_sock_fd == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Socket::init -> " << address << ":" << sin_port << std::endl;
    std::memset(&m_sock_addr, 0, sizeof(m_sock_addr));
    m_sock_addr.sin_family      = AF_INET;          /* IPv4 */
    m_sock_addr.sin_port        = htons(sin_port);
    m_sock_addr.sin_addr.s_addr = INADDR_ANY;       /* listen for anything 0.0.0.0 */
    if (address != "*")
        m_sock_addr.sin_addr.s_addr = inet_addr(address.c_str());

    /* bind address to currently nameless socket */
    if (m_sock_addr.sin_addr.s_addr == INADDR_NONE ||
        bind(m_sock_fd, (SA *)&m_sock_addr, (socklen_t)sizeof(m_sock_addr)) == SOCK_ERROR)
    {
        std::cout << "Failed to bind to port " << '\n';
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* loose the 'address currently in use' error because the kernel hasn't properly cleaned everything up */
    yes = 1;
    if (setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* set the socket to be non blocking so recv() and send() functions don't block */
    if (fcntl(m_sock_fd, F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* set the socket for listening with a max connection backlog of DFL_BACKLOG */
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
