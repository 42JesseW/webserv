#include "Server.hpp"

Server::Server()
{

}

Server::Server(const Server &server) :
    m_sock(server.m_sock),
    m_names(server.m_names),
    m_routes(server.m_routes)
{

}

Server&     Server::operator = (const Server &server)
{
    if (this != &server)
    {
        *this = server;
    }
    return (*this);
}

const Socket&               Server::getSocket()
{
    return (m_sock);
}

std::vector<std::string>&   Server::getNames()
{
    return (m_names);
}

std::vector<Route>&         Server::getRoutes()
{
    return (m_routes);
}

int                         Server::initListener(const short port)
{
    if (m_sock.init(port) == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}
