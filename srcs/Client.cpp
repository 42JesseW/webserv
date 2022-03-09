#include "../includes/Client.hpp"


Client::Client() : m_request()
{

}

Client::Client(const Client &client)
{
    *this = client;
}

Client::~Client()
{

}

Client&     Client::operator = (const Client &client)
{
    if (this != &client)
    {
    }
    return (*this);
}

void    Client::setSocket(int socket)
{
    m_socket = socket;
}

Request & Client::getRequest()
{
    return (m_request);
}

int & Client::getSocket()
{
    return (m_socket);
}
