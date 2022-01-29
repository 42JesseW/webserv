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
** initialise socket for listening on specified port
*/

int        Socket::init(const short sin_port)
{
    m_sock_fd = socket(AF_INET, SOCK_STREAM, SOCK_TCP_T);
    if (m_sock_fd == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    std::memset(&m_sock_addr, 0, sizeof(m_sock_addr));
    m_sock_addr.sin_family      = AF_INET;
    m_sock_addr.sin_port        = htons(sin_port);
    m_sock_addr.sin_addr.s_addr = INADDR_ANY;       /* listen for anything 0.0.0.0 */

    if (bind(m_sock_fd, (SA *)&m_sock_addr, (socklen_t)sizeof(m_sock_addr)) == SOCK_ERROR)
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

const int       Socket::getFileDescriptor()
{
    return (m_sock_fd);
}
