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
	m_headers = copy.m_headers;
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
		m_headers = copy.m_headers;
		m_body = copy.m_body;
		m_response = copy.m_response;
	}
	return (*this);
}

void				GetResponse::handleMethod()
{
	// Processes the GET Method and returns the status code
}

std::string			GetResponse::ft_itos(int nbr)
{
	return (static_cast<std::ostringstream&>((std::ostringstream() << std::dec << nbr)).str());
}

void				GetResponse::buildStartLine(ConfigUtil::status_code_map_t& m_error_files)
{
	std::string									str_status_code;
	std::string 								reason_phrase;
	std::string									white_space;
	ConfigUtil::status_code_map_t::iterator		it;

	m_status_code = m_request.getStatus();
	// if (m_status_code != 200)
		/* handle errors */
	str_status_code = ft_itos(m_status_code);

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
	m_start_line = ALLOWED_VERSION + white_space + str_status_code + white_space + reason_phrase + "\r\n\r\n";
}

void				GetResponse::buildHeaders()
{
	
}

void				GetResponse::buildBody()
{
	m_body = "They see me pollin, they hating.\n"; 
}
