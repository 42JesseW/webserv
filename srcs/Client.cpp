#include <Client.hpp>

Client::Client() : m_request(), m_response()
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
		m_request.setStatus(HTTP_STATUS_NOT_FOUND);
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
		m_request.setStatus(HTTP_STATUS_METHOD_NOT_ALLOWED);
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

void Client::searchFile()
{
	std::string filepath;

	filepath.append(m_route.getFileSearchPath() + m_request.getFilename());
	filepath.erase(0,1);
	if (open(filepath.c_str(), O_RDONLY) != -1)
	{
		m_request.setStatus(HTTP_STATUS_OK);
	}
    else
    {
        m_request.setStatus(HTTP_STATUS_NOT_FOUND);
    }
}

void Client::searchDefaultIndexPages()
{
	std::string filepath;
	std::vector<std::string>::iterator it;

	for(it = m_route.getIndexFiles().begin(); it != m_route.getIndexFiles().end(); it++)
	{
		filepath.append(m_route.getFileSearchPath() + "/" + *it);
		filepath.erase(0,1);
		if (open(filepath.c_str(), O_RDONLY) != -1)
		{
			m_request.setStatus(HTTP_STATUS_OK);
			return ;
		}
		filepath.clear();
	}
	m_request.setStatus(HTTP_STATUS_NOT_FOUND);
}

bool Client::seachCGIExtensions()
{
	std::string extension = m_request.getFilename().substr(m_request.getFilename().find('.'));
	std::vector<std::string>::iterator it;

	for(it = m_route.getCgiFileExtensions().begin(); it != m_route.getCgiFileExtensions().end(); it++)
	{
		if (extension == *it)
		{
			m_request.setStatus(HTTP_STATUS_OK);
			return (true);
		}
	}
	m_request.setStatus(HTTP_STATUS_NOT_FOUND);
	return (false);
}

void Client::checkFileSearchPath()
{

	if (m_request.getStatus() == HTTP_STATUS_OK)
	{
		if (m_request.getFilename() == "/")
		{
			searchDefaultIndexPages();
		}
		else
		{
			if (!seachCGIExtensions())
			{
				searchFile();
			}
		}
	}
}
