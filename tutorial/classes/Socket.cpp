#include "Socket.hpp"

Socket::Socket() : m_sock_fd(SOCK_FD_EMPTY)
{

}

Socket::~Socket()
{
    if (m_sock_fd != SOCK_FD_EMPTY)
    {
        close(m_sock_fd);
    }
}

/*
** initialise socket for listening on specified port
*/

void        Socket::init(const short sin_port)
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
}

const int       Socket::getFileDescriptor()
{
    return (m_sock_fd);
}
