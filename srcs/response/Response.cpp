#include "../includes/Response.hpp"

Response::Response() {}

Response::Response(Request &re, Route &ro) : m_request(re), m_route(ro)
{

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

Response::~Response() {}

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
	strftime(buf, 500, "%a, %d %b %Y %H:%M:%S GMT", ptm);
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

string_pair_t			Response::_buildServer()
{
	return (std::make_pair("Server", "Websurf/1.0.0 (Unix)"));
}

string_pair_t			Response::_buildConnection()
{
    return (std::make_pair("Connection", "close"));
}

string_pair_t			Response::_buildTransferEncoding()
{
	return (std::make_pair("Transfer-Encoding", "chunked"));
}

string_pair_t			Response::_buildContentLength()
{
	int			string_size;
	std::string	str_string_size;

	string_size = m_body.size();
	str_string_size = ft::intToString(string_size);
	return (std::make_pair("Content-Length", str_string_size));
}

// check if there is a case we have different Content-Type
string_pair_t			Response::_buildContentType()
{
	std::string	content_type;

	content_type = "text/html; charset=\"utf-8\"";
	return(std::make_pair("Content-Type", content_type));
}

void					Response::buildHeaders()
{
	std::map<std::string, std::string>::iterator	it;
	// to be removed
	int	chunked = 0;

	m_headers_map.insert(_buildDate());
	if (m_status_code == 201 || (m_status_code >= 300 && m_status_code < 400))
		m_headers_map.insert(_buildLocation());
	if (m_status_code == 503 || m_status_code == 429 || (m_status_code >= 300 && m_status_code < 400))
		m_headers_map.insert(_buildRetryAfter());
	if (m_status_code == 405)
		m_headers_map.insert(_buildAllow());
	m_headers_map.insert(_buildServer());
	m_headers_map.insert(_buildConnection());
	// Content-Length and Transfer-Encoding cannot be used both at the same time. 
	// We use a flag chunked (==cgi i think) to indicate which one needs to be used
	if (chunked)
		m_headers_map.insert(_buildTransferEncoding());
	else
		m_headers_map.insert(_buildContentLength());
	m_headers_map.insert(_buildContentType());

	for (it = m_headers_map.begin(); it != m_headers_map.end(); ++it)
		m_headers_str += (it->first + ": " + it->second + CRLF);

	m_headers_str += "\n";
}

int 					Response::_readFileIntoString(const std::string &path)
{
	std::string new_path;

	new_path = "html/" + path;
	std::ifstream input_file(new_path.c_str());

	if (!input_file.is_open())
		return (0);

	m_body = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

	return (1);
}

void					Response::buildBody()
{
	if (m_request.getMethod() == "GET" && m_status_code == 200)
	{
		std::string							path;
		std::vector<std::string>			path_vector;
		std::vector<std::string>::iterator	iter;

		path_vector = m_route.getIndexFiles();
		path = m_route.getBaseUrl();
		if (path.at(path.length() - 1) == '/')
		{
			for (iter = path_vector.begin(); iter != path_vector.end(); ++iter)
			{
				if (_readFileIntoString(path_vector.at(iter - path_vector.begin())))
					break;	
			}
			if (iter == path_vector.end())
			// to change to what the config file brings
				_readFileIntoString("page_not_found.html");
		}
	}
	if (m_request.getMethod() == "GET" && m_status_code == 404)
		_readFileIntoString("page_not_found.html");
}

void					Response::buildResponse(ConfigUtil::status_code_map_t& m_error_files)
{
	buildStartLine(m_error_files);
	buildBody();
	buildHeaders();
	m_response = m_start_line + m_headers_str + m_body;
}

void					Response::resetResponse()
{
	m_request.resetRequest();
	m_status_code = 0;
	m_start_line.clear();
	m_headers_map.clear();
	m_headers_str.clear();
	m_body.clear();
	m_response.clear();
}
