#include "../includes/Client.hpp"

Client::Client()
{

}

Client::Client(const Client &client)
{

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
