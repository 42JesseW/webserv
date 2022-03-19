#include <Client.hpp>

Client::Client() : m_request(), m_response(m_request, m_route)
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

void    Client::setCorrectRoute(std::vector<Route> &server_routes)
{
	if (m_request.getStatus()== HTTP_STATUS_OK) 
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

void Client::checkRoute()
{
	checkAcceptedMethods();
	checkRedirects();
	checkFileSearchPath();
}

void Client::checkAcceptedMethods()
{
	if (m_request.getStatus() == HTTP_STATUS_OK)
	{
		for (std::vector<std::string>::iterator it = m_route.getAcceptedMethods().begin(); it != m_route.getAcceptedMethods().end(); it++)
		{
			if (*it == m_request.getMethod())
			{
				return;
			}
		}
		m_request.setStatus(405);
	}
}

void Client::checkRedirects()
{
	if (m_request.getStatus() == HTTP_STATUS_OK) 
	{
		if (m_route.getRedir() != NULL)
		{
			m_request.setStatus(m_route.getRedir()->status_code);
		}
	}
}

void Client::checkFileSearchPath()
{
	std::string filepath;

	if (m_request.getStatus() == HTTP_STATUS_OK)
	{
		filepath.append(m_route.getFileSearchPath() + m_request.getFilename());
		filepath.erase(0,1);
		if (open(filepath.c_str(), O_RDONLY) == -1)
		{
			m_request.setStatus(404);
		}
	}
}
