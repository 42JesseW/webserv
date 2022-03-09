#include <Request.hpp>

Request::Request() : m_target("/"), m_status(200), m_port(80) {}

Request::Request(const Request &copy)
{
	m_status = copy.m_status;
	m_target = copy.m_target;
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
	{
		m_status = copy.m_status;
		m_target = copy.m_target;
		m_query = copy.m_query;
		m_method = copy.m_method;
		m_version = copy.m_version;
		m_request = copy.m_request;
		m_headers = copy.m_headers;
		m_port = copy.m_port;
		m_body = copy.m_body;
	}
	return (*this);
}

void Request::setRequest(std::string new_request)
{
	this->resetRequest();
	this->m_request = new_request;
}

void Request::setStatus(int status)
{
	m_status = status;
}

int	&Request::getStatus()
{
	return (m_status);
}

std::string	&Request::getTarget()
{
	return (m_target);
}

std::string	&Request::getQuery()
{
	return (m_query);
}

std::string	&Request::getMethod()
{
	return (m_method);
}

std::string	&Request::getVersion()
{
	return (m_version);
}
int	&Request::getPort()
{
	return (m_port);
}

std::map<std::string, std::string>	&Request::getHeaders()
{
	return (m_headers);
}

std::string	&Request::getBody()
{
	return (m_body);
}

void	Request::resetRequest() 
{
	m_status = 200;
	m_target = "/";
	m_query.clear();
	m_method.clear();
	m_version.clear();
	m_request.clear();
	m_headers.clear();
	m_port = 80;
	m_body.clear();
}
