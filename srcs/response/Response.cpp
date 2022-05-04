#include <Response.hpp>

Response::Response() 
{

}

Response::Response(Request &re) : m_request(re)
{
	m_status_code = m_request.getStatus();
}

Response::Response(Request &re, Route &ro) : m_request(re), m_route(ro)
{
	m_status_code = m_request.getStatus();
}

Response::Response(const Response &copy)
{
	m_request = copy.m_request;
	m_route = copy.m_route;
	m_status_code = copy.m_status_code;
	m_start_line = copy.m_start_line;
	m_headers_map = copy.m_headers_map;
	m_headers_str = copy.m_headers_str;
	m_body = copy.m_body;
	m_response = copy.m_response;
}

Response::~Response()
{
	// delete m_response_cstr;
}

Response & Response::operator=(const Response &copy)
{
	if (this != &copy)
	{
		m_request = copy.m_request;
		m_route = copy.m_route;
		m_status_code = copy.m_status_code;
		m_start_line = copy.m_start_line;
		m_headers_map = copy.m_headers_map;
		m_headers_str = copy.m_headers_str;
		m_body = copy.m_body;
		m_response = copy.m_response;
	}
	return (*this);
}

Request const &									Response::getRequest() const
{
	return (m_request);
}

Route const &									Response::getRoute() const
{
	return (m_route);
}

int const &										Response::getStatusCode() const
{
	return (m_status_code);
}

std::string const &								Response::getStartLine() const
{
	return (m_start_line);
}

std::map<std::string, std::string> const &		Response::getHeadersMap() const
{
	return (m_headers_map);
}

std::string const &								Response::getHeadersStr() const
{
	return (m_headers_str);
}

std::string const &								Response::getBody() const
{
	return (m_body);
}

std::string const &								Response::getResponse() const
{
	return (m_response);
}

// char* 		const &								Response::getResponse_cstr() const
// {
// 	return (m_response_cstr);
// }

void											Response::setRequest(Request &req)
{
	m_request = req;
}

void											Response::setRoute(Route &rou)
{
	m_route = rou;
}

char*											Response::buildResponse(ConfigUtil::status_code_map_t& m_error_files)
{
	buildStartLine(m_error_files);
	buildBody();
	buildHeaders();
	m_response = m_start_line + m_headers_str + m_body;

	char* m_response_cstr = new char[m_response.size()];

	// std::cout << "RESPONSE SIZE " << m_response.size() << std::endl;

	// std::copy_n(m_response.begin(), m_response.size(), m_response_cstr);
	memcpy(m_response_cstr, m_response.data(), m_response.size());
	// if (memcmp(m_response_cstr, m_response.data(), m_response.size()) != 0)
	// 	std::cout << "WRONG MEMCPY" << std::endl;
	// else
	// 	std::cout << "correct memcpy" << std::endl;
 
	// std::cout << "M_RESPONSE_CSTR SIZE IS " << std::strlen(m_response_cstr) << std::endl;

    std::cout << "[DEBUG] Created response " << std::endl;
	// std::cout << m_response << std::endl;

	std::cout << m_response_cstr << std::endl;
	return (m_response_cstr);
}
