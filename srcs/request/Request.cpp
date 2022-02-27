#include "Request.hpp"

Request::Request() : m_status(200), m_uri("/"), m_path("/"), m_port(80) {}

Request::Request(const Request &copy)
{
	m_status = copy.m_status;
	m_uri = copy.m_uri;
	m_path = copy.m_path;
	m_query = copy.m_query;
	m_method = copy.m_method;
	m_version = copy.m_version;
	m_request = copy.m_request;
	m_headers = copy.m_headers;
	m_port = copy.m_port;
	m_body = copy.m_body;
}

Request::~Request() {}

Request & Request::operator=(const Request &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

void	Request::setExtension() {}

int	&Request::getStatus()
{
	return (m_status);
}

std::string	&Request::getUri()
{
	return (m_uri);
}

std::string	&Request::getPath()
{
	return (m_path);
}

std::string	&Request::getQuery()
{
	return (m_query);
}

std::map<std::string, std::string>	&Request::getHeaders()
{
	return (m_headers);
}

std::string	&Request::getBody()
{
	return (m_body);
}
