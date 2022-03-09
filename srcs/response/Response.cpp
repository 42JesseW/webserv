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

void					Response::handleMethod()
{
	// WIP
}

void					Response::buildStartLine(ConfigUtil::status_code_map_t& m_error_files)
{
	std::string									str_status_code;
	std::string 								reason_phrase;
	std::string									white_space;
	ConfigUtil::status_code_map_t::iterator		it;

	m_status_code = m_request.getStatus();
	// if (m_status_code != 200)
		/* handle errors */
	str_status_code = ft::intToString(m_status_code);

	it = m_error_files.find(m_status_code);
	if (it != m_error_files.end())
		reason_phrase = it->second.first;
	else
	{
		// throw exception?
		std::cout << "Error: status code" <<std::endl;
		std::exit(EXIT_FAILURE);
	}

	white_space = " ";
	m_start_line = ALLOWED_VERSION + white_space + str_status_code + white_space + reason_phrase + CRLF;
}

string_pair_t			Response::_buildDate()
{
	std::time_t		rawtime;
	struct std::tm	*ptm;
	char			buf[50];

	time(&rawtime);
	ptm = gmtime(&rawtime);
	strftime(buf, 500, "%a, %d %b %G %T GMT", ptm);
	return (std::make_pair("Date", std::string(buf)));
}

// WIP 
string_pair_t			Response::_buildLocation()
{
	// we need the route.m_redirect->url member
	return (std::make_pair("Location", ""));
}

string_pair_t			Response::_buildRetryAfter()
{
	return (std::make_pair("Retry-After", RETRY_AFTER_SEC));
}

// WIP
string_pair_t			Response::_buildAllow()
{
	// we need the route.m_accepted_methods and because it's a vector we need to transform it to "GET, POST, DELETE"
	return (std::make_pair("Allow", ""));
}

// not sure if that can mess up the reponse - test and remove if needed
string_pair_t			Response::_buildServer()
{
	return (std::make_pair("Server", "Websurf/1.0.0 (Unix)"));
}

string_pair_t			Response::_buildConnection()
{
    return (std::make_pair("Connection", "closed"));
}

void					Response::buildHeaders()
{
	std::map<std::string, std::string>::iterator	it;

	m_headers_map.insert(_buildDate());
	if (m_status_code == 201 || (m_status_code >= 300 && m_status_code < 400))
		m_headers_map.insert(_buildLocation());
	if (m_status_code == 503 || m_status_code == 429 || (m_status_code >= 300 && m_status_code < 400))
		m_headers_map.insert(_buildRetryAfter());
	if (m_status_code == 405)
		m_headers_map.insert(_buildAllow());
	m_headers_map.insert(_buildServer());
	m_headers_map.insert(_buildConnection());
	// _buildContentLength();
	// _buildContentType();
	// _buildTransferEncoding();

	for (it = m_headers_map.begin(); it != m_headers_map.end(); ++it)
		m_headers_str += (it->first + ": " + it->second + CRLF);

	m_headers_str += "\n";
	// std::cout << m_headers_str << std::endl;
}

void						Response::buildBody()
{
	m_body = "They see me pollin, they hating.";
	m_body += "\r\n"; 
}

void						Response::buildResponse(ConfigUtil::status_code_map_t& m_error_files)
{
	// handleMethod();
	buildStartLine(m_error_files);
	buildHeaders();
	buildBody();
	m_response = m_start_line + m_headers_str + m_body;
	// std::cout << m_response << std::endl;
}
