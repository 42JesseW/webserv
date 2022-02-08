#include "../includes/Response.hpp"

Response::Response(const Request &r) : m_request(r) {}

Response::Response(const Response &copy)
{
	m_request = copy.m_request;
	m_start_line = copy.m_start_line;
	m_headers = copy.m_headers;
	m_body = copy.m_body;
	m_response = copy.m_response;
}

Response::~Response() {}

Response & Response::operator=(const Response &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

std::string Response::getResponse()
{
	return (m_response);
}

void		Response::buildResponse()
{
	m_response = m_start_line + m_headers + m_body;
}
