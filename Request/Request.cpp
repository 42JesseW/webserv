#include "Request.hpp"

Request::Request() : m_status(200), m_port(80), m_keep_alive(false) {}

Request::Request(const Request &copy)
{
	// Implement in the end
}

Request::~Request() {}

Request & Request::operator=(const Request &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

std::string Request::getRequest()
{
	return (m_request);
}

std::string  Request::getMethod()
{
	return (m_method);
}

std::map<std::string, std::string> Request::getHeaders()
{
	return (m_headers);
}

std::string Request::getBody()
{
	return (m_body);
}

int Request::getStatus()
{
	return (m_status);
}
