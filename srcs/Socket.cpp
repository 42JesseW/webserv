#include <Socket.hpp>
#include <Server.hpp>

Socket::Socket() :
    m_sock_fd(SOCK_FD_EMPTY),
    m_address(DFL_SERVER_HOST),
    m_port(DFL_SERVER_PORT)
{

}

/* destroy current socket and copy all the data */
Socket::Socket(const Socket &sock)
{
    *this = sock;
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
#ifdef __APPLE__
        m_sock_addr.sin_len = sock.m_sock_addr.sin_len;
#endif
        m_address = sock.m_address;
        m_port = sock.m_port;
    }
    return (*this);
}

/* overwrite default address */
void    Socket::setAddress(const std::string &address)
{
    m_address = address;
}

/* overwrite default port */
void    Socket::setPort(unsigned short port)
{
    m_port = port;
}

/*
** Initialise socket for listening on specified port.
** Use PF_INET instead of AF_INET when calling socket():
** https://beej.us/guide/bgnet/html/#system-calls-or-bust
*/

int        Socket::init()
{
    int yes;

    m_sock_fd = socket(PF_INET, SOCK_STREAM, SOCK_TCP_T);
    if (m_sock_fd == SOCK_ERROR)
    {
        /* some error handling */
        throw std::runtime_error(std::string(__func__) + ": Failed to create socket.");
    }

    std::cout << "Socket::init[" << m_sock_fd << "] -> " << m_address << ":" << m_port << std::endl;
    std::memset(&m_sock_addr, 0, sizeof(m_sock_addr));
    m_sock_addr.sin_family      = AF_INET;          /* IPv4 */
    m_sock_addr.sin_port        = htons(m_port);
    m_sock_addr.sin_addr.s_addr = INADDR_ANY;       /* listen for anything 0.0.0.0 */
    if (m_address != DFL_SERVER_HOST)
        m_sock_addr.sin_addr.s_addr = inet_addr(m_address.c_str());

    /* bind address to currently nameless socket */
    if (m_sock_addr.sin_addr.s_addr == INADDR_NONE ||
        bind(m_sock_fd, (SA *)&m_sock_addr, (socklen_t)sizeof(m_sock_addr)) == SOCK_ERROR)
    {
        /* some error handling */
        throw std::runtime_error(std::string(__func__) + ": Failed to bind to address.");
    }

    /* loose the 'address currently in use' error because the kernel hasn't properly cleaned everything up */
    yes = 1;
    if (setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == SOCK_ERROR)
    {
        /* some error handling */
        throw std::runtime_error(std::string(__func__) + ": Failed to set sock options.");
    }

    /* set the socket to be non blocking so recv() and send() functions don't block */
    if (fcntl(m_sock_fd, F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        throw std::runtime_error(std::string(__func__) + ": Failed to set NONBLOCKING.");
    }

    /* set the socket for listening with a max connection backlog of DFL_BACKLOG */
    if (listen(m_sock_fd, DFL_BACKLOG) == SOCK_ERROR)
    {
        /* some error handling */
        throw std::runtime_error(std::string(__func__) + ": Failed to listen on socket.");
    }
    return (SOCK_SUCCESS);
}

int&        Socket::getFd()
{
    return (m_sock_fd);
}
