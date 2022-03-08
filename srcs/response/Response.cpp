#include "../includes/Response.hpp"

Response::Response() {}

Response::Response(const Request &r)
{
	m_request = r;
}

Response::Response(const Response &copy)
{
	m_request = copy.m_request;
	m_start_line = copy.m_start_line;
	m_headers_map = copy.m_headers_map;
	m_headers_str = copy.m_headers_str;
	m_body = copy.m_body;
	m_response = copy.m_response;
}

Response::~Response() {}

Response & Response::operator=(const Response &copy)
{
	if (this != &copy)
	{
		m_request = copy.m_request;
		m_start_line = copy.m_start_line;
		m_headers_map = copy.m_headers_map;
		m_headers_str = copy.m_headers_str;
		m_body = copy.m_body;
		m_response = copy.m_response;
	}
	return (*this);
}

std::string const &		Response::getResponse() const
{
	return (m_response);
}

void					Response::buildResponse(ConfigUtil::status_code_map_t& m_error_files)
{
	// handleMethod();
	buildStartLine(m_error_files);
	buildHeaders();
	buildBody();
	m_response = m_start_line + m_headers_str + m_body;
}
