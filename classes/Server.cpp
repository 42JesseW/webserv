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

Server::~Server() 
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

/*
** defaults are passed from Config (DFL_SERVER_*)
** host species a full host <host>:<port>
*/

int                         Server::initListener(const std::string& host)
{
    std::string address;
    short       sin_port;

    if (ft::count(host.begin(), host.end(), ':') > 1)
        throw std::invalid_argument("Host " + host + " is invalid");

    /* convert the <port> from <host>:<port> to an integer */
    sin_port = std::atoi(host.substr(host.find(":") + 1).c_str());

    address = host.substr(0, host.find(":"));
    if (sin_port == 0 || m_sock.init(address, sin_port) == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}

int                         Server::initPolling(void)
{
    if (m_sock.addSocketToQueue() < 0 )
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    return (SOCK_SUCCESS);
}

int                         Server::eventLoop(void)
{
    if (m_sock.pollingLoop() < 0)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    return (SOCK_SUCCESS);
}