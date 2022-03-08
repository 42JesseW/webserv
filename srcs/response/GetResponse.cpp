#include "../includes/GetResponse.hpp"

GetResponse::GetResponse() {}

GetResponse::GetResponse(const Request &r)
{
	m_request = r;
}

GetResponse::GetResponse(const GetResponse &copy)
{
	m_request = copy.m_request;
	m_start_line = copy.m_start_line;
	m_headers_map = copy.m_headers_map;
	m_headers_str = copy.m_headers_str;
	m_body = copy.m_body;
	m_response = copy.m_response;
}

GetResponse::~GetResponse() {}

GetResponse & GetResponse::operator=(const GetResponse &copy)
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

void								GetResponse::handleMethod()
{
	// Processes the GET Method and returns the status code
}

void								GetResponse::buildStartLine(ConfigUtil::status_code_map_t& m_error_files)
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
	m_start_line = ALLOWED_VERSION + white_space + str_status_code + white_space + reason_phrase + CRLF + CRLF;
}

std::pair<std::string, std::string>	GetResponse::_buildDate()
{
	std::time_t		rawtime;
	struct std::tm	*ptm;
	char			buf[50];

	time(&rawtime);
	ptm = gmtime(&rawtime);
	strftime(buf, 500, "%a, %d %b %G %T GMT", ptm);
	return (std::make_pair("Date", buf));
}

void								GetResponse::buildHeaders()
{
	std::map<std::string, std::string>::iterator	it;

	m_headers_map.insert(_buildDate());
	// _buildLocation();
	// _buildRetryAfter();
	// _buildAllow();
	// _buildServer();
	// _buildConnection();
	// _buildContentLength();
	// _buildContentType();
	// _buildTransferEncoding();

	for (it = m_headers_map.begin(); it != m_headers_map.end(); ++it)
		m_headers_str.append(it->first + ": " + it->second + CRLF);

	m_headers_str.append(CRLF);
	// m_headers_str.append("\n");
	// m_headers_str.append(CRLF);
	// std::cout << m_headers_str << std::endl;
}

void								GetResponse::buildBody()
{
	m_body = "They see me pollin, they hating.";
	m_body += CRLF; 
}
