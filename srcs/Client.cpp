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

Request & Client::getRequest()
{
    return (m_request);
}
