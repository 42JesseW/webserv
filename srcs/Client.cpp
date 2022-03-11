#include <Client.hpp>

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

void    Client::setCorrectRoute(std::vector<Route> server_routes)
{
    if (m_request.getStatus() == 200) 
    {
        for (std::vector<Route>::iterator it = server_routes.begin(); it != server_routes.end(); it++)
        {
            if (it->getBaseUrl() == m_request.getTarget())
            {
                m_route = (*it);
                return ;
            }
        }
        m_request.setStatus(404);
    }
}
